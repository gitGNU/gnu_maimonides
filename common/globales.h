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
#include <string.h>
#include <gst/interfaces/xoverlay.h>

#ifndef globales
#define globales 

//#define PUBLICISTA
//#define CONFIGURADOR

//VERSIÓN DE LA APLICACIÓN

static const gchar* appversion = "v0.0.1 rev19/12/2012";
static const gchar* usbpath = "/media/PROINTEGRA";
#ifdef CONFIGURADOR
static const gchar* appname = "Maimo";
#else
static const gchar* appname = "Nides";
#endif
int i,seconds;
int binicializado;
int mode;

/* Structure to contain all our information, so we can pass it around */

typedef struct _CustomData {
  GstElement *playbin2;           /* Our one and only pipeline */ 
  GstElement *sink;
  guintptr window_handle;
  GstState state;                 /* Current state of the pipeline */
  gint64 duration;                /* Duration of the clip, in nanoseconds */
} CustomData;

typedef struct imagen{ //estructura de una imágen
  gchar name[128];
  int  seconds;
  int  transition;
}strImagen;

typedef struct pista{ //estructura de una imágen
  gchar name[128];
}strPista;

typedef struct glob_settings{ //estructura de la configuración global
  gint prestype;
}strSettingsPista;

enum PROCESS_CODE            /* codigos de procesoDefines an enumeration type    */
{
  ALL_OK = 1,       /* Names day and declares a       */
  NO_IMAGES = -10,     /* variable named workday with    */ 
  NO_VIDEOS = -11,
};
enum LOADING_MODE            /* codigos de procesoDefines an enumeration type    */
{
  FILES_MODE = 0,     
  XML_MODE = 1,
  TUSB_MODE = 2,
  FUSB_MODE = 3,
  SAVEXML_MODE = 4,
};
enum PROYECCION_MODE           /* codigos de procesoDefines an enumeration type    */
{
  IMAGES_SOUND = 1,       /* Names day and declares a       */
  IMAGES_NOSOUND = 10,     /* variable named workday with    */ 
  VIDEOS_SOUND = 20,
};
int iDebug;
int no_images,no_pistas,no_videos;
int actual_audio_index,actual_video_index;

int playing; 


#endif
