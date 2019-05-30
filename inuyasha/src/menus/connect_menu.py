#!/usr/bin/python3

from inuyasha_symbols import *  # @UnusedWildImport

from consolemenu.items.function_item import FunctionItem

from menus.menu_base import MenuBase
from command_target import CommandTarget


class ConnectMenu(MenuBase):
	
	def init_menu(self):
		connect_item = FunctionItem(IDM_CONNECT_REMOTE_MACHINE,
								CommandTarget.ConnectRemoteMachine)
		self.menu.append_item(connect_item)
	
	def __init__(self):
		self.loadMenu(APP_TITLE, APP_SUBTITLE)
		self.init_menu()
