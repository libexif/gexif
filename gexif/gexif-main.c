/* gexif-main.c
 *
 * Copyright © 2001 Lutz Müller <lutz@users.sourceforge.net>
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

#include "config.h"
#include "gexif-main.h"
#include "gexif-thumbnail.h"

#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <libjpeg/jpeg-data.h>
#include <libexif-gtk/gtk-exif-browser.h>

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
#if GTK_CHECK_VERSION(3,0,0)
gexif_main_destroy (GtkWidget *widget)
#else
gexif_main_destroy (GtkObject *object)
#endif
{
#if GTK_CHECK_VERSION(3,0,0)
	GExifMain *m = GEXIF_MAIN (widget);
#else
	GExifMain *m = GEXIF_MAIN (object);
#endif

	if (m->priv->data) {
		jpeg_data_unref (m->priv->data);
		m->priv->data = NULL;
	}

	if (m->priv->path) {
		g_free (m->priv->path);
		m->priv->path = NULL;
	}

#if GTK_CHECK_VERSION(3,0,0)
	GTK_WIDGET_CLASS (parent_class)->destroy (widget);
#else
	GTK_OBJECT_CLASS (parent_class)->destroy (object);
#endif
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
#if GTK_CHECK_VERSION(3,0,0)
	GtkWidgetClass *widget_class;
	GObjectClass *gobject_class;

	widget_class = GTK_WIDGET_CLASS (g_class);
	widget_class->destroy = gexif_main_destroy;
#else
	GtkObjectClass *object_class;
	GObjectClass *gobject_class;

	object_class = GTK_OBJECT_CLASS (g_class);
	object_class->destroy = gexif_main_destroy;
#endif

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
		GtkWidget *dialog_parent = gtk_widget_get_ancestor (
										GTK_WIDGET (m), GTK_TYPE_WINDOW);
		GtkWidget *dialog = gtk_message_dialog_new (
								GTK_WINDOW(dialog_parent),
								GTK_DIALOG_DESTROY_WITH_PARENT,
								GTK_MESSAGE_WARNING,
								GTK_BUTTONS_CLOSE,
								_("No EXIF data found!"));
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
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

#if GTK_CHECK_VERSION(3,0,0)
	gtk_widget_destroy (GTK_WIDGET (m));
#else
	gtk_object_destroy (GTK_OBJECT (m));
#endif
}

static void
action_save (gpointer callback_data, guint callback_action,
	     GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);

	gexif_main_save_file (m, m->priv->path);
}

static void
on_save_as_ok_clicked (GtkWidget *fchoser, GExifMain *m)
{
	gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fchoser));
	gexif_main_save_file (m, filename);
	g_free(filename);
}

static void
action_save_as (gpointer callback_data, guint callback_action,
		GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);
	GtkWidget *fchoser;
	GtkWidget *fchoser_parent;

	fchoser_parent = gtk_widget_get_ancestor (GTK_WIDGET (m), GTK_TYPE_WINDOW);
	fchoser = gtk_file_chooser_dialog_new (
					_("Save As..."),
					GTK_WINDOW(fchoser_parent),
					GTK_FILE_CHOOSER_ACTION_SAVE,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
					NULL);

	if (gtk_dialog_run (GTK_DIALOG (fchoser)) == GTK_RESPONSE_ACCEPT)
		on_save_as_ok_clicked (fchoser, m);

	gtk_widget_destroy (fchoser);
}

static void
on_open_ok_clicked (GtkWidget *fchoser, GExifMain *m)
{
	gchar *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fchoser));
	gexif_main_load_file (m, filename);
	g_free(filename);
}

static void
action_open (gpointer callback_data, guint callback_action,
	     GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);
	GtkWidget *fchoser;
	GtkWidget *fchoser_parent;

	fchoser_parent = gtk_widget_get_ancestor (GTK_WIDGET (m), GTK_TYPE_WINDOW);
	fchoser = gtk_file_chooser_dialog_new (
					_("Open..."),
					GTK_WINDOW(fchoser_parent),
					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					NULL);

	if (gtk_dialog_run (GTK_DIALOG (fchoser)) == GTK_RESPONSE_ACCEPT)
		on_open_ok_clicked (fchoser, m);

	gtk_widget_destroy (fchoser);
}

static void
action_about (gpointer callback_data, guint callback_action,
	      GtkWidget *widget)
{
	GExifMain *m = GEXIF_MAIN (callback_data);

	const gchar *authors[] = {
		"Lutz Müller <lutz@users.sourceforge.net>",
		NULL
	};

	GtkWidget* dialog = gtk_about_dialog_new ();
	gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (m));
	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (dialog), "Copyright © 2001 Lutz Müller");
	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog), _("gexif is the GTK+ based GUI interface to libexif-gtk"));
	gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (dialog), "http://libexif.sourceforge.net/");
	gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (dialog), "http://libexif.sourceforge.net/");
	gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (dialog), authors);
	gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (dialog),
		"Copyright © 2001 Lutz Müller <lutz@users.sourceforge.net>\n\n"
		"This library is free software; you can redistribute it and/or\n"
		"modify it under the terms of the GNU Lesser General Public\n"
		"License as published by the Free Software Foundation; either\n"
		"version 2 of the License, or (at your option) any later version.\n\n"
		"This library is distributed in the hope that it will be useful,\n"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
		"Lesser General Public License for more details.\n\n"
		"You should have received a copy of the GNU Lesser General Public\n"
		"License along with this library; if not, write to the\n"
		"Free Software Foundation, Inc., 59 Temple Place - Suite 330,\n"
		"Boston, MA 02111-1307, USA.\n");

	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
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

static GtkWidget *
gexif_main_create_menu (GExifMain *m)
{
	GtkAccelGroup *ag    = NULL;
	GtkWidget *menubar   = NULL;
	GtkWidget *menu      = NULL;
	GtkWidget *menuitem  = NULL;
	GtkWidget *menuimage = NULL;
	guint accelerator_key;
	GdkModifierType accelerator_mods;
	gint i;

	struct _MenuInfo
	{
		gint type;
		gchar mnemonic[32];
		gchar accelerator[32];
		gchar *image;
		void (*callback)(gpointer, guint, GtkWidget*);
	} info[10] = {
	{ 0, N_("_File"        ), ""          , NULL             , NULL             },
	{ 1, N_("_Open..."     ), "<Control>o", GTK_STOCK_OPEN   , action_open      },
	{ 1, N_("_Save"        ), "<Control>s", GTK_STOCK_SAVE   , action_save      },
	{ 1, N_("Save _As..."  ), ""          , GTK_STOCK_SAVE_AS, action_save_as   },
	{ 2, ""                 , ""          , NULL             , NULL             },
	{ 1, N_("E_xit"        ), "<Control>x", GTK_STOCK_QUIT   , action_exit      },
	{ 0, N_("_View"        ), ""          , NULL             , NULL             },
	{ 1, N_("_Thumbnail..."), "<Control>t", NULL             , action_thumbnail },
	{ 0, N_("_Help"        ), ""          , NULL             , NULL             },
	{ 1, N_("_About..."    ), ""          , NULL             , action_about     }};

	ag = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW (m), ag);

	menubar = gtk_menu_bar_new ();

	for (i = 0; i < 10; i++) {
		switch (info[i].type) {
			case 0:
				menu = gtk_menu_new ();
				gtk_widget_show (menu);

				menuitem = gtk_menu_item_new_with_mnemonic (_(info[i].mnemonic));
				gtk_menu_item_set_submenu (GTK_MENU_ITEM(menuitem), menu);

				gtk_menu_shell_append (GTK_MENU_SHELL (menubar), menuitem);
				gtk_widget_show (menuitem);
				break;

			case 1:
				if (info[i].image) {
					menuitem = gtk_image_menu_item_new_with_mnemonic (
														_(info[i].mnemonic));
					menuimage = gtk_image_new_from_stock (info[i].image,
													GTK_ICON_SIZE_MENU);
					gtk_image_menu_item_set_image (
								GTK_IMAGE_MENU_ITEM (menuitem), menuimage);
				} else
					menuitem = gtk_menu_item_new_with_mnemonic (
													_(info[i].mnemonic));

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);

				gtk_accelerator_parse (info[i].accelerator,
								&accelerator_key, &accelerator_mods);

				if (accelerator_key && accelerator_mods)
					gtk_widget_add_accelerator(menuitem, "activate", ag,
						accelerator_key, accelerator_mods, GTK_ACCEL_VISIBLE);

				if (info[i].callback)
					g_signal_connect_swapped (G_OBJECT(menuitem), "activate",
										G_CALLBACK(info[i].callback), m);
				break;

			case 2:
				menuitem = gtk_separator_menu_item_new ();

				gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
				gtk_widget_show (menuitem);
		}
	}

	return menubar;
}

GtkWidget *
gexif_main_new (void)
{
	GExifMain *m;
	GtkWidget *browser, *vbox;
	GtkWidget *menubar;

	m = g_object_new (GEXIF_TYPE_MAIN, NULL);
	gtk_window_set_title (GTK_WINDOW (m), PACKAGE);
	gtk_window_set_default_size (GTK_WINDOW (m), 640, 480);
#if GTK_CHECK_VERSION(3,0,0)
	g_signal_connect (G_OBJECT (m), "delete_event",
			  G_CALLBACK (gtk_widget_destroy), NULL);
#else
	g_signal_connect (GTK_OBJECT (m), "delete_event",
			  G_CALLBACK (gtk_object_destroy), NULL);
#endif

	vbox = gtk_vbox_new (FALSE, 1);
	gtk_widget_show (vbox);
	gtk_container_add (GTK_CONTAINER (m), vbox);

	menubar = gexif_main_create_menu (m);
	gtk_widget_show (menubar);
	gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);

	browser = gtk_exif_browser_new ();
	gtk_widget_show (browser);
	gtk_box_pack_start (GTK_BOX (vbox), browser, TRUE, TRUE, 0);
	m->priv->browser = GTK_EXIF_BROWSER (browser);

	return (GTK_WIDGET (m));
}
