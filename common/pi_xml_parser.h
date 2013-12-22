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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>

#include "globales.h"
#ifndef pi_parse_xml
#define pi_parse_xml



static void processNode(xmlTextReaderPtr reader, int *section,
			int *subsection,int* parameter,struct imagen** ,struct pista** ,struct pista**,struct glob_settings** );
int populateDoc(char *docname,struct imagen** image_index,struct pista** audios_index,struct pista** videos_index,struct glob_settings** settings);

int recoverDoc(char *docname,struct imagen **image_index,struct pista **audios_index,struct pista **videos_index,struct glob_settings** settings);
#endif
