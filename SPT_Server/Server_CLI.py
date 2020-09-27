#!/usr/bin/env python
import cmd
from Server_Utilities import Xolor, check_ip, check_port
from Server_Manager import Manager

try:
    from pyreadline import Readline
    readline = Readline()
except ImportError:
    print(Xolor.WARN + "Looks like you don't have pyreadlines installed, tab comleteion won't work :(" + Xolor.END)
    pass

class manager_CLI(cmd.Cmd, object):
    def __init__(self, manager):
        cmd.Cmd.__init__(self)
        self.manager = manager
        self.prompt = '(mngr)$ '

    def do_exit(self):
        """Exits manager_CLI

        Args:
            None

        Returns:
            None
        """
        print("Closing manager...")
        self.manager.delete_all_sessions()
        print("Goodbye!")
        quit()
    do_EOF = do_exit
    do_quit = do_exit
    do_q = do_exit

    def help_exit(self):
        print('Exit and close all sessions; alt Ctrl-D')

    def do_listen(self, input_str):
        """Starts a listening session via create_session(addr, port, sock_type [0 = listen, 1 = connect]

        Args:
            input: string of '<ip> <port>'

        Returns:
            None
        """
        addr, port = input_str.split()
        try:
            check_ip(addr)
            check_port(port)
        except ValueError as e:
            print(e)
            return

        new_index = self.manager.create_session(addr, port, 0)
        print(f"Starting a listener on {addr}:{port} as session {new_index}")

    def help_listen(self):
        print("Listen for a connection")

    def do_connect(self, input_str):
        print(f"Connecting to {input_str}.")

    def help_connect(self):
        print("Connect to a remote socket")

    def do_session(self, input_str):
        index = int(input_str.split()[0])
        indexs = [ i[0] for i in self.manager.list_sessions() ]
        if index not in indexs:
            print(Xolor.WARN + f"Invalid index: {index}" + Xolor.END)
            return
        print(f"Joining session {index}.")
        session = self.manager.join_session(index)
        sess_cli = session_CLI(session, index)
        sess_cli.cmdloop()

    def do_list(self, input_str):
        print(Xolor.CYAN + "Active Sessions" + Xolor.END)
        for index, addr, port in self.manager.list_sessions():
            print(f"{index} - {addr}:{port}")

    def do_remove(self, input_str):
        print("Removing input_str")



class session_CLI(cmd.Cmd, object):
    def __init__(self, session, index: int):
        cmd.Cmd.__init__(self)
        self.session = session
        self.prompt = f'(session-{index})# '
    def do_exit(self, input_str):
        print("Leaving session...")
        #manager.close()
        print("Goodbye!")
        quit()
    do_EOF = do_exit
    do_quit = do_exit
    do_q = do_exit
    def help_exit(self):
        print('Exit session; alt Ctrl-D')

    def do_burn(self):
        print(Xolor.WARN + "Closing Session" + Xolor.END)

    def help_burn(self):
        print('Burn session, attempting to stop the remote client and end the session')

    def do_put(self, input_str):
        print("put <from local> <to remote>")

    def do_get(self, input_str):
        print("get <from remote> <to local>")

    def do_shell(self, input_str):
        print("Dropping into remote shell")

def main():
    print(Xolor.CYAN + "Starting STP Server" + Xolor.END)
    manager = Manager()
    mgr_cli = manager_CLI(manager)
    mgr_cli.cmdloop()

if __name__ == "__main__":
    main()

