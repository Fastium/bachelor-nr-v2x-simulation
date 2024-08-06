from datetime import datetime, timedelta
import sqlite3

from utils import configure_simulator, run_simulator, write
from parameters import Parameters
from tableStructure import TracePackets


date = datetime.now()
write("##################################################################################################################################################")
write("##################################################################################################################################################")
write("##################################################################################################################################################")
write("                                                      Date : " + date.strftime("%d/%m/%Y %H:%M:%S"))


# configure
configure_simulator()
arguments = Parameters()

# set parameters
arguments.udpPacketSizeBe = 1000 # bytes
arguments.dataRateBe = 50000 # bits per second
arguments.phyLatency = 100000 # ns

arguments.initialMcs = 28

arguments.simulationTime = 10 # seconds
arguments.serverStartTime = 3 # seconds
arguments.bearerActivationTime = 5 # seconds
arguments.bearerDelay = 0.01 # seconds

arguments.numSidelinkProcess = 4

t_finalActivationBearers = arguments.bearerActivationTime + arguments.bearerDelay
t_finalSimulation = t_finalActivationBearers + arguments.simulationTime
realAppStart = t_finalActivationBearers + arguments.udpPacketSizeBe * 8.0 / (arguments.dataRateBe * 1000.0)

arguments.numRouters = 1
arguments.ueDistance = 100 # meters

tableNameApplication = "appStats"
tableNameIpLayer = "ipLayerStats"
tableNamePhySpectrum = "phySpectrumStats"

tableNames = [tableNameApplication, tableNameIpLayer, tableNamePhySpectrum]

allDataRate = []

## run
def run(i, j, k):
    id_simulation = str(i) + ":" + str(j) + ":" + str(k)
    # set parameters
    arguments.simTag = "test" + id_simulation
    # arguments.initialMcs = i
    # arguments.dataRateBe = pow(10, j) # kilobits per second
    # arguments.udpPacketSizeBe = pow(2, k) # bits
    # clear table if it exists
    connection = sqlite3.connect(arguments.simTag + "-output-simulator.db")
    cursor = connection.cursor()
    for tableName in tableNames:
        cursor.execute("DROP TABLE IF EXISTS " + tableName)
    connection.close()

    write("")
    write("--- Simulations " + id_simulation + " start ! ---")


    write("")
    write("Initial MCS : " + str(i))
    write("Data Rate : " + str(arguments.dataRateBe) + " bits per second")
    write("UDP Packet Size : " + str(arguments.udpPacketSizeBe) + " bits")
    write("")

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

    allDataRate.append(averageThrouhputs)

    write("Theoretical Throughput : " + str(thereoticalThroughput) + " kilobits per second")
    write("Average Throughput     : " + str(averageThrouhputs) + " kilobits per second")

    ####################################################################################################################
    # IP Layer
    # print("IP Layer ----------------------------")
    # cursor.execute("SELECT * FROM " + tableNameIpLayer)
    # rows = cursor.fetchall()
    # data_TxRxStats = [TracePackets(*row) for row in rows]
    #
    # rxIpLayer = data_TxRxStats
    #
    # print("Nbr of Rx Paquets : ", len(rxIpLayer))
    #
    # for p in rxIpLayer:
    #     exist = False
    #     for pRxApp in rxPaquets:
    #         if (p.packetId == pRxApp.packetId):
    #             print("Packet Received : ", str(p.packetId))
    #             exist = True
    #             break
    #
    #     if (exist == False):
    #         print("Packet Lost : ", str(p.packetId) + "--------------------------------------------------------------------------------------------------")

    ####################################################################################################################
    # PHY SPECTRUM
    print("PHY Spectrum ----------------------------")
    cursor.execute("SELECT * FROM " + tableNamePhySpectrum)
    rows = cursor.fetchall()
    data_TxRxStats = [TracePackets(*row) for row in rows]

    txPhySpectrum = data_TxRxStats

    for pTxApp in txPaquets:
        exist = False
        for pTxPhy in txPhySpectrum:
            if (pTxApp.packetId == pTxPhy.packetId):
                print("Packet Sent : ", str(pTxApp.packetId), " app size : ", pTxApp.packetSize, " spectrum size : ", pTxPhy.packetSize)
                exist = True
        if (exist == False):
            print("Packet Lost : ", str(pTxApp.packetId), " app size : ", pTxApp.packetSize, " spectrum size : ", pTxPhy.packetSize)

    # Close the connection
    connection.close()

    write("")
    write("--- Simulations " + id_simulation + " end ! ---")
    write("")

    finsih = datetime.now()
    write("Stop : " + finsih.strftime("%H::%M::%S"))
    duration = finsih - start
    write("Duration : " + str(duration))

######################################################################################################################################
# MAIN
maxMcs = 29
maxDataRate = 7
maxUdpPacketSize = 15

for i in range(0, 1):
    for j in range(0, 1):
        for k in range(0, 1):
            run(i, j, k)

allDataRate.sort(reverse=True)
write("")
write("Best Throughput : " + str(allDataRate[0]) + " kilobits per second")

x = 0
for i in allDataRate:
    write(str(x) + "--" + str(i))
    x+=1