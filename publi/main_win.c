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
#include <gtk/gtk.h>
#include <cairo.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "../common/globales.h"
#include "../frontends/proyeccion.h"
#include "../frontends/proyeccion_vid.h"
#include "../frontends/progressbar.h"

#include "main_win.h"

GtkTextBuffer *buffer;
    
static void start_show(GtkWidget *widget)
{
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;

  cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);
 
  switch(settings->prestype)
    {

    case IMAGES_SOUND:
      if(no_images>0)
	{
	  printf("ejecutamos imagenes con sonido \n");
	  show_proyeccion(widget,settings->prestype);
	}
      break;
    case IMAGES_NOSOUND:
      if(no_images>0)
	{
	  printf("ejecutamos imagenes sin sonido \n");
	  show_proyeccion(widget,settings->prestype);
	}
      break; 
    default:
      if(no_videos>0)
	{
	  printf("ejecutamos videos \n");
	  show_proyeccion_vid(widget,settings->prestype);
	}
      break; 
    }

  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
  if(settings != NULL)
    free(settings);
  return;

}

static void show_about(GtkWidget *widget, gpointer data)
{

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("Maimonides_logo_nides.png", NULL);

  GtkWidget *dialog = gtk_about_dialog_new();

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "ProMaimónides v.1.0.11");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), appversion); 
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), 
      "(c) Pro Integra S.L.");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
     "Nides es el programa proyector de información de la suit ProMaimónides");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),"http://www.prointegra.com");
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  g_object_unref(pixbuf), pixbuf = NULL;
  gtk_dialog_run(GTK_DIALOG (dialog));

  gtk_widget_destroy(dialog);
}

static GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
 }
void clear_textview ()
{
    GtkTextIter start, end;

  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_delete(buffer, &start, &end);
}
void actualize_textview ()
{
  GtkTextIter iter;
  gchar* Text_input;
  int i;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  Text_input = malloc(64*sizeof(gchar));


  cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

  //MODO
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  sprintf(Text_input,"OPCIONES DE REPRESENTACION \n");
  gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
  //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  switch(settings->prestype)
    {
    case(IMAGES_SOUND):
      sprintf(Text_input,"  TIPO: IMAGENES CON SONIDO \n\n");
      break;
    case(IMAGES_NOSOUND):
      sprintf(Text_input,"  TIPO: IMAGENES SIN SONIDO \n\n");
      break;
    default:
      sprintf(Text_input,"  TIPO: VIDEOS \n\n");
      break;     
    }
  printf("DEBUG: Actualiza tabla!\n");
  gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
  //IMAGENES
  //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  sprintf(Text_input,"IMÁGENES CARGADAS\n");
  gtk_text_buffer_insert(buffer, &iter, Text_input, -1);

  for(i=0;i<no_images;i++)
    {
      //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
      sprintf(Text_input,"  IMÁGEN\n");
      gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
      //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
      //sprintf(Text_input,"    NOMBRE: ");
      //strcat(Text_input,image_index[i].name);
      //strcat(Text_input," \n");
      sprintf(Text_input,"    NOMBRE: %s \n",image_index[i].name);
      gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
      //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);       
      sprintf(Text_input,"    TIEMPO: %d \n",image_index[i].seconds);
      gtk_text_buffer_insert(buffer, &iter,Text_input, -1);
      //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
      sprintf(Text_input,"    TRANSICIÓN: SIN_TRANSICIÓN \n\n");
      gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
    }
  //VIDEOS
  //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  sprintf(Text_input,"VIDEOS CARGADOS \n");
  gtk_text_buffer_insert(buffer, &iter, Text_input, -1);

  for(i=0;i<no_videos;i++)
    {
      //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
      sprintf(Text_input,"  VIDEO \n");
      gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
      //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
      sprintf(Text_input,"    NOMBRE: %s \n\n",videos_index[i].name);
      gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
    }
  //AUDIOS
  //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  sprintf(Text_input,"PISTAS DE AUDIO CARGADAS \n");
  gtk_text_buffer_insert(buffer, &iter, Text_input, -1);

  for(i=0;i<no_pistas;i++)
    {
      //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
      sprintf(Text_input,"  PISTA \n");
      gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
      //gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
      sprintf(Text_input,"    NOMBRE: %s \n\n",audios_index[i].name);
      gtk_text_buffer_insert(buffer, &iter, Text_input, -1);
    }
  if(no_images)
    {
      if(audios_index != NULL)
	free(audios_index);
    }
  if(no_pistas)
    {
      if(image_index != NULL)
	free(image_index);
    }
  if(no_videos)
    {
      if(videos_index != NULL)
	free(videos_index);
    }
   if(settings != NULL)
    free(settings);

  return;
} 

void refresh_text (GtkWidget* widget)
{
  load_biblioteca(GINT_TO_POINTER(FUSB_MODE));
  clear_textview(buffer);
  //actualize_textview();
  return;
}
int main_win (int init_mode)
{
  GtkWidget *window; 
  GtkWidget *main_vbox; 
  GtkWidget *hbox1,*hbox2,*hbox3;
  GtkWidget *halign;
  GtkWidget* scrolledwindow; 

  GtkWidget *btcarga,*btplay,*bthelp;
  GtkWidget *icon;
  //GtkLabel *lbconfiguracion;
  GtkWidget *frconfiguracion; 
  GtkWidget *txtconfiguracion; 

  GtkTextIter start, end;
  GtkTextIter iter;

  int mode = init_mode;

  gtk_init(NULL, NULL);
  //window options
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
  gtk_window_set_title(GTK_WINDOW(window), "Nides");
  gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("Maimonides_logo_nides.png"));
  //initializing layouts
  main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add(GTK_CONTAINER(window), main_vbox);
  //buttons
  btcarga =  gtk_button_new_with_label("Transeferencia desde USB");
  btplay=  gtk_button_new_with_label("Presentación");
   bthelp = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (bthelp), icon);  
  gtk_box_pack_start(GTK_BOX(hbox1),btcarga , FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox1),btplay , FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox1),bthelp , FALSE, FALSE, 0);
  //buttons  align
  halign = gtk_alignment_new(0.5, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), hbox1);
  //configuration
  //configuration frame
  frconfiguracion = gtk_frame_new("Configuración");
  gtk_frame_set_shadow_type(GTK_FRAME(frconfiguracion), GTK_SHADOW_IN);
  gtk_box_pack_start(GTK_BOX(hbox2),frconfiguracion , TRUE, TRUE, 0);
  //configuration text
  txtconfiguracion = gtk_text_view_new();
  gtk_text_view_set_editable (txtconfiguracion,FALSE);
  scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
  //gtk_box_pack_start(GTK_BOX(frconfiguracion),txtconfiguracion , TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (frconfiguracion), scrolledwindow );
  gtk_container_add(GTK_CONTAINER(scrolledwindow), txtconfiguracion);
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txtconfiguracion));
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
  //gtk_text_buffer_insert(buffer, &iter, "Configuración aquí! \n", -1);
  //window managing
  gtk_box_pack_start(GTK_BOX(main_vbox),halign, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(main_vbox),hbox2 , TRUE, TRUE, 0);
 

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
			   G_CALLBACK(gtk_main_quit), G_OBJECT(window));
  //buttons signals
  g_signal_connect(G_OBJECT(btcarga), "clicked",
		   G_CALLBACK(refresh_text), NULL);
  g_signal_connect(G_OBJECT(btplay), "clicked",
		   G_CALLBACK(start_show), NULL);
  g_signal_connect(G_OBJECT(bthelp), "clicked",
		   G_CALLBACK(show_about), window);

  //  g_timeout_add_seconds(10, (GSourceFunc)refresh_text,buffer);
  //actualizamos configuración actual
  actualize_textview ();
  gtk_widget_show_all(window);
 
  gtk_main();

  return 0;

}
