bin_PROGRAMS = gexif

gexif_SOURCES =					\
	gexif.c					\
	gexif-main.c gexif-main.h		\
	gexif-thumbnail.c gexif-thumbnail.h

# We cannot define -DGTK_DISABLE_DEPRECATED here
# as long as we're still using GtkItemFactory.

gexif_CFLAGS = \
	$(AM_CFLAGS) $(CFLAGS)				\
	-I$(top_srcdir)					\
	$(LIBEXIF_CFLAGS)				\
	$(LIBEXIF_GTK_CFLAGS)				\
	$(GTK_CFLAGS)

gexif_LDADD = \
	$(top_builddir)/libjpeg/libjpeg.la 		\
	$(LIBEXIF_LIBS)					\
	$(LIBEXIF_GTK_LIBS)				\
	$(GTK_LIBS)
