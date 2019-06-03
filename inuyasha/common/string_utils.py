class StringUtilities(object):

        @staticmethod
        def IsNullOrWhiteSpace(value):
            # assume value is a string
            return value == '' or len(value.strip()) == 0
                
            
