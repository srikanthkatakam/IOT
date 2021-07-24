from machine import UART

import os

uart = UART(0, baudrate=115200)
os.dupterm(uart)


