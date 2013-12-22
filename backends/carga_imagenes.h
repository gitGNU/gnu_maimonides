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
#include <stdio.h>
#include <cairo.h>

#include "../common/globales.h"

#ifndef _h_carga_imagenes
#define _h_carga_imagenes
int isjpeg(char* name);
int topng_3(char* name);
int topng_4(char* name);
int isjpg(char* name);
int isgif(char* name);
int ispng(char* name);
int set_image_resolution(char* image_name,int width,int height);
int filtro_imagenes(strImagen** images_to_analyze, gchar* ruta);

int carga_imagenes_from_local (struct imagen** image_index);
int carga_imagenes_from_XML (struct imagen** image_index);
int carga_imagenes(int MODE,struct imagen** image_index);

#endif


