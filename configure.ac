AC_PREREQ(2.59)
AC_INIT([GTK EXIF viewer/editor],[0.5.0cvs3],[libexif-devel@lists.sourceforge.net],[gexif])
AC_CONFIG_SRCDIR([gexif/gexif.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([auto-m4])
AM_INIT_AUTOMAKE([gnu 1.8 dist-bzip2])

if test ! -d "$srcdir/m4m"; then
AC_MSG_ERROR([
You are missing the m4m/ directory in your top
$PACKAGE_TARNAME source directory.

You are probably using an ill-maintained CVS tree.
Running

    cd $srcdir
    cvs co m4m

and re-running autogen.sh might help.
])
fi

GP_CHECK_SHELL_ENVIRONMENT
GP_CONFIG_MSG([Build])
GP_CONFIG_MSG([Source code location],[${srcdir}])

AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AM_PROG_LIBTOOL
AM_CPPFLAGS="$CPPFLAGS -I\$(top_srcdir)"
AM_PROG_CC_C_O
GP_PKG_CONFIG


# ---------------------------------------------------------------------------
# i18n support
# ---------------------------------------------------------------------------
GP_GETTEXT_HACK([],[Lutz Müller and others])
ALL_LINGUAS="de es fr pl sv"
AM_GNU_GETTEXT_VERSION([0.14.1])
AM_GNU_GETTEXT([external])
AM_PO_SUBDIRS()
AM_ICONV()
GP_GETTEXT_FLAGS()


# ---------------------------------------------------------------------------
# conditional libraries
# ---------------------------------------------------------------------------
AC_ARG_WITH([gtk3],
            [AS_HELP_STRING([--with-gtk3],[build with gtk3 (default is gtk2)])],
            [gtk_version=gtk3],
            [gtk_version=gtk2])

if test "x$gtk_version" == "xgtk3" ; then
  GP_CHECK_LIBRARY([GTK], [gtk+-3.0])dnl
  GP_CHECK_LIBRARY([LIBEXIF_GTK], [libexif-gtk3],[>= 0.3.3])
else
  GP_CHECK_LIBRARY([GTK], [gtk+-2.0], [>= 2.6])dnl
  GP_CHECK_LIBRARY([LIBEXIF_GTK], [libexif-gtk],[>= 0.3.3])
fi


# ---------------------------------------------------------------------------
# Warnings: If we have GCC, be paranoid.
# ---------------------------------------------------------------------------
if test "x$GCC" = "xyes"; then
    AM_CPPFLAGS="$AM_CPPFLAGS -Wall -Wchar-subscripts -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith"
    AM_LDFLAGS="$AM_LDFLAGS -g -Wall"
fi

AC_SUBST([AM_CPPFLAGS])dnl
AC_SUBST([AM_LDFLAGS])dnl


# ---------------------------------------------------------------------------
# Output files
# ---------------------------------------------------------------------------
AC_CONFIG_FILES([
  po/Makefile.in
  Makefile
  m4m/Makefile
  libjpeg/Makefile
  gexif/Makefile
])
AC_OUTPUT()dnl

GP_CONFIG_OUTPUT()dnl
