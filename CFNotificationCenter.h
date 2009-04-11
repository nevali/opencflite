/*
 *	CFNotificationCenter.h
 *
 *	Stuart Crook, 26/2/09
 */

#if !defined(__COREFOUNDATION_CFNOTIFICATIONCENTER__)
#define __COREFOUNDATION_CFNOTIFICATIONCENTER__ 1

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFDictionary.h>

CF_EXTERN_C_BEGIN

typedef struct __CFNotificationCenter * CFNotificationCenterRef;

typedef void (*CFNotificationCallback)(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo);

enum {
    CFNotificationSuspensionBehaviorDrop = 1,
    CFNotificationSuspensionBehaviorCoalesce = 2,
    CFNotificationSuspensionBehaviorHold = 3,
    CFNotificationSuspensionBehaviorDeliverImmediately = 4
};
typedef CFIndex CFNotificationSuspensionBehavior;

CF_EXPORT CFTypeID CFNotificationCenterGetTypeID(void);

CF_EXPORT CFNotificationCenterRef CFNotificationCenterGetLocalCenter(void) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

CF_EXPORT CFNotificationCenterRef CFNotificationCenterGetDistributedCenter(void);

CF_EXPORT CFNotificationCenterRef CFNotificationCenterGetDarwinNotifyCenter(void) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

CF_EXPORT void CFNotificationCenterAddObserver(CFNotificationCenterRef center, const void *observer, CFNotificationCallback callBack, CFStringRef name, const void *object, CFNotificationSuspensionBehavior suspensionBehavior);

CF_EXPORT void CFNotificationCenterRemoveObserver(CFNotificationCenterRef center, const void *observer, CFStringRef name, const void *object);
CF_EXPORT void CFNotificationCenterRemoveEveryObserver(CFNotificationCenterRef center, const void *observer);

CF_EXPORT void CFNotificationCenterPostNotification(CFNotificationCenterRef center, CFStringRef name, const void *object, CFDictionaryRef userInfo, Boolean deliverImmediately);

#if MAC_OS_X_VERSION_10_3 <= MAC_OS_X_VERSION_MAX_ALLOWED

enum {
    kCFNotificationDeliverImmediately = (1 << 0),
    kCFNotificationPostToAllSessions = (1 << 1)
};

void CFNotificationCenterPostNotificationWithOptions(CFNotificationCenterRef center, CFStringRef name, const void *object, CFDictionaryRef userInfo, CFOptionFlags options) AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

#endif


CF_EXTERN_C_END

#endif /* ! __COREFOUNDATION_CFNOTIFICATIONCENTER__ */

