import socket
import struct
import csv
import numpy as np

IP_ADDRESS = "192.168.1.52"
PORT = 1234 

# Create a socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.settimeout(20)

# Connect to the server
client_socket.connect((IP_ADDRESS, PORT))

# Receive data
data = []
while True:
    # Unpack timestamp and sensor sample 
    received_data = client_socket.recv(10)
    value = struct.unpack("<hQ", received_data)
    # We are hard scaling the raw sensor value here by its dividing factor according to the datasheet <sample>/1200
    print(str(value[1]) + "," + str(round(value[0]/1200, 6)))

# Close the socket
client_socket.close()
