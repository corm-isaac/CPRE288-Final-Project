'''
@file server.py

@breif main server code that runs the website and establishes a connection with the bot

@author Cooper Sanders
'''
from flask import Flask, render_template
from flask_socketio import SocketIO
import threading, math

from connection import *

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*", async_mode="threading")

# --- GLOBAL STATE ---
fobj = None
sock = None
connected = False
receiver_running = False

lock = threading.Lock()

STEP_DISTANCE = 0.1  # meters per command
STEP_ANGLE = math.radians(10)  # 10 degrees per turn

# --- CONNECT ---
def connect_robot():
    global fobj, sock, connected, receiver_running

    with lock:
        if connected:
            return

        try:
            fobj, sock = open_connection()
            connected = True
            receiver_running = True

            print("[INFO] Connected to robot")
            socketio.emit("status", {"connected": True})

            threading.Thread(target=receiver_loop, daemon=True).start()

        except Exception as e:
            print("[ERROR] Connection failed:", e)
            connected = False
            socketio.emit("status", {"connected": False})


# --- RECEIVER ---
def receiver_loop():
    global connected, receiver_running

    try:
        while receiver_running:
            line = bot2py(fobj)
            line = line.replace("\x00", "").replace("\r", "").strip() # remove  weird data from the bot

            if ":" not in line:
                print("[LOG]", line)
                continue

            header, payload = line.split(":", 1)
            header = header.strip()

            # --- SCAN ---
            if header == "SCAN":
                try:
                    angle, IR, ping = map(float, payload.split())

                    print(f"[SCAN] angle={angle}° IR={IR} dist={ping}cm")
                    socketio.emit("scan", {"angle": angle, "IR": IR, "ping": ping})

                except Exception as e:
                    print("[WARN] Bad SCAN:", payload, e)

            # --- OBJECT ---
            elif header == "OBJECT":
                try:
                    obj_id, start, end, mid, dist, width = payload.split()

                    obj_id = int(obj_id)
                    start = int(start)
                    end = int(end)
                    mid = int(mid)
                    dist_m = float(dist) / 100
                    width = float(width) #overwrite as radial width is ass

                    # --- COMPUTE WIDTH ---
                    #delta_theta = math.radians(end - start)
                    #computed_width = 2 * dist_m * math.sin(delta_theta / 2)

                    width = 2 * dist_m * math.sin((end - start) * 3.141592 / (360.0))
                    print(
                        f"[OBJECT] id={obj_id} "
                        f"midpoint={mid}"
                        f"dist={dist_m:.3f}m "
                        f"span={end - start}° "
                        f"computed_width={width:.3f}m "
                        
                    )

                    socketio.emit("object", {
                        "id": int(obj_id),
                        "start_angle": float(start),
                        "end_angle": float(end),
                        "mid_angle": float(mid),
                        "distance": float(dist_m),
                        "width": float(width),
                    })

                except Exception as e:
                    print("[WARN] Bad OBJECT:", payload, e)

            # --- DEBUG ---
            elif header == "DEBUG":
                socketio.emit("log", {"msg": payload})

            # --- CONTROL ---
            elif header == "SCAN_START":
                socketio.emit("scan_start")

            elif header == "SCAN_END":
                socketio.emit("scan_end")

            elif header == "ANGLE":
                socketio.emit("angle", {"value": float(payload)})
                print(payload)

            # --- UNKNOWN ---
            else:
                print("[UNKNOWN]2", line)

    except Exception as e:
        print("[ERROR] Receiver crashed:", e)

    print("[INFO] Disconnected")
    connected = False
    receiver_running = False
    socketio.emit("status", {"connected": False})


@app.route("/")
def index():
    return render_template("index.html")

@socketio.on("connect_robot")
def handle_connect():
    threading.Thread(target=connect_robot, daemon=True).start()

#get a command from the gui and send it to the bot
@socketio.on("command")
def handle_command(data):

    if not connected or fobj is None:
        return

    cmd = data["cmd"]

    try:
        py2bot(fobj, cmd)
    except Exception as e:
        print("[ERROR] Failed to send command:", e)
        return


if __name__ == "__main__":
    socketio.run(app, host="0.0.0.0", port=5000, debug=True)