/* gexif-main.c
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
#include "gexif-main.h"

#include <string.h>

#include <gdk/gdkkeysyms.h>

#include <gtk/gtkmenuitem.h>
#include <gtk/gtkmenu.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkvbox.h>
#include <gtk/gtktooltips.h>
#include <gtk/gtkmenubar.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkfilesel.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtkitemfactory.h>
#include <gtk/gtkstock.h>

#include <libjpeg/jpeg-data.h>

#include <libexif-gtk/gtk-exif-browser.h>

#include "gexif-thumbnail.h"

#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

struct _GExifMainPrivate
{
	gchar *path;

	JPEGData *data;

	GtkExifBrowser *browser;
};

#define PARENT_TYPE GTK_TYPE_WINDOW
static GtkWindowClass *parent_class;

static void
gexif_main_destroy (GtkObject *object)
{
	GExifMain *m = GEXIF_MAIN (object);

	if (m->priv->data) {
		jpeg_data_unref (m->priv->data);
		m->priv->data = NULL;
	}

	if (m->priv->path) {
		g_free (m->priv->path);
		m->priv->path = NULL;
	}

	GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

static void
gexif_main_finalize (GObject *object)
{
	GExifMain *m = GEXIF_MAIN (object);

	g_free (m->priv);

	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gexif_main_class_init (gpointer g_class, gpointer class_data)
{
	GtkObjectClass *object_class;
	GObjectClass *gobject_class;

	object_class = GTK_OBJECT_CLASS (g_class);
	object_class->destroy  = gexif_main_destroy;

	gobject_class = G_OBJECT_CLASS (g_class);
	gobject_class->finalize = gexif_main_finalize;

	parent_class = g_type_class_peek_parent (g_class);
}

static void
gexif_main_init (GTypeInstance *instance, gpointer g_class)
{
	GExifMain *m = GEXIF_MAIN (instance);

	m->priv = g_new0 (GExifMainPrivate, 1);
}

GType
gexif_main_get_type (void)
{
	static GType t = 0;
	GTypeInfo ti;

	if (!t) {
		memset (&ti, 0, sizeof (GTypeInfo));
		ti.class_size     = sizeof (GExifMainClass);
		ti.class_init     = gexif_main_class_init;
		ti.instance_size  = sizeof (GExifMain);
		ti.instance_init  = gexif_main_init;

		t = g_type_register_static (PARENT_TYPE, "GExifMain", &ti, 0);
	}

	return (t);
}

static void
gexif_main_load_data (GExifMain *m, JPEGData *jdata)
{
	ExifData *edata;

	g_return_if_fail (GEXIF_IS_MAIN (m));
	g_return_if_fail (jdata != NULL);

	if (m->priv->data)
		jpeg_data_unref (m->priv->data);
	m->priv->data = jdata;
	jpeg_data_ref (jdata);

	edata = jpeg_data_get_exif_data (jdata);
	if (!edata) {
		g_warning ("No EXIF data found!");
		return;
	}

	gtk_exif_browser_set_data (m->priv->browser, edata);
	exif_data_unref (edata);
}

void
gexif_main_load_file (GExifMain *m, const gchar *path)
{
	JPEGData *data;

	g_return_if_fail (GEXIF_IS_MAIN (m));
	g_return_if_fail (path != NULL);

	if (m->priv->path)
		g_free (m->priv->path);
	m->priv->path = g_strdup (path);

	data = jpeg_data_new_from_file (path);
	gexif_main_load_data (m, data);
	jpeg_data_unref (data);
}

static void
gexif_main_save_file (GExifMain *m, const gchar *path)
{
	jpeg_data_save_file (m->priv->data, path);
}

static void
action_exit (gpointer callback_data, guint callback_action,
	     GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);

	gtk_object_destroy (GTK_OBJECT (m));
}

static void
action_save (gpointer callback_data, guint callback_action,
	     GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);

	gexif_main_save_file (m, m->priv->path);
}

static void
on_cancel_clicked (GtkButton *button, GExifMain *m)
{
	GtkWidget *fsel;

	fsel = gtk_widget_get_ancestor (GTK_WIDGET (button), GTK_TYPE_WINDOW);
	gtk_object_destroy (GTK_OBJECT (fsel));
}

static void
on_save_as_ok_clicked (GtkButton *button, GExifMain *m)
{
	GtkWidget *fsel;

	fsel = gtk_widget_get_ancestor (GTK_WIDGET (button),
					GTK_TYPE_FILE_SELECTION);
	gexif_main_save_file (m,
		gtk_file_selection_get_filename (GTK_FILE_SELECTION (fsel)));
	gtk_object_destroy (GTK_OBJECT (fsel));
}

static void
action_save_as (gpointer callback_data, guint callback_action,
		GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);
	GtkWidget *fsel;

	fsel = gtk_file_selection_new (_("Save As..."));
	gtk_widget_show (fsel);
	gtk_window_set_transient_for (GTK_WINDOW (fsel), GTK_WINDOW (m));
	g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (fsel)->ok_button),
			"clicked", G_CALLBACK (on_save_as_ok_clicked), m);
	g_signal_connect (
			GTK_OBJECT (GTK_FILE_SELECTION (fsel)->cancel_button),
			"clicked", G_CALLBACK (on_cancel_clicked), m);
	gtk_file_selection_set_filename (GTK_FILE_SELECTION (fsel),
					 m->priv->path);
}

static void
on_open_ok_clicked (GtkButton *button, GExifMain *m)
{
	GtkWidget *fsel;

	fsel = gtk_widget_get_ancestor (GTK_WIDGET (button),
					GTK_TYPE_FILE_SELECTION);
	gexif_main_load_file (m, 
		gtk_file_selection_get_filename (GTK_FILE_SELECTION (fsel)));
	gtk_object_destroy (GTK_OBJECT (fsel));
}

static void
action_open (gpointer callback_data, guint callback_action,
	     GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);
	GtkWidget *fsel;

	fsel = gtk_file_selection_new (_("Open..."));
	gtk_widget_show (fsel);
	gtk_window_set_transient_for (GTK_WINDOW (fsel), GTK_WINDOW (m));
	g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (fsel)->ok_button),
		"clicked", G_CALLBACK (on_open_ok_clicked), m);
	g_signal_connect (
		GTK_OBJECT (GTK_FILE_SELECTION (fsel)->cancel_button),
		"clicked", G_CALLBACK (on_cancel_clicked), m);
}

static void
action_about (gpointer callback_data, guint callback_action,
	      GtkWidget *widget)
{
	g_warning ("Implement!");
}

static void
action_thumbnail (gpointer callback_data, guint callback_action,
		  GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);
	GtkWidget *dialog;
	ExifData *edata;

	edata = jpeg_data_get_exif_data (m->priv->data);
	if (edata && edata->size) {
		dialog = gexif_thumbnail_new (edata->data, edata->size);
		gtk_window_set_transient_for (GTK_WINDOW (dialog),
					      GTK_WINDOW (m));
		gtk_widget_show (dialog);
	}
}

static GtkItemFactoryEntry mi[] =
{
	{"/_File", NULL, 0, 0, "<Branch>"},
	{"/File/_Open...", NULL, action_open, 0, "<StockItem>", GTK_STOCK_OPEN},
	{"/File/_Save", NULL, action_save, 0, "<StockItem>", GTK_STOCK_SAVE},
	{"/File/Save _As...", NULL, action_save_as, 0, "<StockItem>",
							GTK_STOCK_SAVE_AS},
	{"/File/sep1", NULL, 0, 0, "<Separator>"},
	{"/File/E_xit", NULL, action_exit, 0, "<StockItem>", GTK_STOCK_QUIT},
	{"/_View", NULL, 0, 0, "<Branch>"},
	{"/View/_Thumbnail", NULL, action_thumbnail, 0, NULL, NULL},
	{"/_Help", NULL, 0, 0, "<Branch>"},
	{"/Help/About", NULL, action_about, 0, NULL, NULL}
};

GtkWidget *
gexif_main_new (void)
{
	GExifMain *m;
	GtkWidget *browser, *vbox, *w;
	GtkItemFactory *gif;
	GtkAccelGroup *ag;

	m = g_object_new (GEXIF_TYPE_MAIN, NULL);
	gtk_window_set_title (GTK_WINDOW (m), PACKAGE);
	gtk_window_set_default_size (GTK_WINDOW (m), 640, 480);
	g_signal_connect (GTK_OBJECT (m), "delete_event",
			  G_CALLBACK (gtk_object_destroy), NULL);

	vbox = gtk_vbox_new (FALSE, 1);
	gtk_widget_show (vbox);
	gtk_container_add (GTK_CONTAINER (m), vbox);

	ag = gtk_accel_group_new ();
	gif = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", ag);
	g_object_set_data_full (G_OBJECT (m), "<main>", gif,
				(GDestroyNotify) g_object_unref);
	gtk_window_add_accel_group (GTK_WINDOW (m), ag);
	gtk_item_factory_create_items (gif, G_N_ELEMENTS (mi), mi, m);
	w = gtk_item_factory_get_widget (gif, "<main>");
	gtk_widget_show (w);
	gtk_box_pack_start (GTK_BOX (vbox), w, FALSE, FALSE, 0);
	g_object_unref (G_OBJECT (gif));

	browser = gtk_exif_browser_new ();
	gtk_widget_show (browser);
	gtk_box_pack_start (GTK_BOX (vbox), browser, TRUE, TRUE, 0);
	m->priv->browser = GTK_EXIF_BROWSER (browser);

	return (GTK_WIDGET (m));
}
