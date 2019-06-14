class ShellcodeExecutionFailedException(Exception):
    
    def __init__(self, message):
        self.__message = message
        pass

    def __str__(self):
        return "ShellcodeExecutionFailedException: {}".format(
            self.__message)
