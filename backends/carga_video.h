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

#ifndef _h_carga_videos
#define _h_carga_videos

static int carga_videos_from_local (struct pista** videos_index);
static int carga_videos_from_XML (struct pista** videos_index);
 int carga_video (int MODE,struct pista** videos_index);
static int filtro_videos(strPista** tracks_to_analyze,gchar* ruta);//filtrado de pistas (solo OGG y MP3 por ahora)
static int isavi(char* name);

#endif


