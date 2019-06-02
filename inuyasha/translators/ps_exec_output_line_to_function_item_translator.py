class PsExecOutputLineToFunctionItemTranslator(object):
        
        @staticmethod 
        def ToFunctionItem(line):
            result = None
            if len(line.strip()) == 0:
                return result  # required parameter
            return result