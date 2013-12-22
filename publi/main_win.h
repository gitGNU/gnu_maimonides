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
#ifndef main_window
#define main_window

static void start_show(GtkWidget *widget);
static void show_about(GtkWidget *widget, gpointer data);
static void mode_changed(GtkWidget *combobox, int* mode);
static GdkPixbuf *create_pixbuf(const gchar * filename);
void clear_textview ();
void actualize_textview ();
void refresh_text (GtkWidget *widget);

int main_win (int init_mode);

#endif
