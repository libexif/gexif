/* gexif-thumbnail.c
 *
 * Copyright © 2001 Lutz Müller <lutz@users.sf.net>
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

#include <string.h>

#include <gtk/gtk.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixbuf-loader.h>

/* define this later if needed
struct _GExifThumbnailPrivate
{
};
*/

#define PARENT_TYPE GTK_TYPE_DIALOG
static GtkDialogClass *parent_class;

static void
#if GTK_CHECK_VERSION(3,0,0)
gexif_thumbnail_destroy (GtkWidget *widget)
#else
gexif_thumbnail_destroy (GtkObject *object)
#endif
{
#if GTK_CHECK_VERSION(3,0,0)
	GExifThumbnail *thumbnail = GEXIF_THUMBNAIL (widget);
#else
	GExifThumbnail *thumbnail = GEXIF_THUMBNAIL (object);
#endif

	/* Nothing to free here */
	(void)thumbnail;

#if GTK_CHECK_VERSION(3,0,0)
	GTK_WIDGET_CLASS (parent_class)->destroy (widget);
#else
	GTK_OBJECT_CLASS (parent_class)->destroy (object);
#endif
}

static void
gexif_thumbnail_finalize (GObject *object)
{
	GExifThumbnail *thumbnail = GEXIF_THUMBNAIL (object);

	g_free (thumbnail->priv);

	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gexif_thumbnail_class_init (gpointer g_class, gpointer class_data)
{
	(void)class_data;

#if GTK_CHECK_VERSION(3,0,0)
	GtkWidgetClass *widget_class;
	GObjectClass *gobject_class;

	widget_class = GTK_WIDGET_CLASS (g_class);
	widget_class->destroy = gexif_thumbnail_destroy;
#else
	GtkObjectClass *object_class;
	GObjectClass *gobject_class;

	object_class = GTK_OBJECT_CLASS (g_class);
	object_class->destroy  = gexif_thumbnail_destroy;
#endif

	gobject_class = G_OBJECT_CLASS (g_class);
	gobject_class->finalize = gexif_thumbnail_finalize;

	parent_class = g_type_class_peek_parent (g_class);
}

static void
gexif_thumbnail_init (GTypeInstance *instance, gpointer g_class)
{
	(void)g_class;

	GExifThumbnail *thumbnail = GEXIF_THUMBNAIL (instance);

	thumbnail->priv = g_new0 (GExifThumbnailPrivate, 1);
}

GType
gexif_thumbnail_get_type (void)
{
	static GType t = 0;
	GTypeInfo ti;

	if (!t) {
		memset (&ti, 0, sizeof (GTypeInfo));
		ti.class_size    = sizeof (GExifThumbnailClass);
		ti.class_init    = gexif_thumbnail_class_init;
		ti.instance_size =sizeof (GExifThumbnail);
		ti.instance_init = gexif_thumbnail_init;
		t = g_type_register_static (PARENT_TYPE, "GExifThumbnail",
					    &ti, 0);
	}

	return (t);
}

static void
on_close_clicked (GtkButton *button, GExifThumbnail *thumbnail)
{
	(void)button;

#if GTK_CHECK_VERSION(3,0,0)
	gtk_widget_destroy (GTK_WIDGET (thumbnail));
#else
	gtk_object_destroy (GTK_OBJECT (thumbnail));
#endif
}

GtkWidget *
gexif_thumbnail_new (const guchar *data, guint size)
{
	GExifThumbnail *thumbnail;
	GtkWidget *button, *image;
	GdkPixbufLoader *loader;
	GdkPixbuf *pixbuf;

	thumbnail = g_object_new (GEXIF_TYPE_THUMBNAIL, NULL);
#if GTK_CHECK_VERSION(3,0,0)
	g_signal_connect (G_OBJECT (thumbnail), "delete_event",
			  G_CALLBACK (gtk_widget_destroy), NULL);
#else
	g_signal_connect (GTK_OBJECT (thumbnail), "delete_event",
			  G_CALLBACK (gtk_object_destroy), NULL);
#endif

	loader = gdk_pixbuf_loader_new ();
	gdk_pixbuf_loader_write (loader, data, size, NULL);
	gdk_pixbuf_loader_close (loader, NULL);
	pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
	image = gtk_image_new_from_pixbuf (pixbuf);
	g_object_unref (G_OBJECT (loader));
	gtk_widget_show (image);

#if GTK_CHECK_VERSION(2,14,0)
	gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (thumbnail))),
			    image, FALSE, FALSE, 0);
#else
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (thumbnail)->vbox),
			    image, FALSE, FALSE, 0);
#endif

	button = gtk_button_new_with_label (_("Close"));
	gtk_widget_show (button);
	g_signal_connect (G_OBJECT (button), "clicked",
			  G_CALLBACK (on_close_clicked), thumbnail);

#if GTK_CHECK_VERSION(2,14,0)
	gtk_container_add (GTK_CONTAINER (gtk_dialog_get_action_area (GTK_DIALOG (thumbnail))),
			   button);
#else
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG (thumbnail)->action_area),
			   button);
#endif

	gtk_widget_grab_focus (button);

	return (GTK_WIDGET (thumbnail));
}
