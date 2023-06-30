#!/usr/bin/env python
import threading
import datetime
import errno

from Server_Log import SPTLog, log_function_response, log_function
from Server_Session import Session

## Documentation for a class
#
# More details
class Manager(SPTLog):
    """
    Class that manages a set of sessions
    """
    def __init__(self):
        super(Manager, self).__init__() # Initializes SPTLog
        self.current_session_index = 0
        self.session_dict = dict()
        self.session_dict_lock = threading.Lock()

    def create_session(self, addr, port, session_type):
        """Add and instantiate a session (listen or connect)
        index : [ sessionObject, list() ]

        Args:
            addr: An address (IPv4 or IPv6)
            port: String 0 - 65535
            type: Listen (0) or Connect (1)

        Returns:
            An int reference to the session (index)
        """

        # Build and start the thread
        new_session = Session(addr, port, session_type, self.current_session_index, self.logger)
        new_session.start()
        # wait until thread is initiliazed (read_event set) 
        new_session.ready_event.wait()
        # If there was an error, don't add the index to the dictionary
        if new_session.error == True:
            return self.current_session_index

        with self.session_dict_lock:
            self.session_dict[self.current_session_index] = [new_session, list()]

        self.current_session_index += 1
        return self.current_session_index - 1

    def delete_session(self, index):
        """Removes and closes a session by index

        Args:
            index: Index of session to delete

        Returns:
            None
        """
        with self.session_dict_lock:
            # Dict returns the key first followed by a list of vlaues ( index : [ sessionObject, list() ] )
            session, _ = self.session_dict.pop(index) 
        session.close_session()
        session.cancel()

    def list_sessions(self):
        """Returns a list of (index, addr, port) tuples

        Args:
            None

        Returns:
            list of (index, addr, port) tuples
        """
        with self.session_dict_lock:
            return [ (index, self.session_dict[index][0].addr, 
                      self.session_dict[index][0].port)
                    for index in sorted(self.session_dict.keys())
                    ]

    def join_session(self, index: int) -> Session:
        """Join a session object and issue it commands

        Args:
            index: The index for the session to be joined

        Returns:
            None
        """
        #print(f"Joining session {index}")
        with self.session_dict_lock:
            session, _ = self.session_dict[index]
        return session

    def delete_all_sessions(self):
        """Closes all sessions

        Args:
            None

        Returns:
            None
        """
        for index in self.session_dict.copy().keys():
            print(f"Closing: {index}")
            self.delete_session(index)

"""Refrences
 - https://realpython.com/python-super/
 - https://google.github.io/styleguide/pyguide.html
"""