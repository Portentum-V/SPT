#!/usr/bin/env python
import uuid
import threading

import asyncio

from Interface_Log import info_log, error_log, log_function_response, log_function
from Interface_Utilities import Xolor, check_ip, check_port

class Session(threading.Thread):
    def __init__(self, addr, port, session_type, session_index, logger):
        """
        Listen for, or Connect to, a remote host

        Args:
            addr: Remote (or local) IPv4/IPv6 address
            port: Remote (or local) port
            sesison_type: Listen (0) or Connection (1)
            session_index: Dict index for this session
            logger: Super object that provides logging

        Returns:
            None
        """
        super(Session, self).__init__() # Subclass of Thread
        self.daemon = True
        self.cancelled = False

        self.ready_event = threading.Event()
        self.logger = logger
        self.session_index = session_index
        self.addr = addr
        self.port = port
        self.session_type = session_type

        self.session_uuid = str(uuid.uuid4())
        self.cmd_socket = ()
        self.data_socket = ()
        self.shell_socket = ()

        self.error = False

        self.loop = asyncio.new_event_loop()
        self.loop.set_exception_handler(self.handle_exception)

    def __str__(self):
        """
        Returns a representation of the object as a string
        """
        return f"Session {self.session_index} [{self.addr}:{self.port}]"

    def handle_exception(self, loop, context):
        """
        This doesn't seem to work...
        """
        msg = context.get("exception", context["message"])
        error_log(self, msg)
        name = context.get("future").get_coro().__name__
        if name == "handle_socket":
            print(Xolor.ERROR + "That socket is already in use" + Xolor.END)
        self.error = True
        self.close_session()
        return

    async def shutdown(loop, signal=None):
        if signal:
            info_log(f"Received exit signal: {signal.name}")


    def run(self):
        """Override the run(_) function of Thread class"""
        info_log(self, "Starting cmd socket")
        self.loop.create_task(self.handle_socket(self.addr, self.port, self.session_type, self.cmd_socket))
        try:
            self.loop.run_forever()
        except AssertionError:
            self.error = True
            self.close_session()
            return

    def cancel(self):
        """End this thread"""
        self.cancelled = True

    def __str__(self) -> str:
        """
        Representation of the object as a string (used by logger)

        Args:
            None

        Returns:
            string
        """
        return f"Session {self.addr}:{self.port} {self.session_type}"

    def close_session(self):
        """
        Close the session nicely

        Args:
            None

        Returns:
            None
        """
        if self.cmd_socket:
            print("Closing cmd socket")
            self.cmd_socket[1].close()
        if self.data_socket:
            print("Closing data socket")
            self.data_socket[1].close()
        if self.shell_socket:
            print("Closing shell socket")
            self.shell_socket[1].close()
        #self.loop.stop()
        info_log(self, "Ended session clean")
        self.ready_event.set()
        self.cancel()
        return
        #print("Trying to close handlers...")
        #try:
        #    handler = self.logger.handlers[index]
        #    handler.close()
        #    self.logger.removeHandler(handler)
        #except IndexError:
        #    print("handlers already empty?")
        #    print(self.logger.handlers)

    async def handle_socket(self, addr, port, session_type, sock):
        """
        Handles initial network configuration for the session

        Args:
            addr: Remote (or local) IPv4/IPv6 address
            port: Remote (or local) port
            session_type: Listen (0) or Connection (1)

        Returns:
            None
        """
        if (0 == session_type):

            srv = await asyncio.start_server(self.init_session, addr, port)
            #srv = await asyncio.start_server(lambda r, w: self.init_session(r, w, sock), addr, port)
            srv_info = srv.sockets[0].getsockname()
            print(Xolor.INFO + f"Listening on {srv_info[0]}:{srv_info[1]} for connection as session {self.session_index}" + Xolor.END)

            async with srv:
                self.ready_event.set()
                await srv.serve_forever()
                

        elif (1 == session_type):
            print(f"Attempting a connection to {addr}:{port}")
            reader, writer = asyncio.open_connection(addr, port)

        else:
            print(Xolor.ERROR + "Unknown session_type" + Xolor.END)

    async def listen_session(self, reader, writer, sock):
        await self.init_session(reader, writer)

    async def init_session(self, reader, writer):
        client_addr = writer.get_extra_info('peername')
        #data = await reader.read(5)
        # First read the amount of the next message?
        data = await reader.read(43)
        message = data.decode().split()
        print(Xolor.SUCC + f"Received connect from {client_addr}: {message}" + Xolor.END)
        if message[0] == "000000000-0000-0000-0000-000000000000" and message[1] == "init\x00":
            print(f"Sending message: {self.session_uuid}")
            writer.write(self.session_uuid.encode())
            self.cmd_socket = (reader, writer)
        elif message[0] == self.session_uuid:
            if message[1] == "gets\x00":
                print("Downloading file from client to server")
            elif message[1] == "puts\x00":
                print("Uploading file from server to client")
            elif message[1] == "shel\x00":
                print("Shell callback from client")
            else:
                print(Xolor.WARN + f"Should I handle this command? : {message[1]}" + Xolor.END)
        else:
            print(Xolor.WARN + f"Not sure who that is that connected, but its not for this session..." + Xolor.END)
            send_msg = f"Hello! {message}".encode()
        
        await writer.drain()


    async def connect_session(self, reader, writer):
        """

        Args:
            None

        Returns:
            None
        """
        print("Connect")

    async def send_file(self, file, reader, writer):
        """

        Args:
            None

        Returns:
            None
        """
        print(f"Sending file {file}")

    async def recv_file(self, file, reader, writer):
        """

        Args:
            None

        Returns:
            None
        """
        print(f"Recving file {file}")

    def send_msg(self, msg):
        if self.cmd_socket:
            reader = self.cmd_socket[0]
            writer = self.cmd_socket[1]

            writer.write(msg.encode())
            data = reader.read(10)
            print(f"Message: {data.decode()}")
        else:
            print("CMD socket not yet built!")
            return -1

"""Resources
 - https://docs.python.org/3/library/asyncio-stream.html
"""