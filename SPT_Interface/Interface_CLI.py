#!/usr/bin/env python
import cmd

from Interface_Utilities import Xolor, check_ip, check_port
from Interface_Manager import Manager

try:
    from pyreadline import Readline
    readline = Readline()
except ImportError:
    print(Xolor.WARN + "Looks like you don't have pyreadlines installed, tab completion won't work :(" + Xolor.END)
    pass

class manager_CLI(cmd.Cmd, object):
    def __init__(self, manager):
        cmd.Cmd.__init__(self)
        self.manager = manager
        self.prompt = Xolor.BOLD + '(~)$ ' + Xolor.END

    def do_exit(self, input_str):
        """Exits manager_CLI

        Args:
            input_str: if 'BURN' quit without confirmation

        Returns:
            None
        """
        if input_str == "BURN":
            print("Closing manager...")
            self.manager.delete_all_sessions()
            print("Goodbye!")
            return(1)
        print(Xolor.WARN + "This will close ALL sessions" + Xolor.END)
        burn = input("Type BURN if you wish to continue: ")
        if burn == "BURN":
            print("Closing manager...")
            self.manager.delete_all_sessions()
            print("Goodbye!")
            return(0)
        else:
            pass
    do_EOF = do_exit
    do_quit = do_exit
    do_q = do_exit

    def help_exit(self):
        print('Exit and close all sessions; alt Ctrl-D')

    def emptyline(self):
        pass

    def do_listen(self, input_str):
        """Starts a listening session via create_session(addr, port, sock_type [0 = listen, 1 = connect]

        Args:
            input_str: string of '<ip> <port>'

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
        #print(Xolor.INFO + f"Starting a listener on {addr}:{port} as session {new_index}" + Xolor.END)

    def help_listen(self):
        print("Listen for a connection")

    def do_connect(self, input_str):
        print(f"Connecting to {input_str}.")

    def help_connect(self):
        print("Connect to a remote socket")

    def do_session(self, input_str):
        try:
            index = int(input_str.split()[0])
            indexs = [ i[0] for i in self.manager.list_sessions() ]
        except Exception as e:
            print(Xolor.WARN + f"Invalid index: {input_str}\n{e}" + Xolor.END)
            return
        if index not in indexs:
            print(Xolor.WARN + f"Invalid index: {index}" + Xolor.END)
            return
        session = self.manager.join_session(index)
        sess_cli = session_CLI(session, index)
        sess_cli.cmdloop()

    def help_session(self):
        print("Join a running session - 'session 0'")

    def do_list(self, input_str):
        print(Xolor.CYAN + "Active Sessions" + Xolor.END)
        for index, addr, port in self.manager.list_sessions():
            print(f"{index} - {addr}:{port}")

    def help_list(self):
        print("List active sessions")

    def do_remove(self, input_str):
        index = int(input_str.split()[0])
        print(f"Closing session {index}")
        self.manager.delete_session(index)

    def help_remove(self):
        print("Close a session")

class session_CLI(cmd.Cmd, object):
    def __init__(self, session, index: int):
        cmd.Cmd.__init__(self)
        self.session = session
        self.prompt = f'(session-{index})# '

    def do_exit(self, input_str):
        print("Leaving session...")
        return True
    do_EOF = do_exit
    do_quit = do_exit
    do_q = do_exit

    def help_exit(self):
        print('Exit session; alt Ctrl-D')

    def emptyline(self):
        pass

    def do_burn(self, input_str):
        print(Xolor.WARN + "Closing Session" + Xolor.END)
        self.session.close_session()
        self.do_exit()

    def help_burn(self):
        print('Burn session, attempting to stop the remote client and end the session')

    def do_put(self, input_str):
        print("put <from local> <to remote>")

    def do_get(self, input_str):
        print("get <from remote> <to local>")

    def do_shell(self, input_str):
        print("Dropping into remote shell")

    def do_send(self, input_str):
        self.session.send_msg(input_str)

def main():
    print(Xolor.CYAN + "Starting STP Interface" + Xolor.END)
    manager = Manager()
    mgr_cli = manager_CLI(manager)
    try:
        mgr_cli.cmdloop()
    except KeyboardInterrupt:
        manager.delete_all_sessions()
    return 1

if __name__ == "__main__":
    main()

