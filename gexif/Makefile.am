bin_PROGRAMS = gexif

gexif_SOURCES =					\
	gexif.c					\
	gexif-main.c gexif-main.h		\
	gexif-thumbnail.c gexif-thumbnail.h

gexif_CPPFLAGS = \
	$(AM_CPPFLAGS) $(CPPFLAGS)			\
	$(LIBEXIF_CFLAGS)				\
	$(LIBEXIF_GTK_CFLAGS)				\
	$(GTK_CFLAGS)

gexif_LDADD = \
	$(top_builddir)/libjpeg/libjpeg.la 		\
	$(LIBEXIF_LIBS)					\
	$(LIBEXIF_GTK_LIBS)				\
	$(GTK_LIBS)
