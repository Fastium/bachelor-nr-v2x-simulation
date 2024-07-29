from utils import configure_simulator, run_simulator
from parameters import Parameters
import sqlite3
from tableStructure import TracePackets

def calculateThroughput(cursor):
    rows = cursor.fetchall()

    # Convert each row and add it to a list
    data_TxRxStats = [TracePackets(*row) for row in rows]

    txPaquets = []
    rxPaquets = []

    for p in data_TxRxStats:
        if(p.txRx == "Tx"):
            txPaquets.append(p)
        else:
            rxPaquets.append(p)

    print("Nbr of Tx Paquets : ", len(txPaquets))
    print("Nbr of Rx Paquets : ", len(rxPaquets))

    byteSentCounter = 0
    byteReceivedCounter = 0

    for p in txPaquets:
        byteSentCounter += p.packetSize
    for p in rxPaquets:
        byteReceivedCounter += p.packetSize

    averageThrouhputs = (byteReceivedCounter * 8) / (t_finalSimulation-realAppStart) / 1000.0

    print("MCS : ", arguments.initialMcs)
    print("Theoretical Throughput : ", arguments.dataRateBe, " kilobits per second")
    print("Average Throughput     : ", averageThrouhputs, " kilobits per second")


# configure
configure_simulator()
arguments = Parameters()

# set parameters
arguments.udpPacketSizeBe = 200 # bytes
arguments.dataRateBe = 1000 # kilobits per second

arguments.simulationTime = 20 # seconds
arguments.serverStartTime = 3 # seconds
arguments.bearerActivationTime = 5 # seconds
arguments.bearerDelay = 0.001 # seconds

arguments.numSidelinkProcess = 1

t_finalActivationBearers = arguments.bearerActivationTime + arguments.bearerDelay
t_finalSimulation = t_finalActivationBearers + arguments.simulationTime
realAppStart = t_finalActivationBearers + arguments.udpPacketSizeBe * 8.0 / (arguments.dataRateBe * 1000.0)

arguments.numRouters = 1
arguments.ueDistance = 50 # meters

tableName = "TracePackets"
# clear table if it exists
connection = sqlite3.connect(arguments.simTag + "-output-simulator.db")
cursor = connection.cursor()
cursor.execute("DROP TABLE IF EXISTS " + tableName)
connection.close()


## run
for i in range(1, 30):
    arguments.simTag = "test" + str(i)
    arguments.initialMcs = i

    run_simulator(arguments)

    # Connect to the database
    connection = sqlite3.connect(arguments.simTag + "-output-simulator.db")
    cursor = connection.cursor()

    # Query the database
    cursor.execute("SELECT * FROM " + tableName)

    calculateThroughput(cursor)

    # Close the connection
    connection.close()






