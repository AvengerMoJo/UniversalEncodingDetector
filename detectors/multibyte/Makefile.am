## Makefile.am for UED/detectors/multibyte

INCLUDES = \
	-I$(top_srcdir)/include	\
	-I$(top_builddir)/lib	\
	-I$(top_srcdir)/lib	\
	-I$(top_srcdir)/src	

include_HEADERS = \
	BIG5Freq.tab             \
	EUCTWFreq.tab            \
	EUCKRFreq.tab            \
	GB18030Freq.tab          \
	JISFreq.tab              \
	big5_statemachine.h      \
	euctw_statemachine.h     \
	euckr_statemachine.h     \
	gb18030_statemachine.h   \
	shift_jis_statemachine.h \
        unicode_statemachine.h

multibyte_LTLIBRARIES = libued_multibyte.la

if BUILD_STATIC
multibyte_PLUGIN = $(multibyte_LTLIBRARIES:.la=.o)
endif

multibytedir = $(MODULEDIR)/detectors

libued_multibyte_la_SOURCES =	\
	multibyte_detector.c

libued_multibyte_la_LDFLAGS = \
	-export-dynamic		\
	-avoid-version		\
	$(UED_LDFLAGS)

libued_multibyte_la_LIBADD = \
	$(top_builddir)/lib/libued.la \
	$(top_builddir)/src/libuniversal_encoding_detector.la


include $(top_srcdir)/rules/libobject.make
