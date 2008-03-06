## Makefile.am for UED/detectors/unicode

INCLUDES = \
	-I$(top_srcdir)/include	\
	-I$(top_builddir)/lib	\
	-I$(top_srcdir)/lib	\
	-I$(top_srcdir)/src	

unicode_LTLIBRARIES = libued_unicode.la

if BUILD_STATIC
unicode_PLUGIN = $(unicode_LTLIBRARIES:.la=.o)
endif

unicodedir = $(MODULEDIR)/detectors

libued_unicode_la_SOURCES =	\
	unicode_detector.c

libued_unicode_la_LDFLAGS = \
	-export-dynamic		\
	-avoid-version		\
	$(UED_LDFLAGS)

libued_unicode_la_LIBADD = \
	$(top_builddir)/lib/libued.la \
	$(top_builddir)/src/libuniversal_encoding_detector.la


include $(top_srcdir)/rules/libobject.make
