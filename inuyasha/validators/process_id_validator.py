from common.inuyasha_symbols import INVALID_PID_VALUE, ERROR_PICK_PID_FROM_LIST


class ProcessIdValidator(object):
    
    @staticmethod
    def HandleInvalidChoice(strPID):  # @UnusedVariable
        print(ERROR_PICK_PID_FROM_LIST)
        pass
        
    @staticmethod
    def IsValid(strPID, choiceValueSet=[]):  # @UnusedVariable
        nPID = INVALID_PID_VALUE
        
        try:
            nPID = int(strPID)
            return nPID > INVALID_PID_VALUE
        except:
            return False
