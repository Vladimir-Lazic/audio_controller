import tkinter as tk
from tkinter import ttk
from tkdial import Dial as TKDial
import socket
import threading
import struct
import time
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from enum import Enum

# ======= ENUMS AND CONFIGURATION =======

class WaveformType(Enum):
    Sine = 0
    Sawtooth = 1
    Triangle = 2
    Square = 3
    WhiteNoise = 4

class WaveformState:
    def __init__(self):
        self.running = False

# ======= COMMUNICATION / SENDER LOGIC =======

class WaveformSender:
    def __init__(self, send_address):
        self.send_address = send_address
        self.send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def send(self, message: str):
        self.send_sock.sendto(message.encode("utf-8"), self.send_address)

# ======= WIDGET BUILDERS / MODULAR UI COMPONENTS =======

class WaveformControls(ttk.Frame):
    def __init__(self, master, sender: WaveformSender, state: WaveformState, **kwargs):
        super().__init__(master, **kwargs)
        self.sender = sender
        self.build_controls()
        self.state = state

    def build_controls(self):
        self.waveform_type = tk.StringVar(value="Sine")
        self.amplitude = tk.DoubleVar(value=1.0)
        self.frequency = tk.DoubleVar(value=440.0)
        self.phase = tk.DoubleVar(value=0.0)
        self.sample_rate = tk.IntVar(value=44100)

        # Waveform selector
        ttk.Label(self, text="Waveform Type").grid(row=0, column=0, sticky="w")
        waveform_menu = ttk.OptionMenu(
            self,
            self.waveform_type,
            "Sine",
            *[w.name for w in WaveformType],
            command=lambda e: self.on_param_change(),
        )
        waveform_menu.grid(row=1, column=0)

        # Amplitude Slider
        ttk.Label(self, text="Amplitude").grid(row=2, column=0, sticky="w")
        amp_slider = ttk.Scale(
            self,
            from_=0.0,
            to=1.0,
            variable=self.amplitude,
            orient="horizontal",
            command=lambda e: self.on_param_change(),
        )
        amp_slider.grid(row=3, column=0, pady=5)

        # Phase Slider
        ttk.Label(self, text="Phase").grid(row=4, column=0, sticky="w")
        phase_slider = ttk.Scale(
            self,
            from_=0.0,
            to=6.28,
            variable=self.phase,
            orient="horizontal",
            command=lambda e: self.on_param_change(),
        )
        phase_slider.grid(row=5, column=0, pady=5)

        # Frequency Slider
        ttk.Label(self, text="Frequency").grid(row=6, column=0, sticky="w")
        frequency_slider = ttk.Scale(
            self,
            from_=1.0,
            to=5000.0,
            variable=self.frequency,
            orient="horizontal",
            command=lambda e: self.on_param_change(),
        )
        frequency_slider.grid(row=7, column=0, pady=5)

        # Buttons
        ttk.Button(self, text="Play", command=self.start_sending).grid(
            row=8, column=0, pady=5
        )
        ttk.Button(self, text="Stop", command=self.stop_sending).grid(row=9, column=0)

    def on_param_change(self):
        if self.state.running:
            self.send_params()

    def send_params(self):
        message = f"{0};{WaveformType[self.waveform_type.get()].value};{self.frequency.get()};{self.amplitude.get()};{self.phase.get()}"
        self.sender.send(message)

    def start_sending(self):
        if not self.state.running:
            self.state.running = True
            self.send_params()

    def stop_sending(self):
        self.state.running = False
        message = f"{1};{WaveformType[self.waveform_type.get()].value};{self.frequency.get()};{self.amplitude.get()};{self.phase.get()}"
        self.sender.send(message)

# ======= PLOT WIDGET =======

from collections import deque

import tkinter as tk
from tkinter import ttk
import socket
import struct
import threading
import queue
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.animation import FuncAnimation

class WaveformPlot(ttk.Frame):
    def __init__(self, master, recv_port, state, **kwargs):
        super().__init__(master, **kwargs)
        self.recv_port = recv_port
        self.state = state

        self.data_queue = queue.Queue(maxsize=128)
        self.build_plot()

        threading.Thread(target=self.listen_udp, daemon=True).start()

        self.ani = FuncAnimation(self.fig, self.animate_plot, interval=30, cache_frame_data=False)

    def build_plot(self):
        self.fig, self.ax = plt.subplots(figsize=(5, 2), facecolor="black")
        self.ax.set_facecolor("black")
        (self.line,) = self.ax.plot([], [], color="lawngreen")
        self.ax.set_ylim(-1.2, 1.2)
        self.ax.set_xlim(0, 128)
        self.ax.get_xaxis().set_visible(False)
        self.ax.get_yaxis().set_visible(False)

        self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

    def listen_udp(self):
        recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        recv_sock.bind(("", self.recv_port))
        while True:
            data, _ = recv_sock.recvfrom(65535)
            if self.state.running:
                floats = struct.unpack(f"{len(data)//4}f", data)
                for value in floats:
                    # Maintain a fixed size queue
                    if self.data_queue.full():
                        self.data_queue.get()
                    self.data_queue.put(value)
            else:
                time.sleep(0.05)

    def animate_plot(self, frame):
        if self.data_queue.empty():
            return

        data = list(self.data_queue.queue)

        self.line.set_data(np.arange(len(data)), data)
        self.ax.set_xlim(0, 128)
        self.ax.figure.canvas.draw_idle()

# ======= MAIN APPLICATION =======

class WaveformApp(tk.Tk):
    def __init__(self, send_address, recv_port):
        super().__init__()
        self.title("Waveform Generator")
        self.configure(bg="#444")

        self.state = WaveformState()

        sender = WaveformSender(send_address)

        controls = WaveformControls(self, sender, self.state)
        controls.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=10)

        plot = WaveformPlot(self, recv_port, self.state)
        plot.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

# ======= ENTRY POINT =======

def main():
    send_address = ("127.0.0.1", 8081)
    recv_port = 8080
    app = WaveformApp(send_address, recv_port)
    app.mainloop()

if __name__ == "__main__":
    main()
