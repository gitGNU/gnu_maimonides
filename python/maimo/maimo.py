#!/usr/bin/env python
"""@package main
 Documentation for this module.
 
 More details.
"""


from gi.repository import Gtk
import gui.loading



class Maimoglade:
	def onDeleteWindow(self, *args):		
		Gtk.main_quit(*args)

	def onButtonPressed(self, button2):
		dialog = gui.loading.DlgLoading()
		dialog.show()
        	print("Hello World")	
				

store = Gtk.ListStore(str)
store.append(['hello'])
store.append(['goodbye'])

builder = Gtk.Builder()
builder.add_from_file("Maimo.glade")
builder.connect_signals(Maimoglade())


combo = builder.get_object("combobox1")
combo.set_model(store)
#combo.append_text("store")
#combo.append("store1")
#combo.append("store1")

render = Gtk.CellRendererText()
combo.pack_start(render, True)
combo.add_attribute(render, 'text', 0)


window = builder.get_object("window1")

window.show_all()

Gtk.main()
#print store







