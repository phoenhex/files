import argparse
import socket
import sys
import threading
import time

start_time = time.time()

def cur():
    t = time.time() - start_time
    return '%02d:%02d.%03d' % (t//60, t%60, int(t * 1000) % 1000)

def recv_until(sock, term):
    buf = ''
    while True:
        c = sock.recv(1)
        if not c:
            return None
        if c == term:
            return buf
        buf += c

def log_handler(client, addr):
    print '[*] Got logging connection from %s:%d' % addr
    while True:
        svc = recv_until(client, ':')
        if svc is None:
            return
        line = recv_until(client, '\n')
        if line is None:
            return
        sys.stdout.write('[%s %.4s] %s\n' % (cur(), svc, line))
        sys.stdout.flush()

class ThreadedTCPServer(object):
    def __init__(self, host, port, handler):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))
        self.handler = handler

    def listen(self):
        self.sock.listen(5)
        while True:
            client, addr = self.sock.accept()
            t = threading.Thread(target = self.handler, args = (client, addr))
            t.daemon = True
            t.start()

if __name__ == '__main__':
    p = argparse.ArgumentParser()
    p.add_argument('--host', default='127.0.0.1')
    p.add_argument('--port', type=int, default=1337)
    args = p.parse_args()

    print '[*] Listening on %s:%d' % (args.host, args.port)
    ThreadedTCPServer(args.host, args.port, log_handler).listen()
