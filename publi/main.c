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
#include <unistd.h>
#include <dirent.h>

#include "../common/globales.h"
#include "../common/pi_xml_parser.h"
//PUBLICISTA 

int main (int argc, char * argv[])
{
  /* int i;
  int ret;
  struct imagen* image_index;
  struct pista* audios_index, *videos_index;

  //xml config creation/openning*/
  printf("INFO: starting\n");
  /*  printf("INFO: carga USB\n");
  ret = carga_usb(); 
  printf("DEBUG: loading usb returns %d \n",ret);
  //cargar_recursos_biblioteca(XML_MODE);
  printf("INFO: lectura del archivo XML \n");
  cargar_XML_biblioteca(XML_MODE,&image_index,&audios_index,&videos_index);
  */
  iDebug = 0;
  mode = IMAGES_SOUND;
  main_win(IMAGES_SOUND);
  /*
  if(image_index!=NULL)
    free(image_index);
  if(audios_index!=NULL)
    free(audios_index);
  if(videos_index!=NULL)
  free(videos_index);*/
  xmlCleanupParser();
  return 0;
}


