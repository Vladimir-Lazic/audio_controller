import socket
import numpy as np
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt


def receive_and_plot():
    print("Initializing...")
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("localhost", 8080))

    plt.ion()
    fig, ax = plt.subplots()
    (line,) = ax.plot([], [])
    ax.set_ylim(-1.2, 1.2)

    MAX_SIZE = 8192

    while True:
        data, _ = sock.recvfrom(MAX_SIZE)
        waveform = np.frombuffer(data, dtype=np.float32)

        line.set_xdata(np.arange(len(waveform)))
        line.set_ydata(waveform)
        ax.relim()
        ax.autoscale_view()

        plt.pause(0.01)


if __name__ == "__main__":
    receive_and_plot()
