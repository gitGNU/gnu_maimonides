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


#ifndef carga
#define carga 
struct strProgressBarData
{
  int mode;
  gpointer *win;
  struct pista * audios_index;
  struct imagen *image_index;
  struct pista * videos_index;
}DATA_progressbar;

static gboolean destroy_win(gpointer data);
static gboolean load_done(gpointer data);
gboolean update_progress(gpointer data);
gpointer call_fscripts_FILES (gpointer data);
gpointer call_fscripts_FUSB (gpointer data);
gpointer call_fscripts_TUSB (gpointer data);

int load_biblioteca(gpointer mode);




#endif
