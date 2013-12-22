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
#ifdef WIN32
#include <windows.h>
#endif

//CONFIGURADOR
#include "../common/globales.h"
#include "../common/pi_xml_parser.h"


int main (int argc, char * argv[])
{

  // int ret;

  //xml config creation/openning
  printf("INFO: starting\n");
  
  iDebug = 0;
  mode = IMAGES_SOUND;
  main_win(IMAGES_SOUND);

  xmlCleanupParser();
  return 0;
}



