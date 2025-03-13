import socket 
import sys
from threading import Thread 


def show_help():
    help = """
    About:
        A simple tool writen in python for network communication.
        It can be used to make simple server and clients using python.

    Options:
        -l          listen mode [listen on specific host and port]
        -c          client mode [connect to specific host and port]

    Order:
        python nc.py <option> <host> <port>

    Examples:
        python nc.py -l 0.0.0.0 80  [Will listen in all interface in port 80]
        python nc.py -c 192.168.1.160 53 [Connect to server 192.168.1.160 in port 53]
    
    Author:
        MD.Bayazid ssc batch 2025

    """
    print(help)

a= sys.argv
print(a)
if len(a)<4:
    show_help()
    sys.exit()

listen = True
connect = False

if '-c' in a and not "-l" in a:
    listen = False 
    connect = True 
elif '-l' in a and not "-c" in a:
    listen = True
    connect = False
else:
    show_help()
    sys.exit()

host = a[2]
port = int(a[3])


class server:
    def __init__(self , host , port ):
        self.host = host 
        self.port = port 
        self.server()
    def receiver(self):
        while True:
            try:
                res = self.client.recv(1024*1024)
                if not res :
                    continue 
                try:
	   
                    res = res.decode()
                except:
                    pass 
                print(res)
            except KeyboardInterrupt:
                self.client.close()
                sys.stdout.write("Exiting by user requests!")
                sys.exit()
            except Exception as e:
                print(f"[!] [error] {e}")
    def sender(self):
        while True:
            try: 
                cmd = input("[>] ")
                self.client.sendall(cmd.encode()+b'\n')
            except KeyboardInterrupt :
                sys.stdout.write("[!] Exiting by user requets!")
                self.client.close()
                sys.exit()
            except Exception as e:
                print(f"[!] [error] {e}")
    def server(self):
        s = socket.socket()
        s.bind((self.host , self.port))
        s.listen(5)
        print(f"[*] Server listening on {self.host}:{self.port}")
        self.client , a = s.accept()
        print(f"[*] Server get connection from {a}")
        t = Thread(target= self.receiver)
        t.daemon = True
        t.start()
        self.sender()

class client(server):
    def __init__(self, host , port ):
        self.host = host 
        self.port = port 
        self.connect_to_server()
    def connect_to_server(self):
        try:
            self.client = socket.socket()
            self.client.connect((self.host , self.port))
        except Exception as e :
            print(f"[!] [error] {e}")
            sys.exit()
        else:
            t = Thread(target=self.receiver)
            t.daemon = True
            t.start()

            self.sender()

if listen and not connect:
    server(host , port )
else:
    client(host , port )