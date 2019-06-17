from common.inuyasha_symbols import INVALID_PID_VALUE, ERROR_PICK_PID_FROM_LIST


class ProcessIdValidator(object):
    
    @staticmethod
    def HandleInvalidChoice(nPID):
        if not isinstance(nPID, int):
            return
        print(ERROR_PICK_PID_FROM_LIST)
        pass
        
    @staticmethod
    def IsValid(nPID, choiceValueSet=[]):  # @UnusedVariable
        if not isinstance(nPID, int):
            return False
        return nPID > INVALID_PID_VALUE
