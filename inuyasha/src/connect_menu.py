#!/usr/bin/python3

from menu import Menu
from consolemenu.items.function_item import FunctionItem
from machine_info import MachineInfo

APP_TITLE = "inuyasha"
APP_SUBTITLE = "Connect to a remote machine and mess it up"
IDM_CONNECT_REMOTE_MACHINE = "Connect to Remote Machine"


class ConnectMenu(Menu):

	def get_remote_machine_info(self):
		print(APP_TITLE)
		print(APP_SUBTITLE)
		print()
		print(IDM_CONNECT_REMOTE_MACHINE)
		print()
		szHostName = input("> Type the hostname of the remote machine: > ")
		nPort = int(input("> Type the port number of the remote machine: > "))
		self.machineInfo = MachineInfo(szHostName, nPort)
		print()
		print("Connecting to machine '{}' on port {}..." \
			.format(self.machineInfo.szHostName, self.machineInfo.nPort))
		print()
		input("Press ENTER key to continue...")
		pass
	
	def __init__(self):
		self.loadMenu(APP_TITLE, APP_SUBTITLE)
		# MenuItem is the base class for all items, 
		# it doesn't do anything when selected
		connect_item = FunctionItem(IDM_CONNECT_REMOTE_MACHINE,
								self.get_remote_machine_info)
		self.menu.append_item(connect_item)
		
