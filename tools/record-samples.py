import socket
import struct
import csv

IP_ADDRESS = "192.168.1.29"
PORT = 1234 

# Create a socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.settimeout(20)

# Connect to the server
client_socket.connect((IP_ADDRESS, PORT))

# Receive data
data = []
while True:
    # Unpack the data as uint16_t
    received_data = client_socket.recv(2)
    value = struct.unpack("<h", received_data)[0]
    print(str(value)) 

# Close the socket
client_socket.close()
