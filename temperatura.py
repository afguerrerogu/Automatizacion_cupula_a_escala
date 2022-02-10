#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb  9 23:30:19 2022

@author: andres
"""

from cProfile import label
from calendar import c
import random
from  itertools import count
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np


plt.style.use('fivethirtyeight')

x_val = []
y_val = []

index = count()

def animate(i):
    data = pd.read_csv('datos.txt',skiprows=20, names=["tiempo", "humedad", "temperatura", "velocidad", "luminosidad"] )
    tiempo = data["tiempo"]
    humedad = data["humedad"]
    temperature = data["temperatura"]
    vel = data["velocidad"]
    luminosidad = data["luminosidad"]



    plt.cla()
    plt.plot(tiempo, temperature,c="black", lw=1.5)
    plt.xlabel("Tiempo[S]")
    plt.ylabel("Temperatura °C")


ani = FuncAnimation(plt.gcf(),animate,interval=1000)

plt.tight_layout()
plt.show()