from collections import OrderedDict
from datetime import datetime, timedelta
import sqlite3

from pyexcel_ods3 import save_data
from statistics import mean

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
arguments.udpPacketSizeBe = 1000
arguments.txPower = 35 # 280m range --> minimum 3 relay nodes
arguments.enableSensing = True

t_finalActivationBearers = arguments.bearerActivationTime + arguments.bearerDelay
t_finalSimulation = t_finalActivationBearers + arguments.simulationTime
realAppStart = t_finalActivationBearers + arguments.udpPacketSizeBe * 8.0 / (arguments.dataRateBe * 1000.0)


tableNameApplication = "appStats"
tableNameIpLayer = "ipLayerStats"
tableNamePhySpectrum = "phySpectrumStats"

tableNames = [tableNameApplication, tableNameIpLayer, tableNamePhySpectrum]


node_min = 3
node_max = 32 #32
node = node_min

dist_max = 800

limit_latency = []
limit_node = []

file_version = "v3"


## run
while (node <= node_max) :

    id_simulation = str(node)
    # set parameters
    arguments.simTag = "test" + id_simulation
    dist = dist_max/node
    arguments.ueDistance = dist
    arguments.numRouters = node
    write("Simulation " + id_simulation + " : " + str(node) + " relay nodes, distance : " + str(dist) + " meters")

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

    latency = []
    rxTime = 0
    txTime = 0
    for p in rxPaquets:
        for t in txPaquets:
            if p.packetId == t.packetId:
                latency.append(abs(p.timeNanoSec - t.timeNanoSec))
    limit_latency.append(latency)
    limit_node.append(node)
    node +=1

    write("")
    write("--- Simulations " + id_simulation + " end ! ---")
    write("")

    finish = datetime.now()
    write("Stop : " + finish.strftime("%H::%M::%S"))
    duration = finish - start
    write("Duration : " + str(duration))
###########################################################

average_latency = []

for l in limit_latency:
    if len(l) == 0:
        average_latency.append(0.0)
    else:
        average_latency.append(mean(l)/1000000)

# make the plot
plt.plot(limit_node, average_latency)
plt.title('Evolution of latency with the number of relay nodes')
plt.ylabel('Latency (ms)')
plt.xlabel('Nbr of node relay')

plt.show()

data.update({"Sheet 1": [limit_node, average_latency]})
save_data("latency-" + file_version + ".ods", data)



