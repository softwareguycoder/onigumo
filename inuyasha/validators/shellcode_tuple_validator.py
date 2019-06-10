class ShellcodeTupleValidator(object):
    
    @staticmethod
    def IsValid(shellcodeTuple):
        return shellcodeTuple \
            and len(shellcodeTuple) == 2 \
            and shellcodeTuple[1] > 0
