/*
 * Copyright (c) 2008 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*	CFUtilities.c
	Copyright 1998-2002, Apple, Inc. All rights reserved.
	Responsibility: Christopher Kane
*/

#include "CFPriv.h"
#include "CFInternal.h"
#include "CFPriv.h"
#include <CoreFoundation/CFBundle.h>
#include <CoreFoundation/CFURLAccess.h>
#include <CoreFoundation/CFPropertyList.h>
#include <CoreFoundation/CFTimeZone.h>
#include <CoreFoundation/CFCalendar.h>
#if (DEPLOYMENT_TARGET_MACOSX) 
#include <CoreFoundation/CFLogUtilities.h>
#include <asl.h>
#include <sys/uio.h>
#endif
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#if DEPLOYMENT_TARGET_MACOSX
    #include <mach/mach.h>
    #include <pthread.h>
    #include <mach-o/loader.h>
    #include <mach-o/dyld.h>
    #include <crt_externs.h>
    #include <dlfcn.h>
    #include <vproc.h>
    #include <vproc_priv.h>
    #include <sys/stat.h>
#include <unistd.h>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <stdio.h>
#endif
#if DEPLOYMENT_TARGET_LINUX || DEPLOYMENT_TARGET_FREEBSD
    #include <string.h>
    #include <pthread.h>
#elif DEPLOYMENT_TARGET_WIN32
    #include <windows.h>
    #include <process.h>
    #define getpid _getpid
#endif

#if DEPLOYMENT_TARGET_MACOSX
typedef void* (*THREAD_FUN_TYPE)(void*);
#endif

/* Comparator is passed the address of the values. */
/* Binary searches a sorted-increasing array of some type.
   Return value is either 1) the index of the element desired,
   if the target value exists in the list, 2) greater than or
   equal to count, if the element is greater than all the values
   in the list, or 3) the index of the element greater than the
   target value.

   For example, a search in the list of integers:
	2 3 5 7 11 13 17

   For...		Will Return...
	2		    0
   	5		    2
	23		    7
	1		    0
	9		    4

   For instance, if you just care about found/not found:
   index = CFBSearch(list, count, elem);
   if (count <= index || list[index] != elem) {
   	* Not found *
   } else {
   	* Found *
   }
   
*/
__private_extern__ CFIndex CFBSearch(const void *element, CFIndex elementSize, const void *list, CFIndex count, CFComparatorFunction comparator, void *context) {
    const char *ptr = (const char *)list;
    while (0 < count) {
        CFIndex half = count / 2;
        const char *probe = ptr + elementSize * half;
        CFComparisonResult cr = comparator(element, probe, context);
	if (0 == cr) return (probe - (const char *)list) / elementSize;
        ptr = (cr < 0) ? ptr : probe + elementSize;
        count = (cr < 0) ? half : (half + (count & 1) - 1);
    }
    return (ptr - (const char *)list) / elementSize;
}


#define ELF_STEP(B) T1 = (H << 4) + B; T2 = T1 & 0xF0000000; if (T2) T1 ^= (T2 >> 24); T1 &= (~T2); H = T1;

CFHashCode CFHashBytes(uint8_t *bytes, CFIndex length) {
    /* The ELF hash algorithm, used in the ELF object file format */
    UInt32 H = 0, T1, T2;
    SInt32 rem = length;
    while (3 < rem) {
	ELF_STEP(bytes[length - rem]);
	ELF_STEP(bytes[length - rem + 1]);
	ELF_STEP(bytes[length - rem + 2]);
	ELF_STEP(bytes[length - rem + 3]);
	rem -= 4;
    }
    switch (rem) {
    case 3:  ELF_STEP(bytes[length - 3]);
    case 2:  ELF_STEP(bytes[length - 2]);
    case 1:  ELF_STEP(bytes[length - 1]);
    case 0:  ;
    }
    return H;
}

#undef ELF_STEP


#if DEPLOYMENT_TARGET_MACOSX
__private_extern__ uintptr_t __CFFindPointer(uintptr_t ptr, uintptr_t start) {
    vm_map_t task = mach_task_self();
    mach_vm_address_t address = start;
    for (;;) {
	mach_vm_size_t size = 0;
	vm_region_basic_info_data_64_t info;
        mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
	mach_port_t object_name;
        kern_return_t ret = mach_vm_region(task, &address, &size, VM_REGION_BASIC_INFO_64, (vm_region_info_t)&info, &count, &object_name);
        if (KERN_SUCCESS != ret) break;
	boolean_t scan = (info.protection & VM_PROT_WRITE) ? 1 : 0;
	if (scan) {
	    uintptr_t *addr = (uintptr_t *)((uintptr_t)address);
	    uintptr_t *end = (uintptr_t *)((uintptr_t)address + (uintptr_t)size);
	    while (addr < end) {
	        if ((uintptr_t *)start <= addr && *addr == ptr) {
		    return (uintptr_t)addr;
	        }
	        addr++;
	    }
	}
        address += size;
    }
    return 0;
}
#endif

#if DEPLOYMENT_TARGET_WIN32
typedef struct _args {
    void *func;
    void *arg;
    HANDLE handle;
};

static unsigned int __stdcall __CFWinThreadFunc(void *arg) {
    struct _args *args = (struct _args*)arg;
    ((void (*)(void *))args->func)(args->arg);
    CloseHandle(args->handle);
    CFAllocatorDeallocate(kCFAllocatorSystemDefault, arg);
    _endthreadex(0);
    return 0;
}
#endif

__private_extern__ void *__CFStartSimpleThread(void *func, void *arg) {
#if DEPLOYMENT_TARGET_MACOSX || DEPLOYMENT_TARGET_LINUX || DEPLOYMENT_TARGET_FREEBSD
    pthread_attr_t attr;
    pthread_t tid = 0;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 60 * 1024);	// 60K stack for our internal threads is sufficient
    OSMemoryBarrier(); // ensure arg is fully initialized and set in memory
    pthread_create(&tid, &attr, (THREAD_FUN_TYPE)func, arg);
    pthread_attr_destroy(&attr);
//warning CF: we dont actually know that a pthread_t is the same size as void *
    return (void *)tid;
#else
    unsigned tid;
    struct _args *args = (struct _args*)CFAllocatorAllocate(kCFAllocatorSystemDefault, sizeof(struct _args), 0);
    if (__CFOASafe) __CFSetLastAllocationEventName(args, "CFUtilities (thread-args)");
    HANDLE handle;
    args->func = func;
    args->arg = arg;
    /* The thread is created suspended, because otherwise there would be a race between the assignment below of the handle field, and it's possible use in the thread func above. */
    args->handle = (HANDLE)_beginthreadex(NULL, 0, __CFWinThreadFunc, args, CREATE_SUSPENDED, &tid);
    handle = args->handle;
    ResumeThread(handle);
    return handle;
#endif
}

__private_extern__ CFStringRef _CFCreateLimitedUniqueString() {
    /* this unique string is only unique to the current host during the current boot */
    uint64_t tsr = __CFReadTSR();
    UInt32 tsrh = (UInt32)(tsr >> 32), tsrl = (UInt32)(tsr & (int64_t)0xFFFFFFFF);
    return CFStringCreateWithFormat(kCFAllocatorSystemDefault, NULL, CFSTR("CFUniqueString-%lu%lu$"), tsrh, tsrl);
}


// Looks for localized version of "nonLocalized" in the SystemVersion bundle
// If not found, and returnNonLocalizedFlag == true, will return the non localized string (retained of course), otherwise NULL
// If bundlePtr != NULL, will use *bundlePtr and will return the bundle in there; otherwise bundle is created and released

static CFStringRef _CFCopyLocalizedVersionKey(CFBundleRef *bundlePtr, CFStringRef nonLocalized) {
    CFStringRef localized = NULL;
    CFBundleRef locBundle = bundlePtr ? *bundlePtr : NULL;
    if (!locBundle) {
        CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorSystemDefault, CFSTR("/System/Library/CoreServices/SystemVersion.bundle"), kCFURLPOSIXPathStyle, false);
        if (url) {
            locBundle = CFBundleCreate(kCFAllocatorSystemDefault, url);
            CFRelease(url);
        }
    }
    if (locBundle) {
	localized = CFBundleCopyLocalizedString(locBundle, nonLocalized, nonLocalized, CFSTR("SystemVersion"));
	if (bundlePtr) *bundlePtr = locBundle; else CFRelease(locBundle);
    }
    return localized ? localized : (CFStringRef)CFRetain(nonLocalized);
}

static CFDictionaryRef _CFCopyVersionDictionary(CFStringRef path) {
    CFPropertyListRef plist = NULL;
    CFDataRef data;
    CFURLRef url;

    url = CFURLCreateWithFileSystemPath(kCFAllocatorSystemDefault, path, kCFURLPOSIXPathStyle, false);
    if (url && CFURLCreateDataAndPropertiesFromResource(kCFAllocatorSystemDefault, url, &data, NULL, NULL, NULL)) {
	plist = CFPropertyListCreateFromXMLData(kCFAllocatorSystemDefault, data, kCFPropertyListMutableContainers, NULL);
	CFRelease(data);
    }
    if (url) CFRelease(url);
    
    if (plist) {
#if DEPLOYMENT_TARGET_MACOSX
	CFBundleRef locBundle = NULL;
	CFStringRef fullVersion, vers, versExtra, build;
	CFStringRef versionString = _CFCopyLocalizedVersionKey(&locBundle, _kCFSystemVersionProductVersionStringKey);
	CFStringRef buildString = _CFCopyLocalizedVersionKey(&locBundle, _kCFSystemVersionBuildStringKey);
	CFStringRef fullVersionString = _CFCopyLocalizedVersionKey(&locBundle, CFSTR("FullVersionString"));
	if (locBundle) CFRelease(locBundle);

        // Now build the full version string
        if (CFEqual(fullVersionString, CFSTR("FullVersionString"))) {
            CFRelease(fullVersionString);
            fullVersionString = CFStringCreateWithFormat(kCFAllocatorSystemDefault, NULL, CFSTR("%@ %%@ (%@ %%@)"), versionString, buildString);
        }
        vers = (CFStringRef)CFDictionaryGetValue((CFDictionaryRef)plist, _kCFSystemVersionProductVersionKey);
        versExtra = (CFStringRef)CFDictionaryGetValue((CFDictionaryRef)plist, _kCFSystemVersionProductVersionExtraKey);
        if (vers && versExtra) vers = CFStringCreateWithFormat(kCFAllocatorSystemDefault, NULL, CFSTR("%@ %@"), vers, versExtra);
        build = (CFStringRef)CFDictionaryGetValue((CFDictionaryRef)plist, _kCFSystemVersionBuildVersionKey);
        fullVersion = CFStringCreateWithFormat(kCFAllocatorSystemDefault, NULL, fullVersionString, (vers ? vers : CFSTR("?")), build ? build : CFSTR("?"));
        if (vers && versExtra) CFRelease(vers);
        
	CFDictionarySetValue((CFMutableDictionaryRef)plist, _kCFSystemVersionProductVersionStringKey, versionString);
	CFDictionarySetValue((CFMutableDictionaryRef)plist, _kCFSystemVersionBuildStringKey, buildString);
	CFDictionarySetValue((CFMutableDictionaryRef)plist, CFSTR("FullVersionString"), fullVersion);
 	CFRelease(versionString);
	CFRelease(buildString);
	CFRelease(fullVersionString);
        CFRelease(fullVersion);
#endif
    }    
    return (CFDictionaryRef)plist;
}

#if defined (__MACH__) || 0
CFStringRef CFCopySystemVersionString(void) {
    CFStringRef versionString;
    CFDictionaryRef dict = _CFCopyServerVersionDictionary();
    if (!dict) dict = _CFCopySystemVersionDictionary();
    versionString = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("FullVersionString"));
    if (versionString) CFRetain(versionString);
    CFRelease(dict);
    return versionString;
}

// Obsolete: These two functions cache the dictionaries to avoid calling _CFCopyVersionDictionary() more than once per dict desired
// In fact, they do not cache any more, because the file can change after
// apps are running in some situations, and apps need the new info.
// Proper caching and testing to see if the file has changed, without race
// conditions, would require semi-convoluted use of fstat().

CFDictionaryRef _CFCopySystemVersionDictionary(void) {
    CFPropertyListRef plist = NULL;
    plist = _CFCopyVersionDictionary(CFSTR("/System/Library/CoreServices/SystemVersion.plist"));
    return (CFDictionaryRef)plist;
}

CFDictionaryRef _CFCopyServerVersionDictionary(void) {
    CFPropertyListRef plist = NULL;
    plist = _CFCopyVersionDictionary(CFSTR("/System/Library/CoreServices/ServerVersion.plist"));
    return (CFDictionaryRef)plist;
}

CONST_STRING_DECL(_kCFSystemVersionProductNameKey, "ProductName")
CONST_STRING_DECL(_kCFSystemVersionProductCopyrightKey, "ProductCopyright")
CONST_STRING_DECL(_kCFSystemVersionProductVersionKey, "ProductVersion")
CONST_STRING_DECL(_kCFSystemVersionProductVersionExtraKey, "ProductVersionExtra")
CONST_STRING_DECL(_kCFSystemVersionProductUserVisibleVersionKey, "ProductUserVisibleVersion")
CONST_STRING_DECL(_kCFSystemVersionBuildVersionKey, "ProductBuildVersion")
CONST_STRING_DECL(_kCFSystemVersionProductVersionStringKey, "Version")
CONST_STRING_DECL(_kCFSystemVersionBuildStringKey, "Build")

typedef struct {
    uint16_t    primaryVersion;
    uint8_t     secondaryVersion;
    uint8_t     tertiaryVersion;
} CFLibraryVersion;

CFLibraryVersion CFGetExecutableLinkedLibraryVersion(CFStringRef libraryName) {
    CFLibraryVersion ret = {0xFFFF, 0xFF, 0xFF};
    char library[CFMaxPathSize];	// search specs larger than this are pointless
    if (!CFStringGetCString(libraryName, library, sizeof(library), kCFStringEncodingUTF8)) return ret;
    int32_t version = NSVersionOfLinkTimeLibrary(library);
    if (-1 != version) {
	ret.primaryVersion = version >> 16;
	ret.secondaryVersion = (version >> 8) & 0xff;
	ret.tertiaryVersion = version & 0xff;
    }
    return ret;
}

CFLibraryVersion CFGetExecutingLibraryVersion(CFStringRef libraryName) {
    CFLibraryVersion ret = {0xFFFF, 0xFF, 0xFF};
    char library[CFMaxPathSize];	// search specs larger than this are pointless
    if (!CFStringGetCString(libraryName, library, sizeof(library), kCFStringEncodingUTF8)) return ret;
    int32_t version = NSVersionOfRunTimeLibrary(library);
    if (-1 != version) {
	ret.primaryVersion = version >> 16;
	ret.secondaryVersion = (version >> 8) & 0xff;
	ret.tertiaryVersion = version & 0xff;
    }
    return ret;
}

/*
If
   (vers != 0xFFFF): We know the version number of the library this app was linked against
   and (versionInfo[version].VERSIONFIELD != 0xFFFF): And we know what version number started the specified release
   and ((version == 0) || (versionInfo[version-1].VERSIONFIELD < versionInfo[version].VERSIONFIELD)): And it's distinct from the prev release
Then
   If the version the app is linked against is less than the version recorded for the specified release
   Then stop checking and return false
   Else stop checking and return YES
Else
   Continue checking (the next library)
*/
#define checkLibrary(LIBNAME, VERSIONFIELD) \
    {uint16_t vers = (NSVersionOfLinkTimeLibrary(LIBNAME) >> 16); \
     if ((vers != 0xFFFF) && (versionInfo[version].VERSIONFIELD != 0xFFFF) && ((version == 0) || (versionInfo[version-1].VERSIONFIELD < versionInfo[version].VERSIONFIELD))) return (results[version] = ((vers < versionInfo[version].VERSIONFIELD) ? false : true)); }

CF_EXPORT Boolean _CFExecutableLinkedOnOrAfter(CFSystemVersion version) {
    // The numbers in the below table should be the numbers for any version of the framework in the release.
    // When adding new entries to this table for a new build train, it's simplest to use the versions of the
    // first new versions of projects submitted to the new train. These can later be updated. One thing to watch for is that software updates
    // for the previous release do not increase numbers beyond the number used for the next release!
    // For a given train, don't ever use the last versions submitted to the previous train! (This to assure room for software updates.)
    // If versions are the same as previous release, use 0xFFFF; this will assure the answer is a conservative NO.
    // NOTE: Also update the CFM check below, perhaps to the previous release... (???)
    static const struct {
        uint16_t libSystemVersion;
        uint16_t cocoaVersion;
        uint16_t appkitVersion;
        uint16_t fouVersion;
        uint16_t cfVersion;
        uint16_t carbonVersion;
        uint16_t applicationServicesVersion;
        uint16_t coreServicesVersion;
        uint16_t iokitVersion;
    } versionInfo[] = {
	{50, 5, 577, 397, 196, 113, 16, 9, 52},		/* CFSystemVersionCheetah (used the last versions) */
	{55, 7, 620, 425, 226, 122, 16, 10, 67},	/* CFSystemVersionPuma (used the last versions) */
        {56, 8, 631, 431, 232, 122, 17, 11, 73},	/* CFSystemVersionJaguar */
        {67, 9, 704, 481, 281, 126, 19, 16, 159},	/* CFSystemVersionPanther */
        {73, 10, 750, 505, 305, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},	/* CFSystemVersionTiger */
        {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},	/* CFSystemVersionChablis */
    };
    static char results[CFSystemVersionMax] = {-2, -2, -2, -2, -2, -2};	/* We cache the results per-release; there are only a few of these... */
    if (version >= CFSystemVersionMax) return false;	/* Actually, we don't know the answer, and something scary is going on */
    if (results[version] != -2) return results[version];

    if (_CFIsCFM()) {
        results[version] = (version <= CFSystemVersionJaguar) ? true : false;
        return results[version];
    }
    
    checkLibrary("System", libSystemVersion);	// Pretty much everyone links with this
    checkLibrary("Cocoa", cocoaVersion);
    checkLibrary("AppKit", appkitVersion);
    checkLibrary("Foundation", fouVersion);
    checkLibrary("CoreFoundation", cfVersion);
    checkLibrary("Carbon", carbonVersion);
    checkLibrary("ApplicationServices", applicationServicesVersion);
    checkLibrary("CoreServices", coreServicesVersion);
    checkLibrary("IOKit", iokitVersion);

    /* If not found, then simply return NO to indicate earlier --- compatibility by default, unfortunately */
    return false;
}
#else
CF_EXPORT Boolean _CFExecutableLinkedOnOrAfter(CFSystemVersion version) {
    return true;
}
#endif

#if DEPLOYMENT_TARGET_MACOSX
__private_extern__ void *__CFLookupCFNetworkFunction(const char *name) {
    static void *image = NULL;
    if (NULL == image) {
	const char *path = NULL;
	if (!issetugid()) {
	    path = getenv("CFNETWORK_LIBRARY_PATH");
	}
	if (!path) {
	    path = "/System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/CFNetwork.framework/Versions/A/CFNetwork";
	}
	image = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
    }
    void *dyfunc = NULL;
    if (image) {
	dyfunc = dlsym(image, name);
    }
    return dyfunc;
}
#endif //__MACH__


#ifndef __CFGetSessionID_defined

__private_extern__ uint32_t __CFGetSessionID(void) {
    return 0;
}

#endif

const char *_CFPrintForDebugger(const void *obj) {
	static char *result = NULL;
	CFStringRef str;
	CFIndex cnt = 0;

	free(result);	// Let go of result from previous call.
	result = NULL;
	if (obj) {
		if (CFGetTypeID(obj) == CFStringGetTypeID()) {
			// Makes Ali marginally happier
			str = __CFCopyFormattingDescription(obj, NULL);
			if (!str) str = CFCopyDescription(obj);
		} else {
			str = CFCopyDescription(obj);
		}
	} else {
		str = (CFStringRef)CFRetain(CFSTR("(null)"));
	}
	
	if (str != NULL) {
		CFStringGetBytes(str, CFRangeMake(0, CFStringGetLength(str)), kCFStringEncodingUTF8, 0, FALSE, NULL, 0, &cnt);
	}
	result = (char *) malloc(cnt + 2);	// 1 for '\0', 1 for an optional '\n'
	if (str != NULL) {
		CFStringGetBytes(str, CFRangeMake(0, CFStringGetLength(str)), kCFStringEncodingUTF8, 0, FALSE, (UInt8 *) result, cnt, &cnt);
	}
	result[cnt] = '\0';

	if (str) CFRelease(str);
	return result;
}

static void _CFShowToFile(FILE *file, Boolean flush, const void *obj) {
     CFStringRef str;
     CFIndex idx, cnt;
     CFStringInlineBuffer buffer;
     bool lastNL = false;

     if (obj) {
	if (CFGetTypeID(obj) == CFStringGetTypeID()) {
	    // Makes Ali marginally happier
	    str = __CFCopyFormattingDescription(obj, NULL);
	    if (!str) str = CFCopyDescription(obj);
	} else {
	    str = CFCopyDescription(obj);
	}
     } else {
	str = (CFStringRef)CFRetain(CFSTR("(null)"));
     }
     cnt = CFStringGetLength(str);

     // iTunes used OutputDebugStringW(theString);

     CFStringInitInlineBuffer(str, &buffer, CFRangeMake(0, cnt));
#if DEPLOYMENT_TARGET_WIN32
    TCHAR *accumulatedBuffer = (TCHAR *)malloc((cnt+1) * sizeof(TCHAR));
#endif
     for (idx = 0; idx < cnt; idx++) {
         UniChar ch = __CFStringGetCharacterFromInlineBufferQuick(&buffer, idx);

#if DEPLOYMENT_TARGET_MACOSX
		 if (ch < 128) {
             fprintf_l(file, NULL, "%c", ch);
	     lastNL = (ch == '\n');
         } else {
             fprintf_l(file, NULL, "\\u%04x", ch);
         }
#elif DEPLOYMENT_TARGET_WIN32
		 if (ch < 128) {
             fprintf(file, "%c", ch);
	     lastNL = (ch == '\n');
         } else {
             fprintf(file, "\\u%04x", ch);
         }
#endif
     }
     if (!lastNL) {
#if DEPLOYMENT_TARGET_MACOSX
         fprintf_l(file, NULL, "\n");
#endif
         if (flush) fflush(file);
     }

     if (str) CFRelease(str);
}

void CFShow(const void *obj) {
     _CFShowToFile(stderr, true, obj);
}



void CFLog(int32_t lev, CFStringRef format, ...) {
    CFStringRef result;
    va_list argList;
    static CFSpinLock_t lock = CFSpinLockInit;

    va_start(argList, format);
    result = CFStringCreateWithFormatAndArguments(kCFAllocatorSystemDefault, NULL, format, argList);
    va_end(argList);

    __CFSpinLock(&lock); 
    CFTimeZoneRef tz = CFTimeZoneCopySystem();	// specifically choose system time zone for logs
    CFGregorianDate gdate = CFAbsoluteTimeGetGregorianDate(CFAbsoluteTimeGetCurrent(), tz);
    CFRelease(tz);
    gdate.second = gdate.second + 0.0005;
    // Date format: YYYY '-' MM '-' DD ' ' hh ':' mm ':' ss.fff
#if DEPLOYMENT_TARGET_WIN32
    printf("%04d-%02d-%02d %02d:%02d:%06.3f %s[%d] CFLog (%d): ", (int)gdate.year, gdate.month, gdate.day, gdate.hour, gdate.minute, gdate.second, *_CFGetProgname(), getpid(), lev);
#else
    fprintf_l(stderr, NULL, "%04d-%02d-%02d %02d:%02d:%06.3f %s[%d:%x] CFLog: ", (int)gdate.year, gdate.month, gdate.day, gdate.hour, gdate.minute, gdate.second, *_CFGetProgname(), getpid(), pthread_mach_thread_np(pthread_self()));
#endif
    CFShow(result);

    __CFSpinUnlock(&lock); 
    CFRelease(result);
}


