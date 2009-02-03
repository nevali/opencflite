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

CS_DATA_DIR			= ${datadir}/CoreServices

CF_COMMON_CPPFLAGS		= $(AM_CPPFLAGS)			\
				  -DU_SHOW_DRAFT_API=1			\
				  -DCF_BUILDING_CF=1			\
				  -D__kCFCharacterSetDir=\"${CS_DATA_DIR}\" \
				  -DMAC_OS_X_VERSION_MAX_ALLOWED=MAC_OS_X_VERSION_10_5	\
				  -I$(top_srcdir)/include		\
				  -I$(top_srcdir)/include/mach_support

CF_COMMON_CFLAGS		= $(AM_CFLAGS)

CF_COMMON_LDFLAGS		= $(AM_LDFLAGS)				\
				  -version-info $(CF_VERSION_INFO)

CF_COMMON_LIBADD		= $(AM_LDADD) $(LIBOBJS)

CF_datadir			= ${CS_DATA_DIR}

CF_data_DATA			= source/CFCharacterSetBitmaps.bitmap	\
				  source/CFUniCharPropertyDatabase.data \
				  source/CFUnicodeData-B.mapping	\
				  source/CFUnicodeData-L.mapping

noinst_HEADERS			= source/auto_stubs.h			\
				  source/CFBundle_BinaryTypes.h		\
				  source/CFBundle_Internal.h		\
				  source/CFInternal.h			\
				  source/CFRunLoopPriv.h		\
				  source/CFUniCharPriv.h		\
				  source/CFWindowsMessageQueue.h	\
				  source/CFXMLInputStream.h

CF_includedir			= ${includedir}/CoreFoundation

CF_include_HEADERS		= source/CFArray.h			\
				  source/CFBag.h			\
				  source/CFBase.h			\
				  source/CFBinaryHeap.h			\
				  source/CFBitVector.h			\
				  source/CFBundle.h			\
				  source/CFBundlePriv.h			\
				  source/CFByteOrder.h			\
				  source/CFCalendar.h			\
				  source/CFCharacterSet.h		\
				  source/CFCharacterSetPriv.h		\
				  source/CFData.h			\
				  source/CFDateFormatter.h		\
				  source/CFDate.h			\
				  source/CFDictionary.h			\
				  source/CFError.h			\
				  source/CFError_Private.h		\
				  source/CFLocale.h			\
				  source/CFLogUtilities.h		\
				  source/CFMachPort.h			\
				  source/CFMessagePort.h		\
				  source/CFNumberFormatter.h		\
				  source/CFNumber.h			\
				  source/CFPlugInCOM.h			\
				  source/CFPlugIn_Factory.h		\
				  source/CFPlugIn.h			\
				  source/CFPreferences.h		\
				  source/CFPriv.h			\
				  source/CFPropertyList.h		\
				  source/CFRunLoop.h			\
				  source/CFRuntime.h			\
				  source/CFSet.h			\
				  source/CFSocket.h			\
				  source/CFStorage.h			\
				  source/CFStreamAbstract.h		\
				  source/CFStream.h			\
				  source/CFStreamInternal.h		\
				  source/CFStreamPriv.h			\
				  source/CFStringDefaultEncoding.h	\
				  source/CFStringEncodingConverterExt.h	\
				  source/CFStringEncodingConverter.h	\
				  source/CFStringEncodingConverterPriv.h\
				  source/CFStringEncodingExt.h		\
				  source/CFString.h			\
				  source/CFTimeZone.h			\
				  source/CFTree.h			\
				  source/CFUniChar.h			\
				  source/CFUnicodeDecomposition.h	\
				  source/CFUnicodePrecomposition.h	\
				  source/CFURLAccess.h			\
				  source/CFURL.h			\
				  source/CFUserNotification.h		\
				  source/CFUUID.h			\
				  source/CFXMLNode.h			\
				  source/CFXMLParser.h			\
				  source/CoreFoundation.h		\
				  source/ForFoundationOnly.h

CF_COMMON_SOURCES		= source/CFApplicationPreferences.c	\
				  source/CFArray.c			\
				  source/CFBag.c			\
				  source/CFBase.c			\
				  source/CFBinaryHeap.c			\
				  source/CFBinaryPList.c		\
				  source/CFBitVector.c			\
				  source/CFBuiltinConverters.c		\
				  source/CFBundle.c			\
				  source/CFBundle_Resources.c		\
				  source/CFCalendar.c			\
				  source/CFCharacterSet.c		\
				  source/CFConcreteStreams.c		\
				  source/CFData.c			\
				  source/CFDateFormatter.c		\
				  source/CFDate.c			\
				  source/CFDictionary.c			\
				  source/CFError.c			\
				  source/CFFileUtilities.c		\
				  source/CFLocaleIdentifier.c		\
				  source/CFLocale.c			\
				  source/CFMachPort.c			\
				  source/CFMessagePort.c		\
				  source/CFNumberFormatter.c		\
				  source/CFNumber.c			\
				  source/CFPlatform.c			\
				  source/CFPlugIn_Factory.c		\
				  source/CFPlugIn_Instance.c		\
				  source/CFPlugIn.c			\
				  source/CFPlugIn_PlugIn.c		\
				  source/CFPreferences.c		\
				  source/CFPropertyList.c		\
				  source/CFRunLoop.c			\
				  source/CFRuntime.c			\
				  source/CFSet.c			\
				  source/CFSocket.c			\
				  source/CFSocketStream.c		\
				  source/CFSortFunctions.c		\
				  source/CFStorage.c			\
				  source/CFStream.c			\
				  source/CFStringEncodingConverter.c	\
				  source/CFStringEncodings.c		\
				  source/CFString.c			\
				  source/CFStringScanner.c		\
				  source/CFStringUtilities.c		\
				  source/CFSystemDirectories.c		\
				  source/CFTimeZone.c			\
				  source/CFTree.c			\
				  source/CFUniChar.c			\
				  source/CFUnicodeDecomposition.c	\
				  source/CFUnicodePrecomposition.c	\
				  source/CFURLAccess.c			\
				  source/CFUserNotification.c		\
				  source/CFUtilities.c			\
				  source/CFUUID.c			\
				  source/CFVersion.c			\
				  source/CFWindowsMessageQueue.c	\
				  source/CFXMLInputStream.c		\
				  source/CFXMLNode.c			\
				  source/CFXMLParser.c			\
				  source/CFXMLPreferencesDomain.c	\
				  source/CFXMLTree.c			\
				  source/Cocotron.c

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
