nobase_dist_icons_DATA = 8x8/gexif.png 16x16/gexif.png 22x22/gexif.png 24x24/gexif.png 32x32/gexif.png 48x48/gexif.png 64x64/gexif.png 128x128/gexif.png 256x256/gexif.png scalable/gexif.svgz

EXTRA_DIST = gexif-master.svg
iconsdir = $(datadir)/icons/hicolor

OPTIMIZE_PNG=advdef -z4

8x8/gexif.png: scalable/gexif.svgz
	-mkdir 8x8
	inkscape -C -w 8 -h 8 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

16x16/gexif.png: scalable/gexif.svgz
	-mkdir 16x16
	inkscape -C -w 16 -h 16 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

22x22/gexif.png: scalable/gexif.svgz
	-mkdir 22x22
	inkscape -C -w 22 -h 22 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

24x24/gexif.png: scalable/gexif.svgz
	-mkdir 24x24
	inkscape -C -w 24 -h 24 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

32x32/gexif.png: scalable/gexif.svgz
	-mkdir 32x32
	inkscape -C -w 32 -h 32 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

48x48/gexif.png: scalable/gexif.svgz
	-mkdir 48x48
	inkscape -C -w 48 -h 48 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

64x64/gexif.png: scalable/gexif.svgz
	-mkdir 64x64
	inkscape -C -w 64 -h 64 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

128x128/gexif.png: scalable/gexif.svgz
	-mkdir 128x128
	inkscape -C -w 128 -h 128 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

256x256/gexif.png: scalable/gexif.svgz
	-mkdir 256x256
	inkscape -C -w 256 -h 256 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache
update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated.  After (un)install, run this:"; \
		echo "***   $(gtk_update_icon_cache)"; \
	fi
# Ensure the svgz source file has an older time stamp than the png files
# to avoid make trying to rebuild them unnecessarily.
dist-hook:
	touch -d 2023-01-01T00:00:00 "$(distdir)/scalable/gexif.svgz"
