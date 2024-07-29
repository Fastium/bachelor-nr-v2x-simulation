class TxRxStats:
    def __init__(self, timeSec, txRx, nodeId, imsi, pktSizeBytes, srcIp, srcPort, dstIp, dstPort, pktSeqNum, SEED, RUN):
        self.timeSec = timeSec
        self.txRx = txRx
        self.nodeId = nodeId
        self.imsi = imsi
        self.pktSizeBytes = pktSizeBytes
        self.srcIp = srcIp
        self.srcPort = srcPort
        self.dstIp = dstIp
        self.dstPort = dstPort
        self.pktSeqNum = pktSeqNum
        self.SEED = SEED
        self.RUN = RUN

class pscchRxUePhy:
    def __init__(self, timeMs, cellId, rnti, bwpId, fram, subFrame, slot, txRnti, dstL2Id, pscchRbLen, pscchMcs, avrgSinr, minSinr, bler, corrupt, psschStartSbCh, psschLenSbCh, maxNumPerReserve, SEED, RUN):
        self.timeMs = timeMs
        self.cellId = cellId
        self.rnti = rnti
        self.bwpId = bwpId
        self.fram = fram
        self.subFrame = subFrame
        self.slot = slot
        self.txRnti = txRnti
        self.dstL2Id = dstL2Id
        self.pscchRbLen = pscchRbLen
        self.pscchMcs = pscchMcs
        self.avrgSinr = avrgSinr
        self.minSinr = minSinr
        self.bler = bler
        self.corrupt = corrupt
        self.psschStartSbCh = psschStartSbCh
        self.psschLenSbCh = psschLenSbCh
        self.maxNumPerReserve = maxNumPerReserve
        self.SEED = SEED
        self.RUN = RUN

class TracePackets:
    def __init__(self, txRx, timeNanoSec, packetId, packetSize):
        self.txRx = str(txRx)
        self.timeNanoSec = float(timeNanoSec)
        self.packetId = int(packetId)
        self.packetSize = int(packetSize)
