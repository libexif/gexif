AC_PREREQ(2.69)
AC_INIT([GTK EXIF viewer/editor],
        [0.5.0git3],
        [libexif-devel@lists.sourceforge.net],
        [gexif],
        [https://libexif.github.io/])
AC_CONFIG_SRCDIR([gexif/gexif.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([auto-m4])
AM_INIT_AUTOMAKE([
  gnu
  1.14.1
  dist-xz
  dist-bzip2
])
AM_SILENT_RULES([yes])

GP_CHECK_SHELL_ENVIRONMENT
GP_CONFIG_MSG([Build])
GP_CONFIG_MSG([Source code location], [${srcdir}])

AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
LT_INIT([win32-dll])
AM_CPPFLAGS="$CPPFLAGS -I\$(top_srcdir)"


# ---------------------------------------------------------------------------
# i18n support
# ---------------------------------------------------------------------------
GP_GETTEXT_HACK([], [Lutz Mueller and others])
ALL_LINGUAS="de es fr pl sv"
AM_PO_SUBDIRS()
AM_GNU_GETTEXT_VERSION([0.18.3])
AM_GNU_GETTEXT([external])
AM_XGETTEXT_OPTION([--from-code=UTF-8])dnl macro since gettext 0.17
AM_ICONV()
GP_GETTEXT_FLAGS()


# ---------------------------------------------------------------------------
# conditional libraries
# ---------------------------------------------------------------------------
AC_ARG_WITH([gtk3],
            [AS_HELP_STRING([--with-gtk3],[build with gtk3 (default is gtk2)])],
            [gtk_version=gtk3],
            [gtk_version=gtk2])

GP_CHECK_LIBRARY([LIBEXIF], [libexif])
AS_VAR_IF([gtk_version], [gtk3], [dnl
  GP_CHECK_LIBRARY([GTK], [gtk+-3.0])dnl
  GP_CHECK_LIBRARY([LIBEXIF_GTK], [libexif-gtk3],[>= 0.3.3])
], [dnl
  GP_CHECK_LIBRARY([GTK], [gtk+-2.0], [>= 2.6])dnl
  GP_CHECK_LIBRARY([LIBEXIF_GTK], [libexif-gtk],[>= 0.3.3])
])


# ---------------------------------------------------------------------------
# Warnings: If we have GCC, be paranoid.
# ---------------------------------------------------------------------------
AS_VAR_IF([GCC], [yes], [dnl
    AM_CPPFLAGS="$AM_CPPFLAGS -Wall -Wchar-subscripts -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith"
    AM_LDFLAGS="$AM_LDFLAGS -g -Wall"
])

AC_SUBST([AM_CPPFLAGS])dnl
AC_SUBST([AM_LDFLAGS])dnl


# ---------------------------------------------------------------------------
# Output files
# ---------------------------------------------------------------------------
AC_CONFIG_FILES([
  Makefile
  gexif/Makefile
  icons/Makefile
  libjpeg/Makefile
  m4m/Makefile
  po/Makefile.in
])
AC_OUTPUT

GP_CONFIG_OUTPUT
