#!/usr/bin/python3

from menu import Menu
from consolemenu import *
from consolemenu.items import *

class ConnectMenu(Menu):
	def __init__(self):
		self.loadMenu("shippo", "Remote Machine Fucker Upper")
		# MenuItem is the base class for all items, it doesn't do anything when selected
		connect_item = MenuItem("Connect to Remote Machine")
		self.menu.append_item(connect_item)
		self.menu.show()


