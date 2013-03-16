/* gexif.c
 *
 * Copyright © 2001 Lutz Müller <lutz@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#include "config.h"
#include "gexif-main.h"

#include <string.h>
#include <gtk/gtk.h>

#ifdef ENABLE_NLS
#  include <locale.h>
#  include <langinfo.h>
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define setlocale(Category, Locale)
#  define nl_langinfo(Item) "UTF-8"
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bind_textdomain_codeset(Domain,String) (Domain)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

int
main (int argc, char **argv)
{
	GtkWidget *w;

	bindtextdomain (PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (PACKAGE, "UTF-8");
	textdomain (PACKAGE);

	gtk_init (&argc, &argv);

	/* There is no nice way to force libexif to provide its strings in
	 * UTF-8, which is what gtk+ expects. If the current locale uses
	 * some other encoding, try to set UTF-8 using the nonstandard hack
	 * of using "UTF-8" as a locale name.  If it doesn't work, oh well,
	 * we tried. This is really a problem that libexif-gtk should solve.
	 */
	if (strcmp(nl_langinfo(CODESET), "UTF-8"))
		setlocale(LC_CTYPE, "UTF-8");

	w = gexif_main_new ();

	g_signal_connect (G_OBJECT (w), "destroy",
			  G_CALLBACK (gtk_main_quit), NULL);

	gtk_widget_show (w);

	if (argc > 1)
		gexif_main_load_file (GEXIF_MAIN (w), argv[1]);

	gtk_main ();

	return (0);
}
