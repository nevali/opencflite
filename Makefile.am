SUBDIRS				= include

#
# We don't want to modify any of the package source to include the
# configuration header, so we use the compiler to include it as a
# prefix header. This is likely GCC specific, so we need to probably
# abstract this further as other deployment targets are supported.
#

DEFS				+= -include ${CONFIG_HEADER}

#
# Define where we will install and where CoreFoundation can find its
# Unicode character mappings.
#

libCoreFoundation_la_datadir	= ${datadir}/CoreServices

libCoreFoundation_la_data_DATA	= CFCharacterSetBitmaps.bitmap		\
				  CFUniCharPropertyDatabase.data	\
				  CFUnicodeData-B.mapping		\
				  CFUnicodeData-L.mapping

CF_COMMON_CPPFLAGS		= $(AM_CPPFLAGS)			\
				  -DU_SHOW_DRAFT_API=1			\
				  -DCF_BUILDING_CF=1			\
				  -D__kCFCharacterSetDir=\"${libCoreFoundation_la_datadir}\" \
				  -DMAC_OS_X_VERSION_MAX_ALLOWED=MAC_OS_X_VERSION_10_5	\
				  -I$(top_srcdir)/include		\
				  -I$(top_srcdir)/include/mach_support

CF_COMMON_CFLAGS		= $(AM_CFLAGS)

CF_COMMON_LDFLAGS		= $(AM_LDFLAGS)				\
				  -version-info $(CF_VERSION_INFO)

CF_COMMON_LIBADD		= $(AM_LDADD) $(LIBOBJS)

noinst_HEADERS			= auto_stubs.h				\
				  CFBundle_BinaryTypes.h		\
				  CFBundle_Internal.h			\
				  CFInternal.h				\
				  CFRunLoopPriv.h			\
				  CFUniCharPriv.h			\
				  CFWindowsMessageQueue.h		\
				  CFXMLInputStream.h

CF_includedir			= ${includedir}/CoreFoundation

CF_include_HEADERS		= CFArray.h				\
				  CFBag.h				\
				  CFBase.h				\
				  CFBinaryHeap.h			\
				  CFBitVector.h				\
				  CFBundle.h				\
				  CFBundlePriv.h			\
				  CFByteOrder.h				\
				  CFCalendar.h				\
				  CFCharacterSet.h			\
				  CFCharacterSetPriv.h			\
				  CFData.h				\
				  CFDateFormatter.h			\
				  CFDate.h				\
				  CFDictionary.h			\
				  CFError.h				\
				  CFError_Private.h			\
				  CFLocale.h				\
				  CFLogUtilities.h			\
				  CFMachPort.h				\
				  CFMessagePort.h			\
				  CFNumberFormatter.h			\
				  CFNumber.h				\
				  CFPlugInCOM.h				\
				  CFPlugIn_Factory.h			\
				  CFPlugIn.h				\
				  CFPreferences.h			\
				  CFPriv.h				\
				  CFPropertyList.h			\
				  CFRunLoop.h				\
				  CFRuntime.h				\
				  CFSet.h				\
				  CFSocket.h				\
				  CFStorage.h				\
				  CFStreamAbstract.h			\
				  CFStream.h				\
				  CFStreamInternal.h			\
				  CFStreamPriv.h			\
				  CFStringDefaultEncoding.h		\
				  CFStringEncodingConverterExt.h	\
				  CFStringEncodingConverter.h		\
				  CFStringEncodingConverterPriv.h	\
				  CFStringEncodingExt.h			\
				  CFString.h				\
				  CFTimeZone.h				\
				  CFTree.h				\
				  CFUniChar.h				\
				  CFUnicodeDecomposition.h		\
				  CFUnicodePrecomposition.h		\
				  CFURLAccess.h				\
				  CFURL.h				\
				  CFUserNotification.h			\
				  CFUUID.h				\
				  CFXMLNode.h				\
				  CFXMLParser.h				\
				  CoreFoundation.h			\
				  ForFoundationOnly.h

CF_COMMON_SOURCES		= CFApplicationPreferences.c		\
				  CFArray.c				\
				  CFBag.c				\
				  CFBase.c				\
				  CFBinaryHeap.c			\
				  CFBinaryPList.c			\
				  CFBitVector.c				\
				  CFBuiltinConverters.c			\
				  CFBundle.c				\
				  CFBundle_Resources.c			\
				  CFCalendar.c				\
				  CFCharacterSet.c			\
				  CFConcreteStreams.c			\
				  CFData.c				\
				  CFDateFormatter.c			\
				  CFDate.c				\
				  CFDictionary.c			\
				  CFError.c				\
				  CFFileUtilities.c			\
				  CFLocaleIdentifier.c			\
				  CFLocale.c				\
				  CFMachPort.c				\
				  CFMessagePort.c			\
				  CFNumberFormatter.c			\
				  CFNumber.c				\
				  CFPlatform.c				\
				  CFPlugIn_Factory.c			\
				  CFPlugIn_Instance.c			\
				  CFPlugIn.c				\
				  CFPlugIn_PlugIn.c			\
				  CFPreferences.c			\
				  CFPropertyList.c			\
				  CFRunLoop.c				\
				  CFRuntime.c				\
				  CFSet.c				\
				  CFSocket.c				\
				  CFSocketStream.c			\
				  CFSortFunctions.c			\
				  CFStorage.c				\
				  CFStream.c				\
				  CFStringEncodingConverter.c		\
				  CFStringEncodings.c			\
				  CFString.c				\
				  CFStringScanner.c			\
				  CFStringUtilities.c			\
				  CFSystemDirectories.c			\
				  CFTimeZone.c				\
				  CFTree.c				\
				  CFURL.c				\
				  CFURLAccess.c				\
				  CFUUID.c				\
				  CFUniChar.c				\
				  CFUnicodeDecomposition.c		\
				  CFUnicodePrecomposition.c		\
				  CFUserNotification.c			\
				  CFUtilities.c				\
				  CFVersion.c				\
				  CFWindowsMessageQueue.c		\
				  CFXMLInputStream.c			\
				  CFXMLNode.c				\
				  CFXMLParser.c				\
				  CFXMLPreferencesDomain.c		\
				  CFXMLTree.c				\
				  Cocotron.c

lib_LTLIBRARIES 			= libCoreFoundation.la

libCoreFoundation_la_CPPFLAGS		= $(CF_COMMON_CPPFLAGS)
libCoreFoundation_la_CFLAGS		= $(CF_COMMON_CFLAGS)
libCoreFoundation_la_LDFLAGS		= $(CF_COMMON_LDFLAGS)
libCoreFoundation_la_LIBADD		= $(CF_COMMON_LIBADD)
libCoreFoundation_la_SOURCES		= $(CF_COMMON_SOURCES)

#
# Only generate a debug instance of the library if --enable-debug was
# passed at configuration time.
#

if CF_BUILD_DEBUG
lib_LTLIBRARIES				+= libCoreFoundation_debug.la

libCoreFoundation_debug_la_CPPFLAGS	= $(CF_COMMON_CPPFLAGS) -DDEBUG
libCoreFoundation_debug_la_CFLAGS	= $(CF_COMMON_CFLAGS)
libCoreFoundation_debug_la_LDFLAGS	= $(CF_COMMON_LDFLAGS)
libCoreFoundation_debug_la_LIBADD	= $(CF_COMMON_LIBADD)
libCoreFoundation_debug_la_SOURCES	= $(CF_COMMON_SOURCES)
endif

#
# Only generate a profile instance of the library if --enable-debug was
# passed at configuration time.
#

if CF_BUILD_PROFILE
lib_LTLIBRARIES				+= libCoreFoundation_profile.la

libCoreFoundation_profile_la_CPPFLAGS	= $(CF_COMMON_CPPFLAGS) -DPROFILE
libCoreFoundation_profile_la_CFLAGS	= $(CF_COMMON_CFLAGS) -pg
libCoreFoundation_profile_la_LDFLAGS	= $(CF_COMMON_LDFLAGS)
libCoreFoundation_profile_la_LIBADD	= $(CF_COMMON_LIBADD)
libCoreFoundation_profile_la_SOURCES	= $(CF_COMMON_SOURCES)
endif

EXTRA_DIST			= APPLE_LICENSE	\
				  INSTALL \
				  LICENSE \
				  BuildCFLite \
				  BuildCFLite.windows \
				  CFLite/CFLite.xcodeproj/project.pbxproj \
				  CFLiteLib/CFLiteLib.vcproj \
				  CFLite.sln \
				  CoreFoundation.xcodeproj/project.pbxproj \
				  examples/Allocator/Allocator.vcproj \
				  examples/Allocator/AllocatorExample.c \
				  examples/CFMessagePort/CFMessagePort.webloc \
				  examples/CFMessagePort/client.c \
				  examples/CFMessagePort/server.c \
				  examples/ReadPListExample/ReadPListExample.cpp \
				  examples/ReadPListExample/ReadPListExample.vcproj \
				  examples/ReadPListExample/schema.xml \
				  examples/StringExample/StringExample.c \
				  examples/StringExample/StringExample.vcproj \
				  examples/WritePListExample/WritePListExample.cpp \
				  examples/WritePListExample/WritePListExample.vcproj \
				  examples/WritePListExample/data/expected_schema.xml \
				  icu/bin/icudt36.dll \
				  icu/bin/icuin36.dll \
				  icu/bin/icuuc36.dll \
				  icu/bin/WHAT_IS_THIS.txt \
				  icu/include/unicode/basictz.h \
				  icu/include/unicode/brkiter.h \
				  icu/include/unicode/calendar.h \
				  icu/include/unicode/caniter.h \
				  icu/include/unicode/chariter.h \
				  icu/include/unicode/choicfmt.h \
				  icu/include/unicode/coleitr.h \
				  icu/include/unicode/coll.h \
				  icu/include/unicode/curramt.h \
				  icu/include/unicode/currunit.h \
				  icu/include/unicode/datefmt.h \
				  icu/include/unicode/dbbi.h \
				  icu/include/unicode/dcfmtsym.h \
				  icu/include/unicode/decimfmt.h \
				  icu/include/unicode/docmain.h \
				  icu/include/unicode/dtfmtsym.h \
				  icu/include/unicode/dtintrv.h \
				  icu/include/unicode/dtitvfmt.h \
				  icu/include/unicode/dtitvinf.h \
				  icu/include/unicode/dtptngen.h \
				  icu/include/unicode/dtrule.h \
				  icu/include/unicode/fieldpos.h \
				  icu/include/unicode/fmtable.h \
				  icu/include/unicode/format.h \
				  icu/include/unicode/gregocal.h \
				  icu/include/unicode/locid.h \
				  icu/include/unicode/measfmt.h \
				  icu/include/unicode/measunit.h \
				  icu/include/unicode/measure.h \
				  icu/include/unicode/msgfmt.h \
				  icu/include/unicode/normlzr.h \
				  icu/include/unicode/numfmt.h \
				  icu/include/unicode/parseerr.h \
				  icu/include/unicode/parsepos.h \
				  icu/include/unicode/platform.h \
				  icu/include/unicode/plurfmt.h \
				  icu/include/unicode/plurrule.h \
				  icu/include/unicode/ppalmos.h \
				  icu/include/unicode/putil.h \
				  icu/include/unicode/pwin32.h \
				  icu/include/unicode/rbbi.h \
				  icu/include/unicode/rbnf.h \
				  icu/include/unicode/rbtz.h \
				  icu/include/unicode/regex.h \
				  icu/include/unicode/rep.h \
				  icu/include/unicode/resbund.h \
				  icu/include/unicode/schriter.h \
				  icu/include/unicode/search.h \
				  icu/include/unicode/simpletz.h \
				  icu/include/unicode/smpdtfmt.h \
				  icu/include/unicode/sortkey.h \
				  icu/include/unicode/strenum.h \
				  icu/include/unicode/stsearch.h \
				  icu/include/unicode/symtable.h \
				  icu/include/unicode/tblcoll.h \
				  icu/include/unicode/timezone.h \
				  icu/include/unicode/translit.h \
				  icu/include/unicode/tzrule.h \
				  icu/include/unicode/tztrans.h \
				  icu/include/unicode/ubidi.h \
				  icu/include/unicode/ubrk.h \
				  icu/include/unicode/ucal.h \
				  icu/include/unicode/ucasemap.h \
				  icu/include/unicode/ucat.h \
				  icu/include/unicode/uchar.h \
				  icu/include/unicode/uchriter.h \
				  icu/include/unicode/uclean.h \
				  icu/include/unicode/ucnv_cb.h \
				  icu/include/unicode/ucnv_err.h \
				  icu/include/unicode/ucnv.h \
				  icu/include/unicode/ucoleitr.h \
				  icu/include/unicode/ucol.h \
				  icu/include/unicode/uconfig.h \
				  icu/include/unicode/ucsdet.h \
				  icu/include/unicode/ucurr.h \
				  icu/include/unicode/udata.h \
				  icu/include/unicode/udat.h \
				  icu/include/unicode/udatpg.h \
				  icu/include/unicode/udeprctd.h \
				  icu/include/unicode/udraft.h \
				  icu/include/unicode/uenum.h \
				  icu/include/unicode/uidna.h \
				  icu/include/unicode/uintrnal.h \
				  icu/include/unicode/uiter.h \
				  icu/include/unicode/ulocdata.h \
				  icu/include/unicode/uloc.h \
				  icu/include/unicode/umachine.h \
				  icu/include/unicode/umisc.h \
				  icu/include/unicode/umsg.h \
				  icu/include/unicode/unifilt.h \
				  icu/include/unicode/unifunct.h \
				  icu/include/unicode/unimatch.h \
				  icu/include/unicode/unirepl.h \
				  icu/include/unicode/uniset.h \
				  icu/include/unicode/unistr.h \
				  icu/include/unicode/unorm.h \
				  icu/include/unicode/unum.h \
				  icu/include/unicode/uobject.h \
				  icu/include/unicode/uobslete.h \
				  icu/include/unicode/urbtok.h \
				  icu/include/unicode/uregex.h \
				  icu/include/unicode/urename.h \
				  icu/include/unicode/urep.h \
				  icu/include/unicode/ures.h \
				  icu/include/unicode/uscript.h \
				  icu/include/unicode/usearch.h \
				  icu/include/unicode/uset.h \
				  icu/include/unicode/usetiter.h \
				  icu/include/unicode/ushape.h \
				  icu/include/unicode/usprep.h \
				  icu/include/unicode/ustdio.h \
				  icu/include/unicode/ustream.h \
				  icu/include/unicode/ustring.h \
				  icu/include/unicode/usystem.h \
				  icu/include/unicode/utext.h \
				  icu/include/unicode/utf16.h \
				  icu/include/unicode/utf32.h \
				  icu/include/unicode/utf8.h \
				  icu/include/unicode/utf.h \
				  icu/include/unicode/utf_old.h \
				  icu/include/unicode/utmscale.h \
				  icu/include/unicode/utrace.h \
				  icu/include/unicode/utrans.h \
				  icu/include/unicode/utypes.h \
				  icu/include/unicode/uversion.h \
				  icu/include/unicode/vtzone.h \
				  icu/lib/icuin.lib \
				  icu/lib/icuuc.lib \
				  icu/lib/WHAT_IS_THIS.txt \
				  Info.plist \
				  Makefile \
				  tests/date_test.c \
				  tests/Make_win32.bat \
				  XCode_Configs/ConfigDebug.xcconfig \
				  XCode_Configs/Config_Master.xcconfig \
				  XCode_Configs/ConfigRelease.xcconfig \
				  ${libCoreFoundation_la_data_DATA}


