/* gexif-thumbnail.c
 *
 * Copyright (C) 2001 Lutz Müller <urc8@rz.uni-karlsruhe.de>
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

#include <gtk/gtkbutton.h>
#include <gtk/gtklabel.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtkscrolledwindow.h>
#include <gtk/gtkimage.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixbuf-loader.h>

struct _GExifThumbnailPrivate
{
};

#define PARENT_TYPE GTK_TYPE_DIALOG
static GtkDialogClass *parent_class;

static void
gexif_thumbnail_destroy (GtkObject *object)
{
	GExifThumbnail *thumbnail = GEXIF_THUMBNAIL (object);

	thumbnail = NULL;

	GTK_OBJECT_CLASS (parent_class)->destroy (object);
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
	GtkObjectClass *object_class;
	GObjectClass *gobject_class;

	object_class = GTK_OBJECT_CLASS (g_class);
	object_class->destroy  = gexif_thumbnail_destroy;

	gobject_class = G_OBJECT_CLASS (g_class);
	gobject_class->finalize = gexif_thumbnail_finalize;

	parent_class = g_type_class_peek_parent (g_class);
}

static void
gexif_thumbnail_init (GTypeInstance *instance, gpointer g_class)
{
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
	gtk_object_destroy (GTK_OBJECT (thumbnail));
}

GtkWidget *
gexif_thumbnail_new (const guchar *data, guint size)
{
	GExifThumbnail *thumbnail;
	GtkWidget *button, *image;
	GdkPixbufLoader *loader;
	GdkPixbuf *pixbuf;

	thumbnail = g_object_new (GEXIF_TYPE_THUMBNAIL, NULL);
	g_signal_connect (GTK_OBJECT (thumbnail), "delete_event",
			  G_CALLBACK (gtk_object_destroy), NULL);

	loader = gdk_pixbuf_loader_new ();
	gdk_pixbuf_loader_write (loader, data, size, NULL);
	gdk_pixbuf_loader_close (loader, NULL);
	pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
	image = gtk_image_new_from_pixbuf (pixbuf);
	g_object_unref (G_OBJECT (loader));
	gtk_widget_show (image);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (thumbnail)->vbox),
			    image, FALSE, FALSE, 0);

	button = gtk_button_new_with_label (_("Close"));
	gtk_widget_show (button);
	g_signal_connect (GTK_OBJECT (button), "clicked",
			  G_CALLBACK (on_close_clicked), thumbnail);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG (thumbnail)->action_area),
			   button);
	gtk_widget_grab_focus (button);

	return (GTK_WIDGET (thumbnail));
}
