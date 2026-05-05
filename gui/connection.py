import socket

TCP_HOST = "192.168.1.1"
TCP_PORT = 288

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

def bot2py(fobj):
    raw = fobj.readline()
    if raw == b"":
        raise ConnectionError("robot disconnected")
    return raw.decode(errors="replace")

def py2bot(fobj, msg):
    fobj.write(msg.encode())
    try:
        fobj.flush()
    except:
        pass
