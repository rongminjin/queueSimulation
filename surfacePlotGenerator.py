'''
======================
3D surface (color map)
======================

Demonstrates plotting a 3D surface colored with the coolwarm color map.
The surface is made opaque by using antialiased=False.

Also demonstrates using the LinearLocator and custom formatting for the
z axis tick labels.
'''

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np
import pandas as pd

######################################### BANK ########################################

fig = plt.figure()
ax = fig.gca(projection='3d')

# Get data
path = "bankData.txt"
arrivalRates = pd.read_csv(path, sep = " ")

# Plot the surface.
surf = ax.plot_trisurf(arrivalRates.CustomerArrivalRate, arrivalRates.MaxCustomerServiceTime, (arrivalRates.AverageWaitPerPerson/60), cmap=cm.BuPu_r,
                       linewidth=0, antialiased=False)

# Create axis labels
ax.set_title("Bank Simulation")
ax.set_xlabel("Arrival Rate (customers/min)")
ax.set_ylabel("Max Service Time (min)")
ax.set_zlabel("90th Percentile Wait (min)")

# Customize the axes.
ax.set_xlim(1, 10)
ax.set_ylim(1, 10)
ax.set_zlim(0, 12)
ax.zaxis.set_major_locator(LinearLocator(10))
ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

# Add a color bar which maps values to colors.
fig.colorbar(surf, shrink=0.5, aspect=5)

######################################### SUPERMARKET ########################################

fig = plt.figure()
ax = fig.gca(projection='3d')

# Get data
path = "supermarketData.txt"
arrivalRates = pd.read_csv(path, sep = " ")

# Plot the surface.
surf = ax.plot_trisurf(arrivalRates.CustomerArrivalRate, arrivalRates.MaxCustomerServiceTime, (arrivalRates.AverageWaitPerPerson/60), cmap=cm.BuPu_r,
                       linewidth=0, antialiased=False)

# Create axis labels
ax.set_title("Supermarket Simulation")
ax.set_xlabel("Arrival Rate (customers/min)")
ax.set_ylabel("Max Service Time (min)")
ax.set_zlabel("90th Percentile Wait (min)")

# Customize the axes.
ax.set_xlim(1, 10)
ax.set_ylim(1, 10)
ax.set_zlim(0, 12)
ax.zaxis.set_major_locator(LinearLocator(10))
ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

# Add a color bar which maps values to colors.
fig.colorbar(surf, shrink=0.5, aspect=5)


plt.show()
