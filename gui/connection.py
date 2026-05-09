'''
@file connection.py

@breif connection code and code to recieve and send data

@author Cooper Sanders
'''
import socket

TCP_HOST = "192.168.1.1"
TCP_PORT = 288

#connect to the bot and return a socket we can use to send and recieve data from
def open_connection():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(10)
    sock.connect((TCP_HOST, TCP_PORT))
    sock.settimeout(None)
    return sock.makefile("rwb", buffering=0), sock

def close_connection(fobj, sock):
    for obj in (fobj, sock):
        if obj:
            try:
                obj.close()
            except:
                pass

#read data sent from the bot
def bot2py(fobj):
    raw = fobj.readline()
    if raw == b"":
        raise ConnectionError("robot disconnected")
    return raw.decode(errors="replace")

#send commands to the bot. currently only single char commands are configured look at manual.c
def py2bot(fobj, msg):
    fobj.write(msg.encode())
    try:
        fobj.flush()
    except:
        pass
