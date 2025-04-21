import serial
from pynput import keyboard
import threading

ser = serial.Serial('/dev/ttyACM0', 115200)

step = 100
wartosc = 10
laser_power = 0

def get_delta(val):
    if val < 10:
        return 1
    elif val < 100:
        return 10
    elif val < 1000:
        return 100
    else:
        return 1000

def adjust(val, inc):
    delta = get_delta(val + 1 if inc > 0 else val - 1)
    return clamp(val + inc * delta)

def clamp(val, min_val=0, max_val=99999):
    return max(min_val, min(val, max_val))

def read_serial():
    while True:
        if ser.in_waiting:
            data = ser.readline().decode().strip()
            print(f"Odebrano: {data}")

def on_press(key):
    global step, wartosc, laser_power

    try:
        if key.char == 'a':
            step = adjust(step, 1)
            print(f"Nowy step: {step}")
        elif key.char == 'z':
            step = adjust(step, -1)
            print(f"Nowy step: {step}")
        elif key.char == 's':
            wartosc = adjust(wartosc, 1)
            print(f"Nowa wartość końcowa: {wartosc}")
        elif key.char == 'x':
            wartosc = adjust(wartosc, -1)
            print(f"Nowa wartość końcowa: {wartosc}")
        elif key.char == 'd':
            laser_power = clamp(laser_power + 1, 0, 255)
            print(f"Nowa moc lasera: {laser_power}")
        elif key.char == 'c':
            laser_power = clamp(laser_power - 1, 0, 255)
            print(f"Nowa moc lasera: {laser_power}")
    except AttributeError:
        a, b = 0, 0
        if key == keyboard.Key.right:
            a = step
        elif key == keyboard.Key.left:
            a = -step
        elif key == keyboard.Key.up:
            b = step
        elif key == keyboard.Key.down:
            b = -step
        elif key == keyboard.Key.space:
            msg = f"2,{laser_power}\n"
            ser.write(msg.encode())
            print(f"Wysłano: {msg.strip()}")
            return
        else:
            return

        msg = f"0,{a},{b},{wartosc}\n"
        ser.write(msg.encode())
        print(f"Wysłano: {msg.strip()}")

threading.Thread(target=read_serial, daemon=True).start()
keyboard.Listener(on_press=on_press).run()
