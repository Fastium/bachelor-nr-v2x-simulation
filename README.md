# Bachelor ns-3 simulator
The goal of this simulator is to measure latency and throughput on a precise use case for a NR V2X mesh network on 
into-train communication (between consist). The C++ simulator is based on ns-3 with the 5GLena extension which contains the V2X application.

# Prerequisites
I recommend to use a Linux distribution to run the simulator. The simulator is tested on Ubuntu 20.04 LTS.

## NS-3
| Prerequisite ns-3 | Version |
|-------------------|---------|
| C++ compiler      | clang++ or g++ (g++ version 9 or greater)|
| Python            | 3.6 or greater |
| CMake             | 3.13 or greater |
| Build system      | make, ninja, xcodebuild(XCode)|

All the prerequisites are available on the official website of ns-3 : [ns-3 getting started](https://www.nsnam.org/docs/release/3.40/tutorial/html/getting-started.html)

## 5GLena
For the usages, you need some libraries and tools to build the 5GLena module.

Install libc6-dev (it provides semaphore.h header file).  The Ubuntu
package name is:

``` terminal
sudo apt-get install libc6-dev
```

Install sqlite:
``` terminal
apt-get install sqlite sqlite3 libsqlite3-dev
```

Notice that ns-3 and nr prerequisites are required (otherwise you will get an error, e.g: fatal error: ns3/sqlite-output.h).


All the prerequisites are available on the official website of 5GLena : [5GLena GitLab](https://gitlab.com/cttc-lena/nr/)

# Usages

To start, I recommend to read the [5GLena Gitlab](https://gitlab.com/cttc-lena/nr/-/tree/5g-lena-v2x-v0.3.y?ref_type=heads) and the [ns-3 documentation](https://www.nsnam.org/docs/release/3.40/tutorial/html/index.html).

## By terminal
To run these commands, you need to be in the simulator folder.

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

To run the simulation (It runs one time the simulation. For more you need the python script):
```terminal 
./ns3 run scratch/simulator/main.cc
```

## Clion

All informations for Clion are avaibles on the ns-3 documentation : [ns-3 Clion](https://www.nsnam.org/docs/release/3.40/tutorial/html/getting-started.html#jetbrains-clion)

In the jetbrains project there is some build profiles to start simulation with python scripts which are in the 
[simulator_script](simulator_script) folder. These python's scripts are used to start the simulations with some flexible 
parameters. They allow to run the simulation with different parameters without recompiling the simulator. They are running the simulator
with the command line. All the measurements are saved in .db files to transmit them from the CPP simulator to the python script.

The CPP simulator main file is [main.cc](scratch/simulator/main.cc).

## Versions
| Name | Version |
|------|---------|
| NS-3 | 3.40    |
|  5GLena    |    v2x 0.3     |

