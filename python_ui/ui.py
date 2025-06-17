import tkinter as tk
from tkinter import ttk
import socket
import threading
import struct
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import json
from enum import Enum


class WaveformType(Enum):
    Sine = 0
    Sawtooth = 1
    Triangle = 2
    Square = 3
    WhiteNoise = 4


class WaveformUI:
    def __init__(self, master, send_address, recv_port):
        self.master = master
        self.send_address = send_address
        self.recv_port = recv_port

        self.send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        self.recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.recv_sock.bind(("", self.recv_port))

        self.waveform_type = tk.StringVar(value="Sine")
        self.amplitude = tk.DoubleVar(value=1.0)
        self.frequency = tk.DoubleVar(value=440.0)
        self.phase = tk.DoubleVar(value=0.0)
        self.sample_rate = tk.IntVar(value=44100)

        self.build_controls()
        self.build_plot()

        threading.Thread(target=self.listen_udp, daemon=True).start()

    def build_controls(self):
        control_frame = ttk.Frame(self.master)
        control_frame.pack(side=tk.LEFT, fill=tk.Y)

        ttk.Label(control_frame, text="Waveform Type:").grid(row=0, column=0)
        waveform_menu = ttk.OptionMenu(
            control_frame, self.waveform_type, "Sine", *[w.name for w in WaveformType]
        )
        waveform_menu.grid(row=1, column=0)

        ttk.Label(control_frame, text="Amplitude (0.0 - 1.0)").grid(row=2, column=0)
        amp_slider = ttk.Scale(
            control_frame,
            from_=0.0,
            to=1.0,
            variable=self.amplitude,
            orient="horizontal",
        )
        amp_slider.grid(row=3, column=0)

        ttk.Label(control_frame, text="Frequency (Hz)").grid(row=4, column=0)
        freq_entry = ttk.Entry(control_frame, textvariable=self.frequency)
        freq_entry.grid(row=5, column=0)

        ttk.Label(control_frame, text="Phase (0 - 6.28)").grid(row=6, column=0)
        phase_slider = ttk.Scale(
            control_frame, from_=0.0, to=6.28, variable=self.phase, orient="horizontal"
        )
        phase_slider.grid(row=7, column=0)

        ttk.Label(control_frame, text="Sample Rate").grid(row=8, column=0)
        sr_entry = ttk.Entry(control_frame, textvariable=self.sample_rate)
        sr_entry.grid(row=9, column=0)

        send_button = ttk.Button(
            control_frame, text="Send Params", command=self.send_params
        )
        send_button.grid(row=10, column=0)

    def build_plot(self):
        fig, self.ax = plt.subplots(figsize=(6, 3))
        (self.line,) = self.ax.plot([], [])
        self.ax.set_ylim(-1.2, 1.2)
        self.ax.set_xlim(0, 1024)

        canvas = FigureCanvasTkAgg(fig, master=self.master)
        canvas.get_tk_widget().pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        self.canvas = canvas

    def send_params(self):
        params = {
            "waveform_type" : WaveformType[self.waveform_type.get()].value,            
            "frequency": self.frequency.get(),            
            "sample_rate": self.sample_rate.get(),
            "amplitude": self.amplitude.get(),
            "phase": self.phase.get(),
        }
        message = f"{params['waveform_type']};{params['frequency']};{params['sample_rate']};{params['amplitude']};{params['phase']}"
        print(message)
        self.send_sock.sendto(message.encode('utf-8'), self.send_address)

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


def main():
    root = tk.Tk()
    root.title("Waveform Controller")

    send_address = ("127.0.0.1", 8081)
    recv_port = 8080

    app = WaveformUI(root, send_address, recv_port)
    root.mainloop()


if __name__ == "__main__":
    main()
