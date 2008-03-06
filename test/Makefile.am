## Makefile.am for UED/test

INCLUDES = \
	-I$(top_builddir)/lib \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/lib     \
	-I$(top_srcdir)/src

include_HEADERS = \
        testing.h \
	testing_framework.h

if BUILD_UED_DEBUG 
noinst_PROGRAMS = test_debug test_linkedlist test_module test_unicode_module test_main test_packint
test_debug_SOURCES = test_debug.c
test_debug_LDADD   = ../src/libuniversal_encoding_detector.la ../lib/libued.la 
else
noinst_PROGRAMS = test_linkedlist test_module test_unicode_module test_main test_packint
endif

test_packint_SOURCES = test_packint.c
test_packint_LDADD   = ../src/libuniversal_encoding_detector.la ../lib/libued.la 

test_linkedlist_SOURCES = test_linkedlist.c
test_linkedlist_LDADD   = ../src/libuniversal_encoding_detector.la ../lib/libued.la 

test_unicode_module_SOURCES = test_unicode_module.c
test_unicode_module_LDADD   = ../src/libuniversal_encoding_detector.la ../lib/libued.la 

test_main_SOURCES = test_main.c
test_main_LDADD   = ../src/libuniversal_encoding_detector.la ../lib/libued.la 

AM_CPPFLAGS = \
        -DDATADIR=\"${RUNTIME_SYSROOT}@DATADIR@\"               \
        -DSOPATH=\"@SOPATH@\"           \
        -DMODULEDIR=\"${RUNTIME_SYSROOT}@MODULEDIR@\"   \
        "-DBUILDTIME=\"`date -u "+%Y-%m-%d %H:%M"`\""

test_module_SOURCES = test_module.c testing_framework.c
test_module_LDADD   = ../src/libuniversal_encoding_detector.la ../lib/libued.la  libued_testing_framework.la

testingframeworkdir = $(MODULEDIR)/test
xxx_moduledir = $(MODULEDIR)/test

if BUILD_STATIC
testingframework_DATA = libued_testing_framework.o
endif

testingframework_LTLIBRARIES = libued_testing_framework.la
xxx_module_LTLIBRARIES = libued_xxx_module.la

## noinst_LTLIBRARIES = libued_xxx_module.la libued_testing_framework.la

libued_testing_framework_la_LIBADD = \
        $(top_builddir)/lib/libued.la \
        $(top_builddir)/src/libuniversal_encoding_detector.la

libued_testing_framework_la_SOURCES = \
	testing_framework.c

libued_xxx_module_la_LDFLAGS = \
        -avoid-version  \
        -module

libued_xxx_module_la_SOURCES = \
        xxx_module.c

libued_xxx_module_la_LIBADD = \
        libued_testing_framework.la


include $(top_srcdir)/rules/libobject.make	
## // include $(top_srcdir)/rules/nmfile.make	
