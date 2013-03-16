/* gexif-main.h
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

#ifndef __GEXIF_MAIN_H__
#define __GEXIF_MAIN_H__

#include <gtk/gtk.h>

#define GEXIF_TYPE_MAIN     (gexif_main_get_type())
#define GEXIF_MAIN(o)       (G_TYPE_CHECK_INSTANCE_CAST((o),GEXIF_TYPE_MAIN,GExifMain))
#define GEXIF_MAIN_CLASS(k) (G_TYPE_CHECK_CLASS_CAST((k),GEXIF_TYPE_MAIN,GExifMainClass))
#define GEXIF_IS_MAIN(o)    (G_TYPE_CHECK_INSTANCE_TYPE((o),GEXIF_TYPE_MAIN))

typedef struct _GExifMain        GExifMain;
typedef struct _GExifMainPrivate GExifMainPrivate;
typedef struct _GExifMainClass   GExifMainClass;

struct _GExifMain
{
	GtkWindow parent;

	GExifMainPrivate *priv;
};

struct _GExifMainClass
{
	GtkWindowClass parent_class;
};

GType      gexif_main_get_type (void);
GtkWidget *gexif_main_new (void);

void       gexif_main_load_file (GExifMain *m, const gchar *path);

#endif /* __GEXIF_MAIN_H__ */
