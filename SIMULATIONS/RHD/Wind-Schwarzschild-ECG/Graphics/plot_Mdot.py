import numpy as np
import matplotlib.pyplot as plt
import sys


directory = "."
if len(sys.argv) > 2:
    directory = sys.argv[2]

dataMdot = np.loadtxt( directory + "/Acc_vs_Time.dat", skiprows=1 )
dataMdotAverage = np.loadtxt( directory + "/Average_vs_Time.dat" , skiprows=1 )

rho0 = 10**(-10)
if len(sys.argv) > 1:
    v0 = float( sys.argv[1] )
else:
    print("Please, provide the value of the velocity v_infinity.")
    sys.exit()

# Plot Mdot / MdotBHL
MdotBHL = 4 * np.pi * rho0 / (v0**3)

print("Number of data points: {}".format( len(dataMdot) ))
print("Last values (at t = {}):".format(dataMdot[-1,0]))
print("\t{:25s}{}".format("Mdot / Mdot_BHL", dataMdot[-1,1] / MdotBHL))
print("\t{:25s}{}".format("MdotAvg / Mdot_BHL", dataMdotAverage[-1,1] / MdotBHL))

fig1 = plt.figure()
plt.plot( dataMdot[:, 0], dataMdot[:, 1] / MdotBHL, label="Mdot_Acc" )
plt.plot( dataMdotAverage[:, 0], dataMdotAverage[:, 1] / MdotBHL, label="Mdot_Average" )
plt.xlabel( "time" )
plt.ylabel( "Mdot/Mdot_BHL" )
plt.legend(loc = "best")
# plt.show()


# Plot Mdot / Mdot0
Mdot0 = 4 * np.pi * rho0

print("\t{:25s}{}".format("log(Mdot / Mdot_0)", np.log10( dataMdot[-1,1] / Mdot0 ) ) )
print("\t{:25s}{}".format("log(MdotAvg / Mdot_0)", np.log10( dataMdotAverage[-1,1] / Mdot0 ) ) )

# fig2 = plt.figure()
# plt.plot( dataMdot[:, 0], np.log10( dataMdot[:, 1] / Mdot0 ), label="Mdot_Acc" )
# plt.plot( dataMdotAverage[:, 0], np.log10( dataMdotAverage[:, 1] / Mdot0 ), label="Mdot_Average" )
# plt.xlabel( "time" )
# plt.ylabel( "log(Mdot/Mdot_0)" )
# plt.legend(loc = "best")


plt.show()
