
class Parameters :
    udpPacketSizeBe = 28000
    dataRateBe = 1000000
    simulationTime = 10
    numerologyBwpSl = 1
    txPower = 23
    phyNoise = 5
    phyLatency = 100
    centralFrequencyBandSl = 5.89e9
    bandwidthBandSl = 400
    antennaNumRows = 1
    antennaNumColumns = 2
    enableSensing = False
    t1 = 1
    t2 = 5
    activePoolId = 0
    reservationPeriod = 50
    numSidelinkProcess = 4
    numRouters = 0
    ueDistance = 100
    errorModel = "ns3::NrEesmIrT2" #"ns3::NrEesmCcT1"
    scenario = "v2v_Highway"
    sidelinkDelay = 0
    simTag = "test"
    serverStartTime = 3
    bearerActivationTime = 5
    bearerDelay = 0.01
    initialMcs = 27

    def get_all(self):
        return (" --udpPacketSizeBe=" + str(self.udpPacketSizeBe) +
                " --dataRateBe=" + str(self.dataRateBe) +
                " --simulationTime=" + str(self.simulationTime) +
                " --numerologyBwpSl=" + str(self.numerologyBwpSl) +
                " --txPower=" + str(self.txPower) +
                " --phyNoise=" + str(self.phyNoise) +
                " --phyLatency=" + str(self.phyLatency) +
                " --centralFrequencyBandSl=" + str(self.centralFrequencyBandSl) +
                " --bandwidthBandSl=" + str(self.bandwidthBandSl) +
                " --antennaNumRows=" + str(self.antennaNumRows) +
                " --antennaNumColumns=" + str(self.antennaNumColumns) +
                " --enableSensing=" + str(self.enableSensing) +
                " --t1=" + str(self.t1) +
                " --t2=" + str(self.t2) +
                " --activePoolId=" + str(self.activePoolId) +
                " --reservationPeriod=" + str(self.reservationPeriod) +
                " --numSidelinkProcess=" + str(self.numSidelinkProcess) +
                " --numRouters=" + str(self.numRouters) +
                " --ueDistance=" + str(self.ueDistance) +
                " --errorModel=" + str(self.errorModel) +
                " --scenario=" + str(self.scenario) +
                " --sidelinkDelay=" + str(self.sidelinkDelay) +
                " --simTag=" + str(self.simTag) +
                " --serverStartTime=" + str(self.serverStartTime) +
                " --bearerActivationTime=" + str(self.bearerActivationTime) +
                " --bearerDelay=" + str(self.bearerDelay) +
                " --initialMcs=" + str(self.initialMcs)
                )


