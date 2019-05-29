#!/usr/bin/python3

from inuyasha_symbols import *  # @UnusedWildImport

from consolemenu.items.function_item import FunctionItem

from menus.menu_base import MenuBase
from factories.machine_info_factory import MachineInfoFactory
from command_target import CommandTarget

class ConnectMenu(MenuBase):
	
	def __init__(self):
		self.loadMenu(APP_TITLE, APP_SUBTITLE)
		# MenuItem is the base class for all items, 
		# it doesn't do anything when selected
		connect_item = FunctionItem(IDM_CONNECT_REMOTE_MACHINE,
								CommandTarget.ConnectRemoteMachine)
		self.menu.append_item(connect_item)
		
