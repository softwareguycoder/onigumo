class ShellcodeValidator(object):
    
    @staticmethod
    def IsValid(shellcodeOutput):
        return shellcodeOutput is not None \
            and len(shellcodeOutput[0]) > 0 \
            and shellcodeOutput[1] > 0
            
            