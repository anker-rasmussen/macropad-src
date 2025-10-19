#!/usr/bin/env python3

import requests
import threading
from pynput import keyboard

# Configuration
SPEAKER_ENDPOINT = "{some_endpoint (see sonos controller.js repo)}"
current_volume = 50
MIN_VOLUME = 0
MAX_VOLUME = 100
VOLUME_STEP = 5

class VolumeController:
    def __init__(self):
        self.volume = current_volume
        self.debounce_timer = None
        self.debounce_time = 0.5  

    def update_volume(self, direction):
        if self.debounce_timer:
            self.debounce_timer.cancel()

        if direction == "up":
            self.volume = min(MAX_VOLUME, self.volume + VOLUME_STEP)
        else:  # down
            self.volume = max(MIN_VOLUME, self.volume - VOLUME_STEP)

        print(f"→ Volume: {self.volume}% ({direction})")

        self.debounce_timer = threading.Timer(self.debounce_time, self.send_volume_request)
        self.debounce_timer.start()

    def send_volume_request(self):
        print(f"  ...sending volume {self.volume}%")
        try:
            payload = {"volume": self.volume}
            response = requests.post(
                SPEAKER_ENDPOINT,
                json=payload,
                timeout=2
            )
            # man i love emojis
            if response.status_code in (200, 202):
                print("  ✅  POST successful")
            else:
                print(f"  ❎ Error: HTTP {response.status_code}")

        except requests.exceptions.ConnectionError:
            print("  ❎ Connection error - check endpoint URL")
        except requests.exceptions.Timeout:
            print("  ❎ Request timeout")
        except requests.exceptions.RequestException as e:
            print(f"  ❎ Error: {e}")

pressed_keys = set()

controller = None

def on_press(key):
    if key in {keyboard.Key.ctrl, keyboard.Key.shift, keyboard.Key.f11, keyboard.Key.f12}:
        pressed_keys.add(key)
        check_hotkeys()

def on_release(key):
    if key in pressed_keys:
        pressed_keys.remove(key)

def check_hotkeys():
    UP_HOTKEY = {keyboard.Key.ctrl, keyboard.Key.shift, keyboard.Key.f12}
    DOWN_HOTKEY = {keyboard.Key.ctrl, keyboard.Key.shift, keyboard.Key.f11}

    if UP_HOTKEY.issubset(pressed_keys):
        if controller:
            controller.update_volume("up")
    elif DOWN_HOTKEY.issubset(pressed_keys):
        if controller:
            controller.update_volume("down")

def main():
    print("=" * 60)
    print(" Global Volume Control Listener")
    print("=" * 60)
    print(f"Endpoint:      {SPEAKER_ENDPOINT}")
    print(f"Start Volume:  {current_volume}%")
    print(f"Volume Step:   ±{VOLUME_STEP}%")
    print(f"Range:         {MIN_VOLUME}% - {MAX_VOLUME}%")
    print("\nListening for hotkeys...")
    print("  - Ctrl+Shift+F12: Volume Up")
    print("  - Ctrl+Shift+F11: Volume Down")
    print("\nPress Ctrl+C in the terminal where this script was started to exit.")
    print("-" * 60)

    global controller
    controller = VolumeController()

    listener = keyboard.Listener(on_press=on_press, on_release=on_release)
    listener.start()

    try:
        # Keep the main thread alive
        listener.join()
    except KeyboardInterrupt:
        print("\nExiting...")
        listener.stop()
    finally:
        if controller and controller.debounce_timer:
            controller.debounce_timer.cancel()
        print("Script terminated.")


if __name__ == "__main__":
    main()