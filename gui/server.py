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

# --- ROBOT POSE (in real-world units, e.g., meters and radians) ---
pose = {"x": 0.0, "y": 0.0, "theta": 0.0}

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
            line = line.replace("\x00", "").replace("\r", "").strip()

            if ":" not in line:
                print("[LOG]", line)
                continue

            header, payload = line.split(":", 1)
            header = header.strip()

            # --- SCAN ---
            if header == "SCAN":
                try:
                    angle, dist = map(float, payload.split())
                    
                    # angle is relative to robot's heading (in degrees, typically)
                    # convert to radians
                    angle_rad = math.radians(angle)
                    
                    # absolute bearing in world frame
                    theta_world = angle_rad + pose["theta"]

                    # convert to Cartesian coordinates in world frame
                    x = pose["x"] + dist * math.cos(theta_world)
                    y = pose["y"] + dist * math.sin(theta_world)

                    print(f"[SCAN] angle={angle}° dist={dist}m -> x={x:.2f}, y={y:.2f}")
                    socketio.emit("scan", {"x": x, "y": y})

                except Exception as e:
                    print("[WARN] Bad SCAN:", payload, e)

            # --- OBJECT ---
            elif header == "OBJECT":
                try:
                    obj_id, start, end, mid, dist, width = payload.split()

                    socketio.emit("object", {
                        "id": int(obj_id),
                        "start": float(start),
                        "end": float(end),
                        "mid": float(mid),
                        "distance": float(dist),
                        "width": float(width)
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

            # --- UNKNOWN ---
            else:
                print("[UNKNOWN]", line)

    except Exception as e:
        print("[ERROR] Receiver crashed:", e)

    print("[INFO] Disconnected")
    connected = False
    receiver_running = False
    socketio.emit("status", {"connected": False})


def pose_broadcast_loop():
    """Broadcast robot pose every second"""
    while True:
        emit_pose()
        socketio.sleep(1)


def emit_pose():
    """Emit current pose to all connected clients"""
    with lock:
        pose_data = {
            "x": pose["x"],
            "y": pose["y"],
            "theta": pose["theta"]
        }
    socketio.emit("pose", pose_data)


threading.Thread(target=pose_broadcast_loop, daemon=True).start()


# --- ROUTE ---
@app.route("/")
def index():
    return render_template("index.html")


# --- CONNECT BUTTON ---
@socketio.on("connect_robot")
def handle_connect():
    threading.Thread(target=connect_robot, daemon=True).start()


def normalize_angle(angle):
    """Normalize angle to [-pi, pi]"""
    while angle > math.pi:
        angle -= 2 * math.pi
    while angle < -math.pi:
        angle += 2 * math.pi
    return angle


# --- COMMAND ---
@socketio.on("command")
def handle_command(data):
    global pose

    if not connected or fobj is None:
        return

    cmd = data["cmd"]

    try:
        py2bot(fobj, cmd + "\n")
    except Exception as e:
        print("[ERROR] Failed to send command:", e)
        return

    # --- UPDATE POSE ---
    with lock:
        if cmd == 'w':  # Forward
            pose["x"] += STEP_DISTANCE * math.cos(pose["theta"])
            pose["y"] += STEP_DISTANCE * math.sin(pose["theta"])
            print(f"[CMD] Forward -> pose: x={pose['x']:.2f}, y={pose['y']:.2f}")

        elif cmd == 's':  # Backward
            pose["x"] -= STEP_DISTANCE * math.cos(pose["theta"])
            pose["y"] -= STEP_DISTANCE * math.sin(pose["theta"])
            print(f"[CMD] Backward -> pose: x={pose['x']:.2f}, y={pose['y']:.2f}")

        elif cmd == 'a':  # Turn left (counterclockwise)
            pose["theta"] += STEP_ANGLE
            pose["theta"] = normalize_angle(pose["theta"])
            print(f"[CMD] Left -> theta={math.degrees(pose['theta']):.1f}°")

        elif cmd == 'd':  # Turn right (clockwise)
            pose["theta"] -= STEP_ANGLE
            pose["theta"] = normalize_angle(pose["theta"])
            print(f"[CMD] Right -> theta={math.degrees(pose['theta']):.1f}°")

    emit_pose()


if __name__ == "__main__":
    socketio.run(app, host="0.0.0.0", port=5000, debug=True)