from utils import configure_simulator, run_simulator
from parameters import Parameters


def main():
    configure_simulator()

    arguments = Parameters()

    run_simulator(arguments)



###############################################################################
main()

