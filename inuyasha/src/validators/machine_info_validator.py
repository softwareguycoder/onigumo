from common.string_utils import StringUtils
from inuyasha_symbols import *  # @UnusedWildImport
from validators.port_number_validator import PortNumberValidator

class MachineInfoValidator:
    
    @staticmethod
    def IsValid(machineInfo, show_errors=False):
        result = False
        if (machineInfo is None):
            return result
        
        if StringUtils.IsNullOrWhiteSpace(machineInfo.szHostName):
            if (show_errors):
                print(ERROR_HOST_NAME_REQUIRED)
            return result
        
        if not PortNumberValidator.isPortNumberValid(machineInfo.nPort):
            if (show_errors):
                print(ERROR_PORT_NUMBER_OUT_OF_RANGE)
            return result
        
        return True