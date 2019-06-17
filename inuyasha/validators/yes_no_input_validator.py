class YesNoInputValidator(object):
    
    @staticmethod
    def IsValid(strChoice):
        if not isinstance(strChoice, str):
            return False
        return strChoice.strip().lower() == 'y' \
            or strChoice.strip().lower() == 'n'