from common.string_utils import StringUtilities
from common.list_utils import ListUtilities

ERROR_RESPONSE_MUST_BE_YES_OR_NO = \
    "ERROR: Only 'Y' for yes or 'N' for no is allowed (lower case is okay too)."

# this is the default format for a prompt
# the text that tells the user what to enter is the first
# then comes the list of available choices
# then comes the default value (entered into the prompt when
# the user presses the ENTER key without typing anything
PROMPT_FORMAT = "> {} ({})[{}]: > "

class Prompter(object):    
    @staticmethod
    def __GetDisplayedPrompt(strPrompt, pvDefault=None,
        choiceValueSet=[]):
        strDisplayedPrompt = strPromptFormat = PROMPT_FORMAT
        if pvDefault is None or \
            StringUtilities.IsNullOrWhiteSpace(pvDefault):
            strPromptFormat = strPromptFormat.replace("[{}]", '')
        if not len(choiceValueSet):
            strPromptFormat = strPromptFormat.replace("({})", '')
        if pvDefault is None:
            if not len(choiceValueSet):
                strDisplayedPrompt = strPromptFormat.format(strPrompt)
            else:
                strDisplayedPrompt = strPromptFormat.format(strPrompt,
                    ListUtilities.FormatEltsSeparatedBy(
                    '/', choiceValueSet))
        else:
            if not len(choiceValueSet):
                strDisplayedPrompt = strPromptFormat.format(strPrompt,
                    pvDefault)
            else:
                strDisplayedPrompt = strPromptFormat.format(strPrompt,
                    ListUtilities.FormatEltsSeparatedBy(
                    '/', choiceValueSet), pvDefault)                
        return strDisplayedPrompt

    @staticmethod
    def __IsChoiceNotInValueSet(theResult, choiceValueSet=[]):
        return str(theResult) not in map(str, choiceValueSet)
    
    @staticmethod
    def __DoDisplayPrompt(strPrompt, pvDefault=None, choiceValueSet=[], 
                        keyboardInterruptHandler=None, inputValidator=None, 
                        invalidInputHandler=None, invalidChoiceHandler=None,
                        pvInvalidValue=None):
        global PROMPT_FORMAT
        try:
            if StringUtilities.IsNullOrWhiteSpace(strPrompt):
                return pvDefault
            theResult = input(
                Prompter.__GetDisplayedPrompt(strPrompt, pvDefault, 
                    choiceValueSet))
            if not theResult:
                theResult = pvDefault
            if inputValidator is not None:
                if not inputValidator(theResult, choiceValueSet) \
                    and invalidInputHandler is not None:
                    invalidInputHandler(theResult)
                    return pvInvalidValue
            elif Prompter.__IsChoiceNotInValueSet(
                theResult, choiceValueSet):
                strChoiceDisplay = ListUtilities.FormatEltsSeparatedBy(
                    ", ", choiceValueSet);
                strChoiceDisplay = StringUtilities.ReplaceFromRight(strChoiceDisplay, 
                    ", ", ", or ", 1)
                if not invalidChoiceHandler:
                    print("ERROR: Please choose {}.".format(strChoiceDisplay))
                else:
                    invalidChoiceHandler(theResult)
                return pvInvalidValue
            return theResult
        except KeyboardInterrupt:
            if keyboardInterruptHandler is not None:
                keyboardInterruptHandler() 
    
    @staticmethod
    def PromptForString(strPrompt, strDefault, choiceValueSet=[], 
                        keyboardInterruptHandler=None, inputValidator=None, 
                        invalidInputHandler=None):
        theResult = Prompter.__DoDisplayPrompt(strPrompt, strDefault, 
            choiceValueSet, keyboardInterruptHandler, inputValidator, 
            invalidInputHandler)
        if theResult is None or not len(theResult.strip()):
            theResult = strDefault
        return theResult

    @staticmethod
    def PromptForInt(strPrompt, nDefault, choiceValueSet=[],
        keyboardInterruptHandler=None, inputValidator=None, 
        invalidInputHandler=None, invalidChoiceHandler=None,
        nInvalidValue=0):
        theResult = nInvalidValue
        while theResult == nInvalidValue:
            try:
                theResult = int(
                    Prompter.__DoDisplayPrompt(strPrompt, nDefault, 
                    choiceValueSet=choiceValueSet, 
                    keyboardInterruptHandler=keyboardInterruptHandler,
                    inputValidator=inputValidator, 
                    invalidInputHandler=invalidInputHandler,
                    invalidChoiceHandler=invalidChoiceHandler, 
                    pvInvalidValue=nInvalidValue)
                    )
                if theResult == nInvalidValue:
                    continue
                if theResult is None:
                    theResult = nDefault
                return theResult
            except KeyboardInterrupt: #parse error more likely
                return nInvalidValue
            except:
                if invalidInputHandler is not None:
                    invalidInputHandler(theResult)
                    theResult = nInvalidValue
                else:
                    return nInvalidValue
    
    @staticmethod
    def __YesNoValidator(theResult, choiceValueSet):  # @UnusedVariable
        if not isinstance(theResult, str):
            return False
        return theResult.lower() == 'y' or theResult.lower() == 'n'
    
    @staticmethod
    def __YesNoInvalidInputHandler(theResult):  # @UnusedVariable
        print(ERROR_RESPONSE_MUST_BE_YES_OR_NO)
        pass
        
    
    @staticmethod
    def YesNoPrompt(strPrompt, bDefault, keyboardInterruptHandler,
                    pvInvalidInput=None):
        theResult = pvInvalidInput
        while theResult == pvInvalidInput:
            theResult = Prompter.__DoDisplayPrompt(strPrompt, 
                    "Y" if bDefault else "N", 
                    ['Y','N'], keyboardInterruptHandler,
                    Prompter.__YesNoValidator, 
                    Prompter.__YesNoInvalidInputHandler,
                    pvInvalidInput)
        return str(theResult).lower() == 'y'
    
    
