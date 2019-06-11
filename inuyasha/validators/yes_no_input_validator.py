class YesNoInputValidator(object):
    
    @staticmethod
    def IsValid(strChoice):
        return strChoice.strip().lower() == 'y' \
            or strChoice.strip().lower() == 'n'