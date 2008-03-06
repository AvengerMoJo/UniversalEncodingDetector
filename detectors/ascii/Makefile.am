## Makefile.am for UED/detectors/ascii

INCLUDES = \
	-I$(top_srcdir)/include	\
	-I$(top_builddir)/lib	\
	-I$(top_srcdir)/lib	\
	-I$(top_srcdir)/src	

include_HEADERS = \
        esc_statemachine.h

ascii_LTLIBRARIES = libued_ascii.la

if BUILD_STATIC
ascii_PLUGIN = $(ascii_LTLIBRARIES:.la=.o)
endif

asciidir = $(MODULEDIR)/detectors

libued_ascii_la_SOURCES =	\
	ascii_detector.c

libued_ascii_la_LDFLAGS = \
	-export-dynamic		\
	-avoid-version		\
	$(UED_LDFLAGS)

libued_ascii_la_LIBADD = \
	$(top_builddir)/lib/libued.la \
	$(top_builddir)/src/libuniversal_encoding_detector.la


include $(top_srcdir)/rules/libobject.make
