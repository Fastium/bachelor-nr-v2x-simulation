# Bachelor ns-3 simulator
The goal of this simulator is to measure latency and throughput on a precise use case for a NR V2X mesh network on 
into-train communication (between consist).

## Build the simulator
With examples :
```terminal
./ns3 clean
./ns3 configure --build-profile=debug --enable-examples --enable-tests
./ns3 build
```
Without examples :
```terminal
./ns3 clean
./ns3 configure --build-profile=debug --enable-tests
./ns3 build
```
Optimized build profile is for real simulation :
```terminal
./ns3 clean
./ns3 configure --build-profile=optimized --enable-tests
./ns3 build
```
It is possible to build in another build profile like optimized for real simulation **(to see in the document of ns-3 in 
right version)**.
## Control the status
```terminal
./ns3 show profile
```

Answer :

```terminal
Build profile: debug
```

## Simulator on scratch

### Directory

Each simulator.cc with some other .cc and .h have to be in the same subdirectory for working.

#### Analyse example
### NrPointToPointEpcHelper
1. set attribute "S1uLinkDelay" to 0ms
2. assign IP address with the net device container

### NrHelper
1. set the epcHelper
2. set the attributes (channel, bands, antenna, PHY, MAC, bandwidth part manager)
3. install and get the net device container
4. assign its stream

### NrSlHelper
1. set the epcHelper
2. set the attributes
3. prepare the ues for sidelink with the net device container and the bwp id container
4. install the preconfig for net device container
5. assign its stream
6. activate the nr sidelink bearer

### NR V2X stack

Parameter for me :
- size of a packet
- number of packets sent
- simulation time between the soft has built the environment and the last packet received
- distance between the two consist
- number of consist (mesh network)
- MCS I want to use (disable the automatic mcs)
- the channel model
- centralFrequencyBandSl
- bandwidth of the sidelink
- power 
- 
### Measure and save latency and throughput

Regarder la class dataCollector dans le fichier dataCollector.h et dataCollector.cc

### Channel model

…

### Mesh network

…

## Input parameters

- size of the data in the packet
- throughput
- error model
- central frequency
- 

## Versions

Version : ns-3.40 \
module : 5GLena v2x 0.3

## Installation

Get ns-3 env (v3.40) : https://www.nsnam.org/releases/ns-3-40/download/ \
Extract the tar.bz2 in the directory you want 

```terminal
tar -xvf ns-allinone-3.40.tar.bz2
```

Move inside the directory 

```terminal
cd ./ns-allinone-3.40
```

Get 5GLena with v2x extension : https://gitlab.com/cttc-lena/nr/-/blob/5g-lena-v2x-v0.3.y/README.md?ref_type=heads

Install the ns-3 with V2X extension

```terminal
git clone https://gitlab.com/cttc-lena/ns-3-dev.git
cd ns-3-dev
```

Download the nr module

```terminal

cd contrib
git clone https://gitlab.com/cttc-lena/nr.git
```

Switch to the latest NR release branch with V2X extensions

```terminal

cd nr
git checkout 5g-lena-v2x-v0.3.y
```

```terminal

cd ../..
git checkout -b v2x-lte-dev origin/v2x-lte-dev
```

Make a detached head

```terminal
git checkout ns-3-dev-v2x-v0.3
```

Configure and build the simulator

```terminal
./ns3 configure --enable-tests --enable-examples
```

```terminal
./ns3 build
```

```terminal
./ns3 run "cttc-nr-v2x-demo-simple"
```

Make all in one :

```terminal
git clone https://gitlab.com/cttc-lena/ns-3-dev.git
cd ./ns-3-dev/contrib
git clone https://gitlab.com/cttc-lena/nr.git
cd ./nr
git checkout 5g-lena-v2x-v0.3.y
cd ../..
git checkout -b v2x-lte-dev origin/v2x-lte-dev
git checkout -b simulator ns-3-dev-v2x-v0.3
```

```terminal

```
