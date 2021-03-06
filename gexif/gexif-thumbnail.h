/* gexif-thumbnail.h
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

#ifndef __GEXIF_THUMBNAIL_H__
#define __GEXIF_THUMBNAIL_H__

#include <gtk/gtk.h>

#define GEXIF_TYPE_THUMBNAIL  (gexif_thumbnail_get_type ())
#define GEXIF_THUMBNAIL(o)    (G_TYPE_CHECK_INSTANCE_CAST((o),GEXIF_TYPE_THUMBNAIL,GExifThumbnail))
#define GEXIF_IS_THUMBNAIL(o) (G_TYPE_CHECK_INSTANCE_TYPE((o),GEXIF_TYPE_THUMBNAIL))

typedef struct _GExifThumbnail        GExifThumbnail;
typedef struct _GExifThumbnailClass   GExifThumbnailClass;

/* use the struct and typedef if you have actually defined some struct members */
typedef void* GExifThumbnailPrivate;
/* typedef struct _GExifThumbnailPrivate GExifThumbnailPrivate; */

struct _GExifThumbnail
{
	GtkDialog parent;

	GExifThumbnailPrivate *priv;
};

struct _GExifThumbnailClass
{
	GtkDialogClass parent_class;
};

GType      gexif_thumbnail_get_type (void);
GtkWidget *gexif_thumbnail_new      (const guchar *data, guint size);

#endif /* __GEXIF_THUMBNAIL_H__ */
