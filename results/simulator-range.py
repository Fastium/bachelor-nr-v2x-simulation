from collections import OrderedDict
from datetime import datetime, timedelta
import sqlite3
from pyexcel_ods3 import save_data

import numpy as np
from matplotlib import pyplot as plt

from utils import configure_simulator, run_simulator, write
from parameters import Parameters
from tableStructure import TracePackets

data = OrderedDict()


date = datetime.now()
write("##################################################################################################################################################")
write("##################################################################################################################################################")
write("##################################################################################################################################################")
write("                                                      Date : " + date.strftime("%d/%m/%Y %H:%M:%S"))


# configure
configure_simulator()
arguments = Parameters()

arguments.dataRateBe = 1000000
arguments.udpPacketSizeBe = 28000



t_finalActivationBearers = arguments.bearerActivationTime + arguments.bearerDelay
t_finalSimulation = t_finalActivationBearers + arguments.simulationTime
realAppStart = t_finalActivationBearers + arguments.udpPacketSizeBe * 8.0 / (arguments.dataRateBe * 1000.0)

arguments.numRouters = 0
arguments.ueDistance = 100 # meters

tableNameApplication = "appStats"
tableNameIpLayer = "ipLayerStats"
tableNamePhySpectrum = "phySpectrumStats"

tableNames = [tableNameApplication, tableNameIpLayer, tableNamePhySpectrum]

allDataRate = []

last_range = 0

max_range = 1000 #850
min_range = 10
range = min_range
foot_range = 10

max_power = 40 #30
min_power = 20
power = min_power

limit_range = []
limit_power = []

## run
while (power <= max_power) :
    while (range <= max_range) :
        id_simulation = str(power) + ":" + str(range)
        # set parameters
        arguments.simTag = "test" + id_simulation
        arguments.ueDistance = range
        arguments.txPower = power

        # clear table if it exists
        connection = sqlite3.connect(arguments.simTag + "-output-simulator.db")
        cursor = connection.cursor()
        for tableName in tableNames:
            cursor.execute("DROP TABLE IF EXISTS " + tableName)
        connection.close()

        write("")
        write("--- Simulations " + id_simulation + " start ! ---")

        start = datetime.now()
        write("Start : " + start.strftime("%H::%M::%S"))

        run_simulator(arguments)

        # Connect to the database
        connection = sqlite3.connect(arguments.simTag + "-output-simulator.db")
        cursor = connection.cursor()

        ####################################################################################################################
        # Application layer (troughputs
        write("Application Layer ------------------------")
        cursor.execute("SELECT * FROM " + tableNameApplication)
        rows = cursor.fetchall()
        data_TxRxStats = [TracePackets(*row) for row in rows]

        txPaquets = []
        rxPaquets = []

        for p in data_TxRxStats:
            if(p.txRx == "Tx"):
                txPaquets.append(p)
            else:
                rxPaquets.append(p)

        write("Nbr of Tx Paquets : " + str(len(txPaquets)))
        write("Nbr of Rx Paquets : " + str(len(rxPaquets)))

        byteSentCounter = 0
        byteReceivedCounter = 0

        for p in txPaquets:
            byteSentCounter += p.packetSize
        for p in rxPaquets:
            byteReceivedCounter += p.packetSize

        averageThrouhputs = (byteReceivedCounter * 8) / (t_finalSimulation-realAppStart) / 1000.0
        thereoticalThroughput = (byteSentCounter * 8) / (t_finalSimulation-realAppStart) / 1000.0

        error_max = 0.1 * thereoticalThroughput
        error = abs(averageThrouhputs-thereoticalThroughput)

        if(error > error_max):
            # take the last point
            break

        else :
            # continue with the next range
            range += foot_range

        write("")
        write("--- Simulations " + id_simulation + " end ! ---")
        write("")

        finish = datetime.now()
        write("Stop : " + finish.strftime("%H::%M::%S"))
        duration = finish - start
        write("Duration : " + str(duration))
    ###########################################################

    limit_range.append(range-foot_range)
    limit_power.append(power)
    write("Power : " + str(power) + " dBm")
    write("Range : " + str(range-foot_range) + " meters")
    power += 1
    range = min_range
###############################################################

# make the plot
plt.plot(limit_power, limit_range)
plt.title('Range of the transmission')
plt.xlabel('Power (dBm)')
plt.ylabel('Range (m)')

plt.show()

data.update({"Sheet 1": [limit_power, limit_range]})
save_data("range-1000m-v4.ods", data)



