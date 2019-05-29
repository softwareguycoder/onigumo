from data.machine_info import MachineInfo
from inuyasha_symbols import *

class MachineInfoFactory():

        @staticmethod
        def make():
            print(APP_TITLE)
            print(APP_SUBTITLE)
            print()
            print(IDM_CONNECT_REMOTE_MACHINE)
            print()
            szHostName = input(IDS_HOSTNAME_PROMPT)
            print()
            nPort = int(input(IDS_PORT_PROMPT))
            print()
            return MachineInfo(szHostName, nPort)
