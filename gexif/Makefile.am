INCLUDES =						\
	-I$(top_srcdir)					\
	-DGTK_DISABLE_DEPRECATED			\
	-DGEXIF_LOCALEDIR="\"$(datadir)/locale\""	\
	$(GEXIF_CFLAGS)

bin_PROGRAMS = gexif

gexif_SOURCES =					\
	gexif.c					\
	gexif-main.c gexif-main.h		\
	gexif-thumbnail.c gexif-thumbnail.h
gexif_LDADD = $(top_builddir)/libjpeg/libjpeg.la $(GEXIF_LIBS)
