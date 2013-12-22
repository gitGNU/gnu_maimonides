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
#include <glib/gprintf.h>
#include <cairo.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../common/globales.h"
#include "carga_audio.h"

int ismp3(char* name)
{
  int length;
  char end1[] = ".mp3";
  char end2[] = ".MP3";
  char* end[4];
 
  length = strlen(name);
  
  strcpy(end,&name[length-4]);

  //printf("comparando : %s con %s y %s \n",end,end1,end2);
  if(!(strcmp(end,end1) &&(strcmp(end,end2))))
    return 1;
  else
    return 0;
  
}
int isogg(char* name)
{
 int length;
  char end1[] = ".ogg";
  char end2[] = ".OGG";
  char* end[4];
 
  length = strlen(name);
  
  strcpy(end,&name[length-4]);

  //printf("comparando : %s con %s y %s \n",end,end1,end2);
  if(!(strcmp(end,end1) &&(strcmp(end,end2))))
    return 1;
  else
    return 0;
}
int filtro_pistas(strPista** tracks_to_analyze, gchar* ruta)
{
  int tracks=0;
  int i = 0,is = 0;
  struct dirent *de;
  strPista * pistas_temp = NULL;
  DIR *pd;

  pd = opendir(ruta);
  if(pd == NULL)
  {
    g_printf("ERROR: Couldn't open /musica directory \n"); //ERROR
    return(-1);
  }

  while(de = readdir(pd))
    {
      if(strcmp(de->d_name,".") && strcmp(de->d_name,".."))
	{
	  is = ismp3(de->d_name);
	  is = is + isogg(de->d_name);
	  if(is) //is valid
	    {
	      tracks++;
	    }
	  is=0;
	}
    }
  //printf("tenemos %d canciones \n",tracks);
  pistas_temp  = malloc(tracks*sizeof(struct pista));
  closedir(pd);
  free(de);
  pd = opendir(ruta);
  is=0;
  while(de = readdir(pd))
    {
      if(strcmp(de->d_name,".") && strcmp(de->d_name,".."))
	{
	  is = ismp3(de->d_name);
	  is = is + isogg(de->d_name);
	  if(is) //is valid
	    {
	      strcpy( pistas_temp[i].name,de->d_name);
	      printf("audio loaded %s \n", pistas_temp[i].name);
	      i++;
	    }
	  is=0;
	}
    }
  *tracks_to_analyze = pistas_temp;
  closedir(pd);
  return tracks;
}
int carga_audios_from_local (struct pista** audios_index)
{
  struct pista * pt_audios = NULL;

  //adaptamos imágenes y las salvamos
  no_pistas = filtro_pistas(&pt_audios,"./musica");
  *audios_index = pt_audios;
  if(no_pistas > 0)
    return 1;
  else
    {   
      no_pistas = 0;
      return 0;
    }

}

int carga_audios_from_XML (struct pista** audios_index)
{
  int ret;
  struct imagen *image_index2 = NULL;
  struct pista *audios_index2 = NULL, *videos_index2 = NULL;

  audios_index2 = *audios_index;
  //audios_index2 = *audios_index; 
  //videos_index2 = *videos_index;
  
  ret = cargar_XML_biblioteca (XML_MODE, image_index2,audios_index2,videos_index2);  

  //no_pistas = filtro_imagenes(&image_index,"./imagenes"); //retorna -1 si algo va mal
  //printf("Hay %d numero de imágenes\n",no_images);

  *audios_index=audios_index2;
  if(image_index2!=NULL)
    free(image_index2);
  if(videos_index2!=NULL)
    free(videos_index2);

  if(no_pistas >0)
    return 1;
  else
    {   
      no_pistas = 0;
      return 0;
    }

}

int carga_audio (int MODE,struct pista** audios_index)
{

  int j,ret;
  struct imagen *image_index2 = NULL;
  struct pista *audios_index2 = NULL, *videos_index2 = NULL;

  audios_index2 = *audios_index;
  
  switch(MODE)
    {
    case(FILES_MODE):
      ret = carga_audios_from_local(&audios_index2);     
      break;
    case(XML_MODE):
      ret = carga_audios_from_XML(&audios_index2);
      break;
    default:
      return -1;
      break;
    }
  *audios_index = audios_index2;
  return ret;
}
