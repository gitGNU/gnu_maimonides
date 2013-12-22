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
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

#include <glib.h>
#include <gdk/gdkkeysyms.h>
#include <gst/gst.h>		
#include <gst/interfaces/xoverlay.h>
#include <gdk/gdkx.h>

#include "../common/globales.h"

#ifndef proyeccion_video
#define proyeccion_video 


//gboolean bus_callback(GstBus * bus, GstMessage *msg, gpointer data);
static void realize_cb (GtkWidget * widget, CustomData *data);//function to create video window
static gboolean on_key_press(GtkWidget *widget,GdkEventKey *event , CustomData *data); //evento de teclado
static gboolean expose_event(GtkWidget *widget, GdkEventExpose *event,CustomData *data);
static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static GstBusSyncReply
bus_sync_handler (GstBus * bus, GstMessage * message, CustomData *data);
int show_proyeccion_vid (GtkWidget *widget, int mode);

#endif
