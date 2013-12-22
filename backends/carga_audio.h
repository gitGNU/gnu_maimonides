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
#include <sys/types.h>
#include <dirent.h>

#include "../common/globales.h"

#ifndef _h_carga_audios
#define _h_carga_audios

int carga_audios_from_local (struct pista** audios_index);
int carga_audios_from_XML (struct pista** audios_index);
int carga_audio (int MODE,struct pista** audios_index);
int filtro_pistas(strPista** tracks_to_analyze,gchar* ruta);//filtrado de pistas (solo OGG y MP3 por ahora)
int ismp3(char* name);
int isogg(char* name);
#endif


