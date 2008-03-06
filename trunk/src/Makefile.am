## Makefile.am for UED/src

INCLUDES = \
        -I$(top_builddir)/include       \
        -I$(top_builddir)/lib           \
        -I$(top_srcdir)/include         \
        -I$(top_srcdir)/lib             \
        -I$(top_srcdir)/src

AM_CPPFLAGS = \
        -DDATADIR=\"${RUNTIME_SYSROOT}@DATADIR@\"               \
        -DSOPATH=\"@SOPATH@\"           \
        -DMODULEDIR=\"${RUNTIME_SYSROOT}@MODULEDIR@\"   \
        "-DBUILDTIME=\"`date -u "+%Y-%m-%d %H:%M"`\""


includedir = $(INCLUDEDIR)

include_HEADERS = \
	configurator.h \
	core_detector.h \
	detector.h

lib_LTLIBRARIES = libuniversal_encoding_detector.la

libuniversal_encoding_detector_la_SOURCES =        \
	detector.c                                 \
	configurator.c                             \
        universal_encoding_detector.c

libuniversal_encoding_detector_la_LDFLAGS = \
        -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)    \
        -release $(LT_RELEASE)                                  \
        $(UED_LDFLAGS)

libuniversal_encoding_detector_la_LIBADD = \
        ../lib/libued.la



#
## and now rebuild the static version with the *correct* object files
#
if BUILD_STATIC

clean-local:
	rm -f libuniversal_encoding_detector_fixed.a

all-local: libuniversal_encoding_detector_fixed.a

libuniversal_encoding_detector_fixed.a: .libs/libuniversal_encoding_detector.a
	rm -f libuniversal_encoding_detector_fixed.a
	${AR} cru libuniversal_encoding_detector_fixed.a `find . -name "*.o" | grep -v '.libs'`
	${RANLIB} libuniversal_encoding_detector_fixed.a
	cp -pf libuniversal_encoding_detector_fixed.a .libs/libuniversal_encoding_detector.a

.libs/libuniversal_encoding_detector.a: libuniversal_encoding_detector.la

else

clean-local:

all-local:

endif


