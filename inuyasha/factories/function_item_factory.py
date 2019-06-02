from consolemenu.items.function_item import FunctionItem


class FunctionItemFactory(object):
    
    @staticmethod
    def Make(itemName, itemHandler):
        return FunctionItem(itemName, itemHandler)
