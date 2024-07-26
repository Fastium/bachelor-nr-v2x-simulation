import os
import parameters

output_dir = "results/outputs/"
simulator_dir = "scratch/simulator/main.cc"
test_simulator_dir = "hello-simulator"

clean_command = "./ns3 clean"
configure_command = "./ns3 configure --disable-tests"
build_command = "./ns3 build"
run_command = "./ns3 run " + simulator_dir


def configure_simulator():
    os.system(clean_command)
    os.system(configure_command)
    os.system(build_command)


def run_simulator(arguments):
    os.system(run_command + " -- " + arguments.get_all())
