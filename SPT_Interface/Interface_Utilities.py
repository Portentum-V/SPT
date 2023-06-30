from platform import system
import re

class Xolor:
    import os
    if os.name == "nt": # Do this to get colors to work on Windows 10+
        import subprocess
        subprocess.call('', shell=True)
    # Foreground
    CYAN = '\033[96m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BOLD = '\033[1m'
    UNDER = '\033[4m'
    ERROR = RED + '[!] '
    WARN = YELLOW + '[*] '
    OTHR = YELLOW + '[~] '
    INFO = GREEN + '[i] '
    SUCC = CYAN + '[+] '
    # End string
    END = '\033[0m'

# Type Validation #
def check_system():
    return system()

def check_ip(ip):
    octet = '([0-1]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))'
    valid_ip = f'({octet}\.){{3}}{octet}'
    if re.fullmatch(valid_ip, ip):
        return ip
    else:
        raise ValueError(Xolor.ERROR + f'Invalid IP: {ip}' + Xolor.END)

def check_port(port):
    try:
        port = int(port)
        if 0 <= port <= 65535:
            return port
        else:
            raise ValueError
    except ValueError:
        raise ValueError(Xolor.ERROR + f'Invalid port: {port}' + Xolor.END)

def input_confirm(prompt='(Y/n) ', default='y'):
    confirm = None
    while not confirm:
        confirm = input(prompt) or default
        if confirm.lower().startswith('y'):
            return True
        elif confirm.lower().startswith('n'):
            return False
        else:
            confirm = None

def input_int(prompt='Enter an integer: ', default='', minInt=0, maxInt=1):
    while True:
        num = input(prompt) or default
        try:
            num = int(num)
            if minInt <= num <= maxInt:
                return num
            else:
                print(Xolor.ERROR + f"Integer not within a valid range ({minInt} - {maxInt})! GTFO." + Xolor.END)
        except ValueError:
            print(Xolor.ERROR + f'Invalid integer: {num}' + Xolor.END)

# Useful Functions #
def recvUntil(sock, delim=b''):
    data = b''
    while not data.endswith(delim):
        data += sock.recv(1)
    print(data)
    return data
