#!/usr/bin/python3

from consolemenu import ConsoleMenu


class MenuBase:
	def init_menu(self):
		pass

	def loadMenu(self, title, subtitle, bShowExit=True):
		self.menu = ConsoleMenu(title, subtitle, show_exit_option=bShowExit)

	def show(self):
		self.menu.show()
