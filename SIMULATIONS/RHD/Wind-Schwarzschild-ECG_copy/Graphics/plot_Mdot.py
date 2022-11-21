import numpy as np
import matplotlib.pyplot as plt
import sys

directory = "."
if len(sys.argv) > 1:
    directory = sys.argv[1]

dataMdot = np.loadtxt( directory + "/Acc_vs_Time.dat", skiprows=1 )
dataMdotAverage = np.loadtxt( directory + "/Average_vs_Time.dat" , skiprows=1 )

rho0 = 10**(-10)
v0 = 0.5
MdotBHL = 4 * np.pi * rho0 / (v0**3)

print("Number of data points: {}".format( len(dataMdot) ))
print("Last values (at t = {}):".format(dataMdot[-1,0]))
print("\tMdot / MdotBHL\t{}".format(dataMdot[-1,1] / MdotBHL))
print("\tMdotAvg / MdotBHL\t{}".format(dataMdotAverage[-1,1] / MdotBHL))

plt.plot( dataMdot[:, 0], dataMdot[:, 1] / MdotBHL, label="Mdot_Acc" )
plt.plot( dataMdotAverage[:, 0], dataMdotAverage[:, 1] / MdotBHL, label="Mdot_Average" )
plt.xlabel( "time" )
plt.ylabel( "Mdot/Mdot_BHL" )
plt.legend(loc = "best")
plt.show()


