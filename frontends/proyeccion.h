/*
 * Maimonides Suite v1.0.11 - advertisment/information player via embedded Raspberry Pi
 *
 *  Copyright 2012-2013 by it's authors. 
 *
 *  Some rights reserved. See COPYING, AUTHORS.
 *  This file may be used under the terms of the GNU General Public
 *  License version 3.0 as published by the Free Software Foundation
 *  and appearing in the file COPYING included in the packaging of
 *  this file.
 *
 *  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 *  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <time.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <gdk/gdkkeysyms.h>
#include <gst/gst.h>		
#include <gst/interfaces/xoverlay.h>

#ifndef proyeccion
#define proyeccion 
int mode;
//gboolean bus_callback(GstBus * bus, GstMessage *msg, gpointer data);
static gboolean time_handler(GtkWidget *widget);  //evento de tiempo, disparado cada segundo
static gboolean retry_playing(CustomData *data); //evento de tiempo, disparado cada segundo
static gboolean on_key_press(GtkWidget *widget,GdkEventKey *event , CustomData *data); //evento de teclado
static gboolean on_key_press_nosound(GtkWidget *widget,GdkEventKey *event,CustomData *data); //evento de teclado sin sonido
static gboolean on_expose_event(GtkWidget *widget,GdkEventKey *event,gpointer data);
static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data);

int show_proyeccion (GtkWidget *widget, int mode);

#endif
