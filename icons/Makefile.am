nobase_dist_icons_DATA = 8x8/gexif.png 16x16/gexif.png 22x22/gexif.png 24x24/gexif.png 32x32/gexif.png 48x48/gexif.png 64x64/gexif.png 128x128/gexif.png 256x256/gexif.png scalable/gexif.svgz

EXTRA_DIST = gexif-master.svg
iconsdir = $(datadir)/icons/hicolor

OPTIMIZE_PNG=advdef -z4

8x8/gexif.png: scalable/gexif.svgz
	inkscape -C -w 8 -h 8 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

16x16/gexif.png: scalable/gexif.svgz
	inkscape -C -w 16 -h 16 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

22x22/gexif.png: scalable/gexif.svgz
	inkscape -C -w 22 -h 22 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

24x24/gexif.png: scalable/gexif.svgz
	inkscape -C -w 24 -h 24 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

32x32/gexif.png: scalable/gexif.svgz
	inkscape -C -w 32 -h 32 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

48x48/gexif.png: scalable/gexif.svgz
	inkscape -C -w 48 -h 48 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

64x64/gexif.png: scalable/gexif.svgz
	inkscape -C -w 64 -h 64 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

128x128/gexif.png: scalable/gexif.svgz
	inkscape -C -w 128 -h 128 -o "$@" "$^"
	-$(OPTIMIZE_PNG) "$@"

256x256/gexif.png: scalable/gexif.svgz
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
