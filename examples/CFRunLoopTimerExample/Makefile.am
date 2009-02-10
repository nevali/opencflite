# This example doesn't work yet on Linux (due to CoreFoundation
# implementation issues), so the progam is EXTRA_ rather than check_
# and test is the target goal rather than check.

AM_CFLAGS			= -I${top_srcdir}/include

if CF_BUILD_TESTS
EXTRA_PROGRAMS			= CFRunLoopTimerExample
endif

CFRunLoopTimerExample_LDADD	= ${top_builddir}/libCoreFoundation.la

CFRunLoopTimerExample_SOURCES	= CFRunLoopTimerExample.c

if CF_BUILD_TESTS
test: CFRunLoopTimerExample
	${LIBTOOL} --mode execute ./CFRunLoopTimerExample 0.25  5.00
	${LIBTOOL} --mode execute ./CFRunLoopTimerExample 1.00  1.00
	${LIBTOOL} --mode execute ./CFRunLoopTimerExample 1.00  0.50
	${LIBTOOL} --mode execute ./CFRunLoopTimerExample 0.10  1.00
	${LIBTOOL} --mode execute ./CFRunLoopTimerExample 1.00  2.00
	${LIBTOOL} --mode execute ./CFRunLoopTimerExample 1.00 10.00
	${LIBTOOL} --mode execute ./CFRunLoopTimerExample 0.33  1.00

gdb: CFRunLoopTimerExample
	${LIBTOOL} --mode execute ${@} ./CFRunLoopTimerExample

valgrind: CFRunLoopTimerExample
	${LIBTOOL} --mode execute ${@} ${VALGRINDFLAGS} ./CFRunLoopTimerExample
endif
