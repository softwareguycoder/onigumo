#!/usr/bin/python3

from consolemenu import ConsoleMenu


class MenuBase:

	def loadMenu(self, title, subtitle):
		self.menu = ConsoleMenu(title, subtitle, show_exit_option=True)

	def show(self):
		self.menu.show()
