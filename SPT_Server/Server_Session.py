#!/usr/bin/env python
import threading
import asyncio
import uuid

from Server_Log import info_log, log_function_response, log_function
from Server_Utilities import Xolor, check_ip, check_port

class Session(threading.Thread):
    def __init__(self, addr, port, session_type, session_index, logger):
        """
        Listen for, or Connect to, a remote host

        Args:
            addr: Remote (or local) IPv4/IPv6 address
            port: Remote (or local) port
            type: Listen (0) or Connection (1)
            logger: Super object that provides logging

        Returns:
            None
        """
        super(Session, self).__init__() # Subclass of Thread
        self.daemon = True
        self.cancelled = False

        self.logger = logger
        self.session_index = session_index
        self.addr = addr
        self.port = port
        self.session_type = session_type

        self.session_uuid = str(uuid.uuid4())
        self.cmd_socket = ()
        self.data_socket = ()
        self.shell_socket = ()

        self.loop = asyncio.get_event_loop()

    def __str__(self):
        """
        Returns a representation of the object as a string
        """
        return f"Session 0 - {self.addr}:{self.port}"

    def run(self):
        """Override the run(_) function of Thread class"""
        #print("(asyncio) Starting session thread")
        asyncio.run(self.handle_cmd_socket(self.addr, self.port, self.session_type, self.cmd_socket))

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

    def close_session(self, index: int):
        """
        Close the session nicely

        Args:
            None

        Returns:
            None
        """
        info_log(self, "Ended session")
        return
        #print("Trying to close handlers...")
        #try:
        #    handler = self.logger.handlers[index]
        #    handler.close()
        #    self.logger.removeHandler(handler)
        #except IndexError:
        #    print("handlers already empty?")
        #    print(self.logger.handlers)

    async def handle_cmd_socket(self, addr, port, session_type, sock):
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

            #srv = await asyncio.start_server(self.listen_session, addr, port)
            srv = await asyncio.start_server(lambda r, w: self.init_session(r, w, sock), addr, port)
            srv_info = srv.sockets[0].getsockname()
            print(Xolor.INFO + f"Listening on {srv_info[0]}:{srv_info[1]} for connection as session {self.session_index}" + Xolor.END)

            async with srv:
                await srv.serve_forever()

        elif (1 == session_type):
            print(f"Attempting a connection to {addr}:{port}")
            reader, writer = asyncio.open_connection(addr, port)
        else:
            print(Xolor.ERROR + "Unknown session_type" + Xolor.END)

    async def listen_session(self, reader, writer, sock):
        await self.init_session(reader, writer)

    async def init_session(self, reader, writer, sock):
        sock = (reader, writer)
        client_addr = writer.get_extra_info('peername')
        data = await reader.read(37)
        message = data.decode()
        print(f"Received connect from {client_addr}: {message}")
        print(f"Sending message: {self.session_uuid}")
        #send_msg = f"Hello! {message}".encode()
        writer.write(self.session_uuid.encode())
        await writer.drain()
        print("Closing connection")
        writer.close()

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

"""Resources
 - https://docs.python.org/3/library/asyncio-stream.html
"""