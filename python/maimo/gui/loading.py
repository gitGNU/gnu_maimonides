#!/usr/bin/env python
"""@package main
 Documentation for this module.
 
 More details.
"""
from gi.repository import Gtk




class DlgLoading:
     """Loading dialog"""
     def __init__(self):
         self.builder = Gtk.Builder()
         self.builder.add_from_file("./gui/Loading.glade")
        # self.builder.add_objects_from_file("./gui/Loading.glade", ("progressbar", "butcancel"))
         self.cancel  = self.builder.get_object("butcancel")
         self.cancel.connect("clicked", self.onButtonCancel)
         

         self.window = self.builder.get_object("DlgLoading1")       

     def show(self):
         self.window.show_all()

     def onButtonCancel(self,butcancel):
        print "estamos en al funcion, capullo!"
        self.window.destroy()   



