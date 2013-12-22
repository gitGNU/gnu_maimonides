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
//#include "../backends/reproductor_sonido.h"

#include "proyeccion_vid.h"


static GstBusSyncReply
bus_sync_handler (GstBus * bus, GstMessage * message, CustomData *data)
{
 // ignore anything but 'prepare-xwindow-id' element messages
 if (GST_MESSAGE_TYPE (message) != GST_MESSAGE_ELEMENT)
   return GST_BUS_PASS;
 if (!gst_structure_has_name (message->structure, "prepare-xwindow-id"))
   return GST_BUS_PASS;

 if (data->window_handle != 0) {
   GstXOverlay *xoverlay;

   // GST_MESSAGE_SRC (message) will be the video sink element
   xoverlay = GST_X_OVERLAY (GST_MESSAGE_SRC (message));
   gst_x_overlay_set_window_handle (xoverlay, data->window_handle);
 } else {
   g_warning ("Should have obtained video_window_xid by now!");
 }

 gst_message_unref (message);
 return GST_BUS_DROP;
}


static gboolean on_key_press(GtkWidget *widget,GdkEventKey *event,CustomData *data) //evento de teclado
{
  if(event->keyval == GDK_KEY_Escape)
   {
     //CLEANING
     gtk_widget_destroy(widget);
   }
  else if(event->keyval == GDK_KEY_space)
    {
      if(playing)
	{	      
	  gst_element_set_state (data->playbin2, GST_STATE_PAUSED);
	  playing=0;
	}
      else
	{
	    gst_element_set_state (data->playbin2, GST_STATE_PLAYING);
	    playing=1;
	}
	 
    }   
     return FALSE;
}
static void realize_cb (GtkWidget * widget, CustomData *data)//draw window
{
  GdkWindow *window = gtk_widget_get_window (widget);


  if (!gdk_window_ensure_native (window))
    g_error("No se pudo crear una ventana nativa!!");
  data->window_handle = GDK_WINDOW_XID(window);
  

}


static gboolean expose_event(GtkWidget *widget, GdkEventExpose *event,CustomData *data)
{
  if (data->state < GST_STATE_PAUSED)
    {
      GtkAllocation allocation;
      GdkWindow *window = gtk_widget_get_window(widget);
      cairo_t *cr;

      gtk_widget_get_allocation(widget,&allocation);
      cr = gdk_cairo_create(window);
      cairo_set_source_rgb(cr,0,0,0);
      cairo_rectangle (cr,0,0,allocation.width,allocation.height);
      cairo_fill(cr);
      cairo_destroy(cr);
    }
  
  return FALSE;

}

/* This function is called when an error message is posted on the bus */
static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data) {
  GError *err;
  gchar *debug_info;
   
  /* Print error details on the screen */
  gst_message_parse_error (msg, &err, &debug_info);
  g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
  g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
  g_clear_error (&err);
  g_free (debug_info);
   
  /* Set the pipeline to READY (which stops playback) */
  gst_element_set_state (data->playbin2, GST_STATE_READY);
}
   

/* This function is called when an End-Of-Stream message is posted on the bus.
 * We just set the pipeline to READY (which stops playback) */
static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data) {

  gchar* video_path,*apppath;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL, *videos_index = NULL;
  struct glob_settings* settings = NULL;
  
  //NEW URI
  if(video_path!=NULL)
    free(video_path);
  cargar_XML_biblioteca (XML_MODE, &image_index,&audios_index,&videos_index,&settings);

//return to start
 gst_element_seek(data->playbin2,1.0,GST_FORMAT_TIME,GST_SEEK_FLAG_FLUSH,GST_SEEK_TYPE_SET,2000000000,GST_SEEK_TYPE_NONE,GST_CLOCK_TIME_NONE);
  
  
 actual_video_index++;
 if(actual_video_index >= no_videos)
    actual_video_index=0;
  
  if(iDebug)
    g_printf("freeing video_path variable \n");
  if(video_path!=NULL)
    free(video_path);

  return_app_path(&apppath);
  //printf("DEBUG:path de la aplicación! %s \n",apppath);
  video_path = malloc (25*sizeof(gchar)+ 64*sizeof(gchar) + strlen(apppath)*sizeof(gchar));
  strcpy(video_path,apppath);
  //printf("DEBUG: liberando memoria \n");
  free(apppath);
  //printf("DEBUG: fin, liberando memoria \n");
  strcat(video_path,"video/");
  strcat(video_path,videos_index[actual_video_index].name);
  g_strlcat(video_path,"\0",2);

 //////////////////////////
 g_print ("End-Of-Stream reached.\n");
 // Set new URI to play TEMPORAL
 gst_element_set_state (data->playbin2, GST_STATE_NULL);
 // gst_object_unref (data->playbin2);
  

 /* Create the elements */
 //data->playbin2 = gst_element_factory_make ("playbin2", "playbin2");
 g_object_set (data->playbin2, "uri",g_filename_to_uri(video_path,NULL,NULL), NULL);

 if(playing ==-1)
   {
     if( gst_element_set_state (data->playbin2, GST_STATE_PLAYING)==GST_STATE_CHANGE_FAILURE)
       playing=-1;
     else
       playing=1;
   }
 else
   gst_element_set_state (data->playbin2, GST_STATE_PLAYING);

 playing = 1;
 free(video_path);
 if(audios_index != NULL)
    free(audios_index);
  if(image_index != NULL)
    free(image_index);
  if(videos_index != NULL)
    free(videos_index);
 if(settings!=NULL)
     free(settings);
}

/* This function is called when the pipeline changes states. We use it to
 * keep track of the current state. */
static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data) {
  GstState old_state, new_state, pending_state;
  gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
  if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->playbin2)) {
    data->state = new_state;
    g_print ("State set to %s\n", gst_element_state_get_name (new_state));
    /* if (old_state == GST_STATE_READY && new_state == GST_STATE_PAUSED) {
      // For extra responsiveness, we refresh the GUI as soon as we reach the PAUSED state 
      refresh_ui (data);
  }*/
  }
}

int show_proyeccion_vid (GtkWidget *widget, int mode)
{
 
  GtkWidget *dialog;
  GtkWidget *video_widget;
  CustomData data;
  GstStateChangeReturn ret;
  GstBus *bus;
  gchar* video_path,*apppath;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL, *videos_index = NULL;
  struct glob_settings* settings = NULL;
  

  i=0;//index to 0
  actual_video_index=0;
cargar_XML_biblioteca (XML_MODE, &image_index,&audios_index,&videos_index,&settings);

  if (!videos_index) //no images, we return
    return NO_VIDEOS;

  //image list initialization(TEMPORAL)
  /////////////////////////////////////
  /* initialization */
  gtk_init(NULL,NULL);
  gst_init (NULL,NULL);
 
  /* Initialize our data structure */
  memset (&data, 0, sizeof (data));
   
  /* Create the elements */
  data.playbin2 = gst_element_factory_make ("playbin2", "playbin2");
  data.sink = gst_element_factory_make ("xvimagesink", "videosink");

  if (!data.playbin2) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }
   // Set the URI to play TEMPORAL
  // g_object_set (data.playbin2, "uri", "file:///home/prointegra/nuevax2.ogg", NULL);
  if(iDebug)
    g_printf("freeing video_path variable \n");
  if(video_path!=NULL)
    free(video_path);

  return_app_path(&apppath);
  //printf("DEBUG:path de la aplicación! %s \n",apppath);
  video_path = malloc (25*sizeof(gchar)+ 64*sizeof(gchar) + strlen(apppath)*sizeof(gchar));
  strcpy(video_path,apppath);
  //printf("DEBUG: liberando memoria \n");
  free(apppath);
  //printf("DEBUG: fin, liberando memoria \n");
  strcat(video_path,"video/");
  strcat(video_path,videos_index[actual_video_index].name);
  g_strlcat(video_path,"\0",2);
 
  g_printf("INFO:video a reproducir!! : %s \n",video_path);
  /////////////////////////////////////////////// 
  
  g_object_set (data.playbin2, "uri",g_filename_to_uri(video_path,NULL,NULL), NULL);
 
  video_widget = gtk_drawing_area_new();
  gtk_widget_set_double_buffered(video_widget,FALSE);

  dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(dialog),TRUE);
  gtk_window_set_keep_above(GTK_WINDOW(dialog),TRUE);
  //pantalla copmleta
  gtk_window_fullscreen(GTK_WINDOW(dialog));
  //eliminar bordes de la ventana
  gtk_window_set_decorated(GTK_WINDOW(dialog),FALSE);
  //gtk_window_set_default_size(GTK_WINDOW(dialog), 640, 480); 
  gtk_widget_set_app_paintable(dialog,TRUE);
  //playlist_initialization(TEMPORAL)
  //////////////////////////////////

  ///////////////////////////////////////////////////
  g_signal_connect(video_widget,"draw",G_CALLBACK(expose_event),&data);
  g_signal_connect(video_widget,"realize",G_CALLBACK(realize_cb),&data);

  g_signal_connect(dialog,"key_press_event",G_CALLBACK(on_key_press),&data);
  g_signal_connect(dialog, "destroy",G_CALLBACK (gtk_main_quit),NULL);

  // main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  //  gtk_box_pack_start(GTK_BOX(main_box),video_widget,TRUE,TRUE,0);
  gtk_container_add (GTK_CONTAINER(dialog),video_widget);



  gtk_widget_show_all(dialog);
  //creation of window ID, necessary to embedded gstreamer video in GTK
  gtk_widget_realize(video_widget);
 

   //audio bus signals
  bus = gst_element_get_bus (data.playbin2);
  gst_bus_set_sync_handler(bus,(GstBusSyncHandler) bus_sync_handler,&data);
  gst_bus_add_signal_watch (bus);
  g_signal_connect (G_OBJECT (bus), "message::error", (GCallback)error_cb, &data);
  g_signal_connect (G_OBJECT (bus), "message::eos", (GCallback)eos_cb, &data);
  g_signal_connect (G_OBJECT (bus), "message::state-changed", (GCallback)state_changed_cb, &data);
  gst_object_unref (bus);

  /* Start playing */
  ret = gst_element_set_state (data.playbin2, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    playing = -1;
  }
  else
    playing = 1;


  gtk_main();
 
 /* clean up */
  //cairo_surface_destroy(image);
  gst_element_set_state(data.playbin2, GST_STATE_NULL);
  gst_object_unref(data.playbin2);
  
 
  free(video_path);
  if(audios_index != NULL)
    free(audios_index);
  if(image_index != NULL)
    free(image_index);
  if(videos_index != NULL)
    free(videos_index);
 if(settings!=NULL)
     free(settings);

  return ALL_OK;

}
