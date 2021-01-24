import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from itertools import count
from ctypes import cdll

dat_path = '../data/voltage_drop.dat'
data = np.loadtxt(dat_path, dtype='float')

# approx = []
# R = 1.
# C = 1.
# w0 = 1 / 1000 # 1. / (R * C)
# for t in range(data.size):
#     # x = 12.0 * (1. - np.exp(- w0 * t))
#     print(t)
#     # print(np.exp(- w0 * t))
#     x = data[t] * (1. - np.exp(- w0 * t))
#     approx.append(x)

POINTS_INTERVAL = 1
MAX_DEGREE = 10

xvals = range(0, data.size, POINTS_INTERVAL)

for d in range(2, MAX_DEGREE + 1):
    poly = np.poly1d(np.polyfit(x=xvals, y=data[xvals], deg=d), variable='x')
    approx = np.array([poly(x) for x in xvals])
    err = ((data[xvals] - approx)**2).mean()
    err = (np.abs(data[xvals] - approx)).mean()
    label = 'poly-degree=' + str(d) + ' [MSE = {:.5f}]'.format(err)
    plt.plot(xvals, approx.copy(), label=label)
    print('MSE(degree={}): {:.6f}'.format(d, err))
    print('-' * 80)
    print(poly)
    print('-' * 80)

plt.plot(range(data.size), data, label='data')
plt.legend()
plt.show()

hex_converter = cdll.LoadLibrary('convertnp2hex.so')
hex_converter.printCoefficients(int(6), np.ctypeslib.as_ctypes(poly.c))
