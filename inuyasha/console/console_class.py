from os import system, name

class Console(object):
    @staticmethod
    # define our clear function 
    def Clear(): 
      
        # for windows 
        if name == 'nt': 
            _ = system('cls') 
      
        # for mac and linux(here, os.name is 'posix') 
        else: 
            _ = system('clear') 
