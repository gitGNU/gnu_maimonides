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
#include <glib.h>
#include <cairo.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "../common/globales.h"
#include "../frontends/proyeccion.h"
#include "../frontends/proyeccion_vid.h"
#include "../frontends/progressbar.h"

#include "main_win.h"

enum
{
  LIST_ITEM = 0,
  N_COLUMNS
};
typedef struct strprogressdlg
{
  GtkWidget *dialog;
  GtkProgressBar *progressbar;
}strprogressdlg;

GtkWidget *ima_list;
GtkWidget *mus_list;
GtkWidget *vid_list;
GtkComboBoxText *combo_chooseplaying;
typedef struct main_win_data
{
  GtkWidget *ima_list;
  GtkWidget *mus_list;
  GtkWidget *vid_list;
  GtkComboBoxText *combo_chooseplaying;
  
}strdata;

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
	  printf("INFO: ejecutamos imagenes con sonido \n");
	  show_proyeccion(widget,settings->prestype);
	}
      break;
    case IMAGES_NOSOUND:
      if(no_images>0)
	{
	  printf("INFO: ejecutamos imagenes sin sonido \n");
	  show_proyeccion(widget,settings->prestype);
	}
      break; 
    default:
      if(no_videos>0)
	{
	  printf("INFO: ejecutamos videos \n");
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

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("Maimonides_logo_maimo.png", NULL);

  GtkWidget *dialog = gtk_about_dialog_new();

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "ProMaimónides v.1.0.11");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), appversion); 
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), 
      "(c) Pro Integra S.L.");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
     "Maimo es el programa configurador de la suit ProMaimónides");
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
init_list(GtkWidget *list)
{

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("List Items",
          renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list), 
      GTK_TREE_MODEL(store));

  g_object_unref(store);
}

void clear_textview (GtkTextBuffer *buffer)
{
    GtkTextIter start, end;

  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  gtk_text_buffer_delete(buffer, &start, &end);
}
void  subir_video_lista(GtkWidget *widget, gpointer selection)
{
  gint k;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  struct pista video_temp;


  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(selection), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);

    //mejorar: leeriamos el xml modificado!
    cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

    if(no_videos>0)
      {
	for(k=1;k<no_videos;k++)
	  {
	    if(!g_strcmp0(videos_index[k].name,value))
	      {

		g_stpcpy(video_temp.name,videos_index[k-1].name);
		g_stpcpy(videos_index[k-1].name,videos_index[k].name);
		g_stpcpy(videos_index[k].name,video_temp.name);
		
		cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
		refresh_tables();
	      }
	  }
      }    
    g_free(value);
  }
  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
   if(settings != NULL)
    free(settings);
}
void  bajar_video_lista(GtkWidget *widget, gpointer selection)
{
   gint k;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  struct pista video_temp;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(selection), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);

    //mejorar: leeriamos el xml modificado!
    cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

    if(no_videos>0)
      {
	for(k=0;k<no_videos-1;k++)
	  {
	    if(!g_strcmp0(videos_index[k].name,value))
	      {		     
		g_stpcpy(video_temp.name,videos_index[k+1].name);
		g_stpcpy(videos_index[k+1].name,videos_index[k].name);
		g_stpcpy(videos_index[k].name,video_temp.name);

		cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
		refresh_tables();
		//está bien usar el break?¿?
		break;
	      }
	  }
      }    
    g_free(value);
  }
  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
   if(settings != NULL)
    free(settings);
}
void  subir_musica_lista(GtkWidget *widget, gpointer selection)
{
  gint k;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  struct pista audio_temp;


  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(selection), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);

    //mejorar: leeriamos el xml modificado!
    cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

    if(no_pistas>0)
      {
	for(k=1;k<no_pistas;k++)
	  {
	    if(!g_strcmp0(audios_index[k].name,value))
	      {

		g_stpcpy(audio_temp.name,audios_index[k-1].name);
		g_stpcpy(audios_index[k-1].name,audios_index[k].name);
		g_stpcpy(audios_index[k].name,audio_temp.name);
		
		cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
		refresh_tables();
	      }
	  }
      }    
    g_free(value);
  }
  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
   if(settings != NULL)
    free(settings);
}
void  bajar_musica_lista(GtkWidget *widget, gpointer selection)
{
   gint k;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  struct pista audio_temp;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(selection), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);

    //mejorar: leeriamos el xml modificado!
    cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

    if(no_pistas>0)
      {
	for(k=0;k<no_pistas-1;k++)
	  {
	    if(!g_strcmp0(audios_index[k].name,value))
	      {		     
		g_stpcpy(audio_temp.name,audios_index[k+1].name);
		g_stpcpy(audios_index[k+1].name,audios_index[k].name);
		g_stpcpy(audios_index[k].name,audio_temp.name);

		cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
		refresh_tables();
		//está bien usar el break?¿?
		break;
	      }
	  }
      }    
    g_free(value);
  }
  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
   if(settings != NULL)
    free(settings);
}
void  subir_imagen_lista(GtkWidget *widget, gpointer selection)
{
  gint k;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  struct imagen image_temp;


  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(selection), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);

    //mejorar: leeriamos el xml modificado!
    cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

    if(no_images>0)
      {
	for(k=1;k<no_images;k++)
	  {
	    if(!g_strcmp0(image_index[k].name,value))
	      {

		g_stpcpy(image_temp.name,image_index[k-1].name);
		image_temp.seconds=image_index[k-1].seconds;
		//la transición iría aquí
		g_stpcpy(image_index[k-1].name,image_index[k].name);
		image_index[k-1].seconds=image_index[k].seconds;
		//la transición iría aquí
		g_stpcpy(image_index[k].name,image_temp.name);
		image_index[k].seconds=image_temp.seconds;
		
		cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
		refresh_tables();
	      }
	  }
      }
    
    g_free(value);
  }

  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
   if(settings != NULL)
    free(settings);


}
void  bajar_imagen_lista(GtkWidget *widget, gpointer selection)
{
   gint k;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  struct imagen image_temp;


  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(selection), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);

    //mejorar: leeriamos el xml modificado!
    cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

    if(no_images>0)
      {
	for(k=(no_images-2);k>=0;k--) //otra forma de hacerlo sin usar el break
	  {
	    if(!g_strcmp0(image_index[k].name,value))
	      {
		     
		g_stpcpy(image_temp.name,image_index[k+1].name);
		image_temp.seconds=image_index[k+1].seconds;
		//la transición iría aquí
		g_stpcpy(image_index[k+1].name,image_index[k].name);
		image_index[k+1].seconds=image_index[k].seconds;
		//la transición iría aquí
		g_stpcpy(image_index[k].name,image_temp.name);
		image_index[k].seconds=image_temp.seconds;


		cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
		refresh_tables();	
	      }
	  }
      }
    
    g_free(value);
  }

  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
   if(settings != NULL)
    free(settings);


}


static void  on_enttiempo_changed(GtkWidget *entrytext,gpointer selection)
{
  gint k,i;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value,*imputtext;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
 
  printf("DEBUG: Evento de isnerción de TEXTO\n");
  if (gtk_tree_selection_get_selected(
				      GTK_TREE_SELECTION(selection), &model, &iter)) 
    {
       printf("DEBUG: Hay seleccionado algo de la tabla\n");
      gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);

      //mejorar: leeriamos el xml modificado!
      cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

  
      for(k=0;k<no_images;k++)
	{
	  if(!g_strcmp0(image_index[k].name,value))
	    {
	      printf("DEBUG: Tenemos la concordancia\n");
	      imputtext = gtk_entry_get_text (entrytext);
	      for(i=0;i<strlen(imputtext);i++)
		{
		  printf("DEBUG: Evaluamos si es dígito\n");
		 if(!isdigit(imputtext[i]))

		    return;    
		}
	      
	      printf("DEBUG: Actualizamos \n");
	      image_index[k].seconds=atoi(imputtext);
	      
	      cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
	      //g_free(imputtext);	   
	   }
	}
      g_free(value);
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
void  on_changed(GtkWidget *widget, gpointer entrytext) 
{
  gint k;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
 

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);

    //mejorar: leeriamos el xml modificado!
    cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

  
    for(k=0;k<no_images;k++)
      {
	if(!g_strcmp0(image_index[k].name,value))
	  {
	    sprintf(value ,"%d",image_index[k].seconds);	    
	  }
      }
      
    gtk_entry_set_text(entrytext,value);
    g_free(value);
  }

  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
   if(settings != NULL)
    free(settings);
}

void  on_combochanged(GtkWidget *widget) 
{
 
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  
  cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);



  switch( gtk_combo_box_get_active (combo_chooseplaying))
    {
    case(0):
      settings->prestype = IMAGES_SOUND;
      cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
      break;
    case(1):
      settings->prestype = IMAGES_NOSOUND;
      cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
      break;
    case(2):
      settings->prestype = VIDEOS_SOUND;
      cargar_recursos_biblioteca (SAVEXML_MODE,&image_index,&audios_index,&videos_index,&settings);
      break;
    default:
      g_printf("AVISO: no se ha seleccioando un modo de presentación \n");
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
void cargar_desde_usb()
{
  load_biblioteca(GINT_TO_POINTER(FUSB_MODE));
  //guarda_usb();
}
void salvar_en_usb()
{
  load_biblioteca(GINT_TO_POINTER(TUSB_MODE));
  //guarda_usb();
}
void carga_local (GtkWidget *widget)
{
  
  gint mode;
  
  g_printf("DEBUG: Iniciando carga de los archivos locales \n");
  //inicilizacion
  //cargamos imagenes en consecuencia y las tratamos
  //cargamos videos en consecuencia y los tratamos
  //cargamos audios en consecuencia y los tratamos
  //guardamos XML local
  g_printf("DEBUG: loading dialog\n");
  mode=FILES_MODE;
  //G_LOCK(thread_locked);
  load_biblioteca(GINT_TO_POINTER(mode));
  return;
}
//Initialization
void init_main ()
{

  refresh_tables();
  refresh_prestypebox(NULL);

  return;
}
gboolean refresh_prestypebox()
{

  //refrescamos según el XML seleccionado
  struct imagen* image_index = NULL;
  struct pista* audios_index = NULL,* videos_index = NULL;
  struct glob_settings* settings = NULL;
  
  cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

  switch(settings->prestype)
    {
    case(IMAGES_SOUND):
      gtk_combo_box_set_active (combo_chooseplaying,0);
      break;
    case(IMAGES_NOSOUND):
      gtk_combo_box_set_active (combo_chooseplaying,1);
      break;
    case(VIDEOS_SOUND):
      gtk_combo_box_set_active (combo_chooseplaying,2);
      break;
    default:
      gtk_combo_box_set_active (combo_chooseplaying,-1);
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
  return TRUE;

}
gboolean refresh_tables()
{
  //refrescamos según el XML seleccionado
     
  actualize_imalist ();

  actualize_vidlist ();
 
  actualize_audlist ();
 
  return TRUE;
}
static void actualize_imalist ()
{
 int i;
 GtkListStore *store;
 GtkTreeIter  iter;
 //refrescamos según el XML seleccionado
 struct imagen* image_index = NULL;
 struct pista* audios_index = NULL,* videos_index = NULL;
 struct glob_settings* settings = NULL;

 g_printf("DEBUG: refresco de la lista de imágenes \n");
 cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);
 
 store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(ima_list)));
 gtk_list_store_clear(store);
 //si verdaderamente hay imágenes actuamos
 if(no_images >0)
   {
     for(i=0;i<no_images;i++)
       { 		
	 gtk_list_store_append(store, &iter);	 
	 gtk_list_store_set(store, &iter, LIST_ITEM, image_index[i].name, -1);	
       }
     if(image_index != NULL)
       free(image_index);
   }
  if(settings != NULL)
    free(settings);
  if(no_pistas >0)
    {
      if(audios_index != NULL)
	free(audios_index);
    }
  if(no_videos)
    {
      if(videos_index != NULL)
	free(videos_index);
    }
  
} 
static void actualize_vidlist ()
{
 int i;
 GtkListStore *store;
 GtkTreeIter  iter;
 //refrescamos según el XML seleccionado
 struct imagen* image_index = NULL;
 struct pista* audios_index = NULL,* videos_index = NULL;
 struct glob_settings* settings = NULL;
  
 cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);
 
 g_printf("DEBUG: refresco de la lista de videos \n");
 store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(vid_list)));
 gtk_list_store_clear(store);

   //si verdaderamente hay videos actuamos
 if(no_videos >0)
   {
     for(i=0;i<no_videos;i++)
       { 		
	 gtk_list_store_append(store, &iter);	 
	 gtk_list_store_set(store, &iter, LIST_ITEM, videos_index[i].name, -1);
       }
     
   }
  if(settings != NULL)
    free(settings);
  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
} 
static void actualize_audlist ()
{
int i;
 GtkListStore *store;
 GtkTreeIter  iter;
 //refrescamos según el XML seleccionado
 struct imagen* image_index = NULL;
 struct pista* audios_index = NULL,* videos_index = NULL;
 struct glob_settings* settings = NULL;
  
 cargar_recursos_biblioteca (XML_MODE,&image_index,&audios_index,&videos_index,&settings);

 g_printf("DEBUG: refresco de la lista de audios \n");
 store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(mus_list)));
 gtk_list_store_clear(store);
  
  //si verdaderamente hay canciones actuamos
 if(no_pistas >0)
   {
     for(i=0;i<no_pistas;i++)
       { 		
	 gtk_list_store_append(store, &iter);	 
	 gtk_list_store_set(store, &iter, LIST_ITEM, audios_index[i].name, -1);
       }
   }
 if(settings != NULL)
    free(settings);
  if(audios_index != NULL)
    free(audios_index);
  if(videos_index != NULL)
    free(videos_index);
  if(image_index != NULL)
    free(image_index);
} 
int main_win (int init_mode)
{
  GtkWidget *window; 
  GtkWidget *table;
  GtkWidget *main_vbox; 
  GtkWidget *main_hbox1,*main_hbox2,*main_hbox3,*hbox_cf_tipo_pres;
  GtkWidget *halign,*halig_cf_tipo_pres,*vhalign_lists,*valign_left,*halign1,*halign2,*halign3;
  GtkWidget *titlehalign,*titlehalign2,*titlehalign3;
  GtkWidget* scrolledwindow; 

  GtkWidget *btcarga,*btguarda,*btcargaUSB,*btplay,*bthelp;
  //GtkLabel *lbconfiguracion;
  GtkWidget *frconfiguracion;   
  
  //del antiguo config.c
  GtkWidget *sw;
  GtkWidget *sw2;
  GtkWidget *sw3;
  GtkWidget *title,*title2,*title3;
  GtkWidget *icon;

  GtkWidget *remove;
  GtkWidget *add;
  GtkWidget *up;
  GtkWidget *down;
  GtkWidget *labtiempo1;
  GtkWidget *enttiempo1;
  GtkTreeSelection *selection; 

  GtkWidget *remove2;
  GtkWidget *add2;
  GtkWidget *up2;
  GtkWidget *down2;
  // GtkWidget *labtiempo2;
  //GtkWidget *enttiempo2;
  GtkTreeSelection *selection2; 

  GtkWidget *remove3;
  GtkWidget *add3;
  GtkWidget *up3;
  GtkWidget *down3;
  //GtkWidget *labtiempo3;
  //GtkWidget *enttiempo3;
  GtkTreeSelection *selection3; 
  GtkWidget *vtotalbox;
  GtkWidget *totalbox;
  GtkWidget *htotalbox;
  GtkWidget *vbox_1;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *hbox1_2;
  GtkWidget *hbox1_3;
  GtkWidget *hbox1_4;
  GtkWidget *vbox2_1;
  GtkWidget *vbox2;
  GtkWidget *hbox2;
  GtkWidget *hbox2_2;
  GtkWidget *hbox2_3;
  GtkWidget *hbox2_4;
  GtkWidget *vbox3_1;
  GtkWidget *vbox3;
  GtkWidget *hbox3;
  GtkWidget *hbox3_2;
  GtkWidget *hbox3_3;
  GtkWidget *hbox3_4;


  //////////////////

  int mode = init_mode;


  /* Initialize our data structure */
  // memset (&data, 0, sizeof (data));

  gtk_init(NULL, NULL);
  //window options
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
  gtk_window_set_title(GTK_WINDOW(window), "Maimo");
  gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("Maimonides_logo_maimo.png"));
  //initializing layouts
  //vtotalbox =  gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  totalbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  htotalbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  main_hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  main_hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  main_hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add(GTK_CONTAINER(window), main_vbox);
  //buttons
  btcargaUSB = gtk_button_new_with_label("Transferir desde USB");
  btcarga = gtk_button_new_with_label("Carga local");
  btguarda = gtk_button_new_with_label("Transferir a USB");
  btplay = gtk_button_new_with_label("Prueba");
  bthelp = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_DIALOG_INFO,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (bthelp), icon);
  gtk_box_pack_start(GTK_BOX(main_hbox1),btguarda , FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(main_hbox1),btcargaUSB , FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(main_hbox1),btcarga , FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(main_hbox1),btplay , FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(main_hbox1),bthelp , FALSE, FALSE, 0);
  //buttons  align
  halign = gtk_alignment_new(0.5, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), main_hbox1);
  //configuration
  //configuration frame   
  frconfiguracion = gtk_frame_new("Configuración");
  gtk_frame_set_shadow_type(GTK_FRAME(frconfiguracion), GTK_SHADOW_IN);
  gtk_box_pack_start(GTK_BOX(main_hbox2),frconfiguracion , TRUE, TRUE, 0);
  //configuration text
  //combobox
  hbox_cf_tipo_pres = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  combo_chooseplaying = gtk_combo_box_text_new();
  gtk_combo_box_text_append(combo_chooseplaying,NULL,"imágenes CON sonido");
  gtk_combo_box_text_append(combo_chooseplaying,NULL,"imágenes SIN sonido");
  gtk_combo_box_text_append(combo_chooseplaying,NULL,"videos");

  halig_cf_tipo_pres = gtk_alignment_new(0.5, 0, 0, 0);
  gtk_box_pack_start(GTK_BOX(hbox_cf_tipo_pres),combo_chooseplaying , FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER( halig_cf_tipo_pres),hbox_cf_tipo_pres);
  // gtk_container_add (GTK_CONTAINER (frconfiguracion),halig_cf_tipo_pres ); esto lo hacemos al final, agregándolo a totalbox
  //nuestras 3 listas
  table = gtk_table_new(7,7,FALSE);
  gtk_table_set_col_spacings(GTK_TABLE(table), 3);

  title = gtk_label_new("Imágenes");
  titlehalign = gtk_alignment_new(0.5, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(titlehalign), title);
  gtk_table_attach(GTK_TABLE(table), titlehalign, 1, 2, 0, 1, 
		   GTK_FILL, GTK_FILL, 0, 0);
  title2 = gtk_label_new("Música");
  titlehalign2 = gtk_alignment_new(0.5, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(titlehalign2), title2);
  gtk_table_attach(GTK_TABLE(table), titlehalign2,3, 4, 0, 1, 
		   GTK_FILL, GTK_FILL, 0, 0);
  title3 = gtk_label_new("Videos");
  titlehalign3 = gtk_alignment_new(0.5, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(titlehalign3), title3);
  gtk_table_attach(GTK_TABLE(table), titlehalign3, 5, 6, 0, 1, 
		   GTK_FILL, GTK_FILL, 0, 0);
  
  sw = gtk_scrolled_window_new(NULL, NULL);
  sw2 = gtk_scrolled_window_new(NULL, NULL);
  sw3 = gtk_scrolled_window_new(NULL, NULL);
  mus_list = gtk_tree_view_new();
  vid_list = gtk_tree_view_new();
  ima_list = gtk_tree_view_new();
  //SCROLLS
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw2),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw3),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
				       GTK_SHADOW_ETCHED_IN);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw2),
				       GTK_SHADOW_ETCHED_IN);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw3),
				       GTK_SHADOW_ETCHED_IN);

  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (ima_list), FALSE);
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (mus_list), FALSE);
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (vid_list), FALSE);
  /* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  vbox3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);*/
  vbox_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  vbox2_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  vbox3_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  /*  
  gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), sw2, TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(vbox), sw3, TRUE, TRUE, 5);*/
  gtk_table_attach(GTK_TABLE(table), sw, 1, 2, 1, 4, 
		   GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);
  gtk_table_attach(GTK_TABLE(table), sw2, 3, 4, 1, 4, 
		   GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);
  gtk_table_attach(GTK_TABLE(table), sw3, 5, 6, 1, 4, 
		   GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 0, 0);

  //alineadores

  hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  /*
  hbox1_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox2_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox3_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox1_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox1_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox2_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox2_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox3_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  hbox3_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  */
  //DATOS DE ELEMENTO
  labtiempo1 = gtk_label_new("Tiempo(s):");
  enttiempo1 = gtk_entry_new();
  gtk_entry_set_width_chars (enttiempo1,2);
  gtk_entry_set_max_length (enttiempo1,2);
  gtk_table_attach(GTK_TABLE(table), labtiempo1, 0, 1, 1, 2, 
      GTK_FILL, GTK_SHRINK, 1, 1);
  valign_left = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(valign_left), enttiempo1);
  gtk_table_set_row_spacing(GTK_TABLE(table), 1, 3);
  gtk_table_attach(GTK_TABLE(table), valign_left, 0, 1, 2, 3, 
      GTK_FILL, GTK_FILL | GTK_EXPAND, 1, 1);

  //BOTONES DE ACCION

  add = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_ADD,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (add), icon);
  remove = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_CANCEL,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (remove), icon);
  up = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_UP,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (up), icon);
  down = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_DOWN,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (down), icon);

  gtk_table_set_row_spacing(GTK_TABLE(table), 3, 6);
  gtk_box_pack_start(GTK_BOX(hbox), add, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), remove, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), up, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), down, TRUE, TRUE, 0);

  gtk_table_attach(GTK_TABLE(table), hbox, 1, 2, 5, 6, 
      GTK_FILL, GTK_FILL, 0, 0);

  /*
  gtk_container_add (GTK_CONTAINER (add), icon);
  remove = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_CANCEL,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (remove), icon);
  up = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_UP,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (up), icon);
  down = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_DOWN,
  GTK_ICON_SIZE_LARGE_TOOLBAR);*/
  /*
  gtk_container_add (GTK_CONTAINER (down), icon);
  gtk_box_pack_start(GTK_BOX(hbox), add, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox), remove, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox), up, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox), down, FALSE, TRUE, 3);
  //gtk_box_pack_start(GTK_BOX(hbox1_2), reload, FALSE, TRUE, 3);

  gtk_box_pack_start(GTK_BOX(hbox1_3), labtiempo1, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox1_4), enttiempo1, FALSE, TRUE, 3);
  */

  //DATOS DE ELEMENTO
  //labtiempo2 = gtk_label_new("Tiempo(s):");
  //enttiempo2 = gtk_entry_new();
  //gtk_entry_set_width_chars (enttiempo2,2);
  //gtk_entry_set_max_length (enttiempo2,2);
  //BOTONES DE ACCION
 add2 = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_ADD,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (add2), icon);
  remove2 = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_CANCEL,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (remove2), icon);
  up2 = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_UP,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (up2), icon);
  down2 = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_DOWN,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (down2), icon);

  gtk_table_set_row_spacing(GTK_TABLE(table), 3, 6);
  gtk_box_pack_start(GTK_BOX(hbox2), add2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox2), remove2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox2), up2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox2), down2, TRUE, TRUE, 0);

  gtk_table_attach(GTK_TABLE(table), hbox2, 3, 4, 5, 6, 
      GTK_FILL, GTK_FILL, 0, 0);
  /*
  add2 = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_ADD,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (add2), icon);
  remove2 = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_CANCEL,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (remove2), icon);
  up2 = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_UP,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (up2), icon);
  down2 = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_DOWN,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (down2), icon);

  gtk_box_pack_start(GTK_BOX(hbox2), add2, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox2), remove2, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox2), up2, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox2), down2, FALSE, TRUE, 3);
  */
  // gtk_box_pack_start(GTK_BOX(hbox2_3), labtiempo2, FALSE, TRUE, 3);
  //gtk_box_pack_start(GTK_BOX(hbox2_4), enttiempo2, FALSE, TRUE, 3);
  //DATOS DE ELEMENTO
  //labtiempo3 = gtk_label_new("Tiempo(s):");
  //enttiempo3 = gtk_entry_new();
  //gtk_entry_set_width_chars (enttiempo3,2);
  //gtk_entry_set_max_length (enttiempo3,2);
  //BOTONES DE ACCION
  add3 = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_ADD,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (add3), icon);
  remove3 = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_CANCEL,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (remove3), icon);
  up3 = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_UP,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (up3), icon);
  down3 = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_DOWN,
				   GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (down3), icon);

  gtk_table_set_row_spacing(GTK_TABLE(table), 3, 6);
  gtk_box_pack_start(GTK_BOX(hbox3), add3, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox3), remove3, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox3), up3, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox3), down3, TRUE, TRUE, 0);

  gtk_table_attach(GTK_TABLE(table), hbox3, 5, 6, 5, 6, 
		   GTK_FILL, GTK_FILL, 0, 0);
  /*
  add3 = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_ADD,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (add3), icon);
  remove3 = gtk_button_new();
  icon = gtk_image_new_from_stock (GTK_STOCK_CANCEL,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (remove3), icon);
  up3 = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_UP,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (up3), icon);
  down3 = gtk_button_new(); 
  icon = gtk_image_new_from_stock (GTK_STOCK_GO_DOWN,
				    GTK_ICON_SIZE_LARGE_TOOLBAR);
  gtk_container_add (GTK_CONTAINER (down3), icon);
  //reload3 = gtk_button_new_with_label("Recargar desde /videos");

  gtk_box_pack_start(GTK_BOX(hbox3), add3, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox3), remove3, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox3), up3, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(hbox3), down3, FALSE, TRUE, 3);
  */
  //gtk_box_pack_start(GTK_BOX(hbox3_2), reload3, FALSE, TRUE, 3);

  //gtk_box_pack_start(GTK_BOX(hbox3_3), labtiempo3, FALSE, TRUE, 3);
  //gtk_box_pack_start(GTK_BOX(hbox3_4), enttiempo3, FALSE, TRUE, 3);
  /*
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox2), hbox2, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox3), hbox3, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox), hbox1_2, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox3), hbox3_2, FALSE, TRUE, 3);

  gtk_box_pack_start(GTK_BOX(vbox_1), hbox1_3, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox_1), hbox1_4, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox2_1), hbox2_3,FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox2_1), hbox2_4, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox3_1), hbox3_3, FALSE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox3_1), hbox3_4, FALSE, TRUE, 3);
  */
  gtk_container_add(GTK_CONTAINER (sw), ima_list);
  gtk_container_add(GTK_CONTAINER (sw2), mus_list);
  gtk_container_add(GTK_CONTAINER (sw3), vid_list);
  /*
  gtk_box_pack_start(GTK_BOX(htotalbox), vbox_1, TRUE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(htotalbox), vbox, TRUE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(htotalbox), vbox2_1, TRUE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(htotalbox), vbox, TRUE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(htotalbox), vbox3_1, TRUE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(htotalbox), vbox, TRUE, TRUE, 3);
  */
  gtk_box_pack_start(GTK_BOX(htotalbox), table, TRUE, TRUE, 3);

  gtk_box_pack_start(GTK_BOX(totalbox), halig_cf_tipo_pres, FALSE, TRUE, 3); //combobox
  gtk_box_pack_start(GTK_BOX(totalbox),htotalbox, TRUE, TRUE, 3);
  ///////////////////////
  // gtk_box_pack_start(GTK_BOX(vtotalbox),halig_cf_tipo_pres , TRUE, TRUE, 3); //combobox
  // gtk_box_pack_start(GTK_BOX(vtotalbox),totalbox , TRUE, TRUE, 3); //el resto de listas y botones
  gtk_container_add (GTK_CONTAINER (frconfiguracion), totalbox ); //añadimos al frame
  //window managing
  gtk_box_pack_start(GTK_BOX(main_vbox),halign, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox2 , TRUE, TRUE, 0);
 

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
			   G_CALLBACK(gtk_main_quit), G_OBJECT(window));
  //lists init
  init_list(ima_list);
  init_list(mus_list);
  init_list(vid_list);
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ima_list)); 
  selection2 = gtk_tree_view_get_selection(GTK_TREE_VIEW(mus_list)); 
  selection3 = gtk_tree_view_get_selection(GTK_TREE_VIEW(vid_list)); 
  //buttons signals
  //si cambia la seleccion de la lista de imagenes vemos sus datos en tiempo real (tiempo y transición)
  g_signal_connect(selection, "changed", 
      G_CALLBACK(on_changed), enttiempo1);
  g_signal_connect(G_OBJECT(enttiempo1), "activate", 
      G_CALLBACK(on_enttiempo_changed), selection);
  g_signal_connect(G_OBJECT(combo_chooseplaying), "changed", 
      G_CALLBACK(on_combochanged), NULL);
  g_signal_connect(G_OBJECT(btcargaUSB), "clicked",
		   G_CALLBACK(cargar_desde_usb), NULL);
  g_signal_connect(G_OBJECT(btcarga), "clicked",
		   G_CALLBACK(carga_local), NULL);
   g_signal_connect(G_OBJECT(btguarda), "clicked",
		   G_CALLBACK(salvar_en_usb), NULL);
   g_signal_connect(G_OBJECT(btplay), "clicked",
		   G_CALLBACK(start_show), NULL);
  g_signal_connect(G_OBJECT(bthelp), "clicked",
		   G_CALLBACK(show_about), window);
  //señales de los botones de las listas
  g_signal_connect(G_OBJECT(up), "clicked",
		   G_CALLBACK(subir_imagen_lista),selection);
  g_signal_connect(G_OBJECT(down), "clicked",
		   G_CALLBACK(bajar_imagen_lista),selection);
  g_signal_connect(G_OBJECT(up2), "clicked",
		   G_CALLBACK(subir_musica_lista),selection2);
  g_signal_connect(G_OBJECT(down2), "clicked",
		   G_CALLBACK(bajar_musica_lista),selection2);
  g_signal_connect(G_OBJECT(up3), "clicked",
		   G_CALLBACK(subir_video_lista),selection3);
  g_signal_connect(G_OBJECT(down3), "clicked",
		   G_CALLBACK(bajar_video_lista),selection3);

  //  g_timeout_add_seconds(10, (GSourceFunc)refresh_text,buffer);

  gtk_widget_show_all(window);
  init_main ();
  gtk_main();

  return 0;

}
