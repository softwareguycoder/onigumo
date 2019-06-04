from factories.function_item_factory import FunctionItemFactory
from validators.process_info_validator import ProcessInfoValidator

class PsExecOutputLineToFunctionItemTranslator(object):
        
        @staticmethod 
        def ToFunctionItem(itemHandler, procInfo):
            result = None
            if itemHandler is None:
                return result   # required parameter
            if not ProcessInfoValidator.IsValid(procInfo):
                return result  # required parameter
            
            # Make a FunctionItem that is named by the command string
            # of the process the user wants to kill, and that passes the
            # procInfo created from the information about that process
            result = FunctionItemFactory.MakeWithArgs(procInfo.get_CMD(),
                itemHandler, procInfo)
            return result
