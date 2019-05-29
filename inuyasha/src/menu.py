#!/usr/bin/python3

from consolemenu import *
from consolemenu.items import *

class Menu:
	def loadMenu(self, title, subtitle):
		self.menu = ConsoleMenu(title, subtitle, show_exit_option=True)


