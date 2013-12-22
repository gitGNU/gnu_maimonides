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
#include <glib.h>
#include <cairo.h>
#include <gdk/gdkkeysyms.h>
#include <gst/gst.h>		
#include <gst/interfaces/xoverlay.h>
///#include <gdk/gdkx.h>

#include "../common/globales.h"
//#include "../backends/reproductor_sonido.h"

#include "proyeccion.h"



static gboolean on_key_press(GtkWidget *widget,GdkEventKey *event,CustomData *data) //evento de teclado
{
  if(event->keyval == GDK_KEY_Escape)
   {
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
static gboolean on_key_press_nosound(GtkWidget *widget,GdkEventKey *event,CustomData *data) //evento de teclado
{
  if(event->keyval == GDK_KEY_Escape)
   {
     gtk_widget_destroy(widget);
   }
  else if(event->keyval == GDK_KEY_space)
    {
      if(playing)
	{
	  playing=0;
	}
      else
	{
	  playing=1;
	}
	 
    }
    
     return FALSE;
}
static gboolean time_handler(GtkWidget *widget) //evento de tiempo, disparado cada segundo
{
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL, *videos_index = NULL;
  struct glob_settings* settings = NULL;

cargar_XML_biblioteca (XML_MODE, &image_index,&audios_index,&videos_index,&settings);
  if ( gtk_widget_get_window(widget) == NULL) return FALSE;
  if(playing)
    {
      seconds = seconds +1;
      //printf("número de segundos que llevamos= %d \n",seconds);
      //printf("número de segundos de la imágen actual= %d \n",image_index[i].seconds);
      if(seconds >= image_index[i].seconds)
       {
	 seconds = 0;
	 i++;
	 if(i >= no_images)
	   {
	     i = 0;
	   }
 
	 //printf("mandamos comando de dibujo!\n");
	 gtk_widget_queue_draw(widget);
       }
      /*      if(playing == -1)
	{
	  if( gst_element_set_state (data->playbin2, GST_STATE_PLAYING)==GST_STATE_CHANGE_FAILURE)
	    playing=-1;
	  else
	    playing=1;
	}
      */
    }
   if(audios_index != NULL)
    free(audios_index);
  if(image_index != NULL)
    free(image_index);
  if(videos_index != NULL)
    free(videos_index);
 if(settings!=NULL)
     free(settings);
  return TRUE;
}
static gboolean retry_playing(CustomData *data) //evento de tiempo, disparado cada segundo
{
 if(playing == -1)
	{
	  if( gst_element_set_state (data->playbin2, GST_STATE_PLAYING)==GST_STATE_CHANGE_FAILURE)
	    playing=-1;
	  else
	    playing=1;
	}
}

static gboolean on_expose_event(GtkWidget *widget,GdkEventKey *event,gpointer data)
{
  cairo_t *cr;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL, *videos_index = NULL;
  struct glob_settings* settings = NULL;
 
  cairo_surface_t * image;

cargar_XML_biblioteca (XML_MODE, &image_index,&audios_index,&videos_index,&settings);
  //cairo_surface_destroy(pub_image); 
  cr = gdk_cairo_create(gtk_widget_get_window(widget));
  printf("intentamos dibujar la imagen: %s \n",image_index[i].name);
  image = cairo_image_surface_create_from_png(image_index[i].name);
  cairo_set_source_surface ( cr, image , 0, 0);
  cairo_paint(cr);
    
  cairo_destroy(cr);
  cairo_surface_destroy(image);
  if(audios_index != NULL)
    free(audios_index);
  if(image_index != NULL)
    free(image_index);
  if(videos_index != NULL)
    free(videos_index);
 if(settings!=NULL)
     free(settings);
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
  gchar* audio_path,*apppath;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL, *videos_index = NULL;
  struct glob_settings* settings = NULL;
 
 cargar_XML_biblioteca (XML_MODE, &image_index,&audios_index,&videos_index,&settings);

  g_print ("End-Of-Stream reached.\n");
  //return to start
  gst_element_seek(data->playbin2,1.0,GST_FORMAT_TIME,GST_SEEK_FLAG_FLUSH,GST_SEEK_TYPE_SET,2000000000,GST_SEEK_TYPE_NONE,GST_CLOCK_TIME_NONE);
      
  //NEW URI
  actual_audio_index++;
  if(actual_audio_index >= no_pistas)
    actual_audio_index=0;

  return_app_path(&apppath);
  //printf("DEBUG:path de la aplicación! %s \n",apppath);
  audio_path = malloc (25*sizeof(gchar)+ 64*sizeof(gchar) + strlen(apppath)*sizeof(gchar));
  strcpy(audio_path,apppath);
  //printf("DEBUG: liberando memoria \n");
  free(apppath);
  //printf("DEBUG: fin, liberando memoria \n");
  strcat(audio_path,"musica/");
  strcat(audio_path,audios_index[actual_audio_index].name);
  g_strlcat(audio_path,"\0",2);
  g_printf("audio a reproducir!! : %s \n",audio_path);

  gst_element_set_state (data->playbin2, GST_STATE_NULL);
  ///////////////////////////////////////////////
  g_object_set (data->playbin2, "uri",g_filename_to_uri(audio_path,NULL,NULL), NULL);


  if(playing ==-1)
    {
      if( gst_element_set_state (data->playbin2, GST_STATE_PLAYING)==GST_STATE_CHANGE_FAILURE)
	playing=-1;
      else
	playing=1;
    }
  else
    gst_element_set_state (data->playbin2, GST_STATE_PLAYING);
 
  // gst_element_seek_simple(data->playbin2,GST_FORMAT_TIME,GST_SEEK_FLAG_FLUSH,0);

  // gst_element_seek(data->playbin2,1.0,GST_FORMAT_TIME,GST_SEEK_FLAG_FLUSH,GST_SEEK_TYPE_SET,2000000000,GST_SEEK_TYPE_NONE,GST_CLOCK_TIME_NONE);
  //g_signal_emit (data,signals[EOS],0);
   
 playing = 1;
 //recogida de basura
 free(audio_path);
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
      //For extra responsiveness, we refresh the GUI as soon as we reach the PAUSED state 
      refresh_ui (data);
  }*/
  }
}

int show_proyeccion (GtkWidget *widget, int g_mode)
{
 
  GtkWidget *dialog;
  CustomData data;
  GstStateChangeReturn ret;
  GstBus *bus;
  gchar* audio_path,*apppath;
  int mode = g_mode;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL, *videos_index = NULL;
  struct glob_settings* settings = NULL;
 

  //printf("DEBUG estamos en proeyccion!!!\n");
  //printf("DEBUG cargamos vectores del XML!!!\n");
  cargar_XML_biblioteca (XML_MODE, &image_index,&audios_index,&videos_index,&settings);
  //printf("DEBUG fin, cargamos vectores del XML!!!\n");
  i=0;//index to 0
  actual_audio_index=0;

  if (image_index == NULL) //no images, we return
    return NO_IMAGES;

  //image list initialization(TEMPORAL)
  // image = cairo_image_surface_create_from_png(image_index[i].name); //created first pic
  /////////////////////////////////////
  /* initialization */
  //printf("DEBUG: gtk  initialization!\n");
  gtk_init(NULL,NULL);
  gst_init (NULL,NULL);
  //printf("DEBUG:fin gtk  initialization!\n");
  if(mode == IMAGES_SOUND)
    {
      //printf("DEBUG: cargando gstreamer!\n");
      if(iDebug)
	g_printf("DBG: creation of pipeline \n");      
      /* Initialize our data structure */
      //printf("DEBUG: estableciendo memoria de los datos!\n"); 
      memset (&data, 0, sizeof (data));
      printf("DEBUG:fin, estableciendo memoria de los datos!\n");  
      /* Create the elements */
      //printf("DEBUG:creando playbin!\n");
      data.playbin2 = gst_element_factory_make ("playbin2", "playbin2");
      //printf("DEBUG:fin, creando playbin!\n");
      if (!data.playbin2) {
	g_printerr ("Not all elements could be created.\n");
	return -1;
      }
      //printf("DEBUG: a reproducir el audio: %s \n",audios_index[actual_audio_index].name);
      // Set the URI to play TEMPORAL
      return_app_path(&apppath);
      //printf("DEBUG:path de la aplicación! %s \n",apppath);
      audio_path = malloc (25*sizeof(gchar)+ 64*sizeof(gchar) + strlen(apppath)*sizeof(gchar));
      strcpy(audio_path,apppath);
      //printf("DEBUG: liberando memoria \n");
      free(apppath);
      //printf("DEBUG: fin, liberando memoria \n");
      strcat(audio_path,"musica/");
      strcat(audio_path,audios_index[actual_audio_index].name);    
      g_strlcat(audio_path,"\0",2);
      g_printf("INFO: audio a reproducir!! : %s \n",audio_path);
      ///////////////////////////////////////////////
      g_object_set (data.playbin2, "uri",g_filename_to_uri(audio_path,NULL,NULL), NULL);      
    }
  else
    playing = 1;

  dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(dialog),TRUE);
  gtk_window_set_keep_above(GTK_WINDOW(dialog),TRUE);
  //pantalla copmleta
  gtk_window_fullscreen(GTK_WINDOW(dialog));
  //eliminar bordes de la ventana
  gtk_window_set_decorated(GTK_WINDOW(dialog),FALSE);
  gtk_window_set_default_size(GTK_WINDOW(dialog), 1024, 768); 
  gtk_widget_set_app_paintable(dialog,TRUE);
  //playlist_initialization(TEMPORAL)
  //////////////////////////////////
  if(mode == IMAGES_SOUND)
    g_signal_connect(dialog,"key_press_event",G_CALLBACK(on_key_press),&data);
  else
    g_signal_connect(dialog,"key_press_event",G_CALLBACK(on_key_press_nosound),&data);

  g_signal_connect(dialog,"draw",G_CALLBACK(on_expose_event),NULL);
  g_signal_connect(dialog, "destroy",G_CALLBACK (gtk_main_quit), NULL);

  if(mode == IMAGES_SOUND)
    {
      if(iDebug)
	g_printf("DBG: creation of bus \n");
      //audio bus signals
      bus = gst_element_get_bus (data.playbin2);
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
    }
  else
    playing = 1;
  g_timeout_add(1000, (GSourceFunc) time_handler, GTK_WIDGET( dialog));
  if(mode == IMAGES_SOUND)
    g_timeout_add(1000, (GSourceFunc) retry_playing, &data);
  gtk_widget_show_all(dialog);

 
  gtk_main();
 
 /* clean up */

  if(mode == IMAGES_SOUND)
    {
      gst_element_set_state(data.playbin2, GST_STATE_NULL);
      gst_object_unref(data.playbin2);
      free(audio_path);
    }
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
