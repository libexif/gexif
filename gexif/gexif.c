/* gexif.c
 *
 * Copyright (C) 2001 Lutz Müller <lutz@users.sourceforge.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details. 
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>

#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>

#include "gexif-main.h"

int
main (int argc, char **argv)
{
	GtkWidget *window;

	gtk_init (&argc, &argv);
	g_log_set_always_fatal (G_LOG_LEVEL_CRITICAL);

	window = gexif_main_new ();
	gtk_widget_show (window);
	g_signal_connect (G_OBJECT (window), "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);
	if (argc > 1)
		gexif_main_load_file (GEXIF_MAIN (window), argv[1]);

	gtk_main ();

	return (0);
}
