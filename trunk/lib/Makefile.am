## Makefile.am for UED/lib

INCLUDES = \
	-I$(top_builddir)/include       \
	-I$(top_builddir)/lib           \
	-I$(top_srcdir)/include         \
	-I$(top_srcdir)/lib

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = $(top_srcdir)/spec/universal_encoding_detector.pc


includedir = @INCLUDEDIR@

include_HEADERS = \
	debug.h			 \
	char_distribution.h	 \
	encoding_state_machine.h \
	linkedlist.h		 \
	modules.h                \
	packint.h                \
	types.h

lib_LTLIBRARIES = libued.la


if BUILD_UED_DEBUG
libued_la_SOURCES =              \
	char_distribution.c	 \
        debug.c	                 \
	encoding_state_machine.c \
	modules.c                \
	linkedlist.c
else 
libued_la_SOURCES =              \
	char_distribution.c	 \
	encoding_state_machine.c \
	modules.c                \
	linkedlist.c
endif

libued_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)    \
	-release $(LT_RELEASE)                                  \
	$(UED_LDFLAGS)


#
## and now rebuild the static version with the *correct* object files
#
if BUILD_STATIC

clean-local:
	rm -f libued_fixed.a

all-local: libued_fixed.a

libued_fixed.a: .libs/libued.a
	rm -f libued_fixed.a
	${AR} cru libued_fixed.a `find . -name "*.o" | grep -v '.libs'`
	${RANLIB} libued_fixed.a
	cp -pf libued_fixed.a .libs/libued.a

.libs/libued.a: libued.la

else

clean-local:

all-local:

endif

include $(top_srcdir)/rules/nmfile.make
