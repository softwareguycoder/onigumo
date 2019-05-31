from data.machine_info import MachineInfo
from inuyasha_symbols import IDS_HOSTNAME_PROMPT, IDS_PORT_PROMPT

class MachineInfoFactory():

        @staticmethod
        def Make():
            szHostName = input(IDS_HOSTNAME_PROMPT)
            print()
            
            nPort = 0
            try:
                nPort = int(input(IDS_PORT_PROMPT))
                print()
            except:
                pass
            
            return MachineInfo(szHostName, nPort)
