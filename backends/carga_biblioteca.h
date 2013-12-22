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

#ifndef _H_carga_biblioteca
#define _H_carga_biblioteca
int carga_usb() ;
int guarda_usb() ;
int return_app_path (gchar **path);
int carga_recursos_biblioteca (int MODE,struct imagen** image_index,struct pista** audio_index,struct pista** video_index,struct glob_settings** settings);
int cargar_XML_biblioteca (int MODE, struct imagen** image_index,struct pista** audio_index,struct pista** video_index,struct glob_settings** settings);

#endif


