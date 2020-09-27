#!/usr/bin/env python
import asyncio
import socket

from Server_Log import info_log, log_function_response, log_function
from Server_Utilities import Xolor, check_ip, check_port

class Session(object):
    def __init__(self, addr, port, session_type, logger):
        """Listen for, or Connect to, a remote host

        Args:
            addr: Remote (or local) IPv4/IPv6 address
            port: Remote (or local) port
            type: Listen (0) or Connection (1)
            logger: Super object that provides logging

        Returns:
            None
        """
        self.logger = logger
        self.addr = addr
        self.port = port
        self.session_type = session_type

    def __str__(self) -> str:
        """Representation of the object as a string (used by logger)

        Args:
            None

        Returns:
            string
        """
        return f"Session {self.addr}:{self.port} {self.session_type}"

    def close_session(self, index: int):
        """Close the session nicely

        Args:
            None

        Returns:
            None
        """
        print("Closing session")
        info_log(self, "Ended session")
        try:
            handler = self.logger.handlers[index]
            handler.close()
            self.logger.removeHandler(handler)
        except IndexError:
            print("handlers already empty?")
            print(self.logger.handlers)

    async def handle_session(self, addr, port, session_type):
        """Handles initial network configuration for the session

        Args:
            addr: Remote (or local) IPv4/IPv6 address
            port: Remote (or local) port
            session_type: Listen (0) or Connection (1)

        Returns:
            None
        """
        if (0 == session_type):

            srv = asyncio.start_server(listen_session, addr, port)
            srv_info = srv.sockets[0].getsockname()
            print("Listening on {srv_info} for connection")

            async with srv:
                await srv.serve_forever()

        elif (1 == session_type):
            print("Attempting a connection to {addr}:{port}")
            reader, writer = asyncio.open_connection(addr, port)
        else:
            print(Xolor.ERROR + "Unknown session_type" + Xolor.END)

    async def listen_session(self, reader, writer):
        client_addr = writer.get_extra_info('peername')
        print(f"Received connect from {client_addr}")
        print("Closing connection")
        writer.close()

    async def connect_session(self, reader, writer):
        print("Connect")

"""Resources
 - https://docs.python.org/3/library/asyncio-stream.html
"""