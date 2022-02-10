#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb  9 01:51:11 2022

@author: andres
"""

import serial
import os
import pandas as pd
import numpy as np

serialArduino = serial.Serial("/dev/ttyACM0",9600)
data = np.array

while True:
    cad = serialArduino.readline().decode('ascii')
    print(cad)

    