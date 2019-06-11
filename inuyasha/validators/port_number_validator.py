from common.inuyasha_symbols import LOWEST_VALID_USER_PORT, \
    HIGHEST_VALID_USER_PORT


class PortNumberValidator(object):
    
    @staticmethod
    def IsValid(nPort):
        return nPort >= LOWEST_VALID_USER_PORT \
            and nPort <= HIGHEST_VALID_USER_PORT
