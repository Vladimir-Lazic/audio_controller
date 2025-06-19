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


# ======= COMMUNICATION / SENDER LOGIC =======

class WaveformSender:
    def __init__(self, send_address):
        self.send_address = send_address
        self.send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def send(self, message: str):
        self.send_sock.sendto(message.encode("utf-8"), self.send_address)


# ======= WIDGET BUILDERS / MODULAR UI COMPONENTS =======

class WaveformControls(ttk.Frame):
    def __init__(self, master, sender: WaveformSender, **kwargs):
        super().__init__(master, **kwargs)
        self.sender = sender
        self.build_controls()

    def build_controls(self):
        self.waveform_type = tk.StringVar(value="Sine")
        self.amplitude = tk.DoubleVar(value=1.0)
        self.frequency = tk.DoubleVar(value=440.0)
        self.phase = tk.DoubleVar(value=0.0)
        self.sample_rate = tk.IntVar(value=44100)
        self.running = False

        # Waveform selector
        ttk.Label(self, text="Waveform Type").grid(row=0, column=0, sticky="w")
        waveform_menu = ttk.OptionMenu(
            self, self.waveform_type, "Sine", *[w.name for w in WaveformType]
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
            command=lambda e: self.on_param_change()
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
            command=lambda e: self.on_param_change()
        )
        phase_slider.grid(row=5, column=0, pady=5)

        # Buttons
        ttk.Button(self, text="Play", command=self.start_sending).grid(row=6, column=0, pady=5)
        ttk.Button(self, text="Stop", command=self.stop_sending).grid(row=7, column=0)

    def on_param_change(self):
        if self.running:
            self.send_params()

    def send_params(self):
        while self.running:
            message = f"{WaveformType[self.waveform_type.get()].value};{self.frequency.get()};{44100};{self.amplitude.get()};{self.phase.get()}"
            print("Sending:", message)
            self.sender.send(message)

    def start_sending(self):
        if not self.running:
            self.running = True
            self.send_params()

    def stop_sending(self):
        self.running = False


# ======= PLOT WIDGET =======

class WaveformPlot(ttk.Frame):
    def __init__(self, master, recv_port, **kwargs):
        super().__init__(master, **kwargs)
        self.recv_port = recv_port
        self.recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.recv_sock.bind(("", self.recv_port))
        self.build_plot()

        threading.Thread(target=self.listen_udp, daemon=True).start()

    def build_plot(self):
        fig, self.ax = plt.subplots(figsize=(5, 2), facecolor="black")
        self.ax.set_facecolor("black")
        self.line, = self.ax.plot([], [], color="lawngreen")
        self.ax.set_ylim(-1.2, 1.2)
        self.ax.set_xlim(0, 1024)
        self.ax.get_xaxis().set_visible(False)
        self.ax.get_yaxis().set_visible(False)

        self.canvas = FigureCanvasTkAgg(fig, master=self)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

    def listen_udp(self):
        while True:
            data, _ = self.recv_sock.recvfrom(8192)
            floats = struct.unpack(f"{len(data)//4}f", data)
            self.update_plot(floats)

    def update_plot(self, data):
        self.line.set_ydata(data)
        self.line.set_xdata(np.arange(len(data)))
        self.ax.set_xlim(0, len(data))
        self.ax.figure.canvas.draw_idle()


# ======= MAIN APPLICATION =======

class WaveformApp(tk.Tk):
    def __init__(self, send_address, recv_port):
        super().__init__()
        self.title("Retro Waveform Generator")
        self.configure(bg="#444")

        sender = WaveformSender(send_address)

        controls = WaveformControls(self, sender)
        controls.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=10)

        plot = WaveformPlot(self, recv_port)
        plot.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)


# ======= ENTRY POINT =======

def main():
    send_address = ("127.0.0.1", 8081)
    recv_port = 8080
    app = WaveformApp(send_address, recv_port)
    app.mainloop()

if __name__ == "__main__":
    main()
