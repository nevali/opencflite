/*
 *    Copyright (c) 2009 Nuovation System Designs, LLC
 *    All rights reserved.
 *
 *    Description:
 *      This file implements a trivial CFRunLoopTimer example that
 *      fires a timer every T seconds for up to L seconds.
 */

#include <stdbool.h>
#include <stdio.h>
#if !defined(_WIN32)
#include <unistd.h>
#endif

#include <AssertMacros.h>

#include <CoreFoundation/CoreFoundation.h>

/* Type Definitions */

typedef struct _TimerData {
	int mIterations;
} TimerData;

/*
 *  void TimerCallback()
 *
 *  Description:
 *    This routine is the callback handler for a CFRunLoopTimer. It
 *    simply prints out the time of day--in the current system time
 *    zone--and the iteration number and then increments the iteration
 *    number.
 *
 *  Input(s):
 *    timer - A CoreFoundation run loop timer reference to the timer
 *            associated with this callback.
 *    info  - A pointer to the callback data associated with this time
 *            when it was created. In this case, the iteration state.
 *
 *  Output(s):
 *    info  - The callback state with the iteration count incremented by
 *            one.
 *
 *  Returns:
 *    N/A
 *
 */
static void
TimerCallback(CFRunLoopTimerRef timer, void *info)
{
	TimerData *theData = info;
    CFTimeZoneRef tz = NULL;
    CFGregorianDate theDate;

	(void)timer;

	tz = CFTimeZoneCopySystem();
	require(tz != NULL, done);

	theDate = CFAbsoluteTimeGetGregorianDate(CFAbsoluteTimeGetCurrent(), tz);

	printf("%04ld-%02d-%02d %02d:%02d:%06.3f, iteration: %d\n",
		   theDate.year, theDate.month, theDate.day,
		   theDate.hour, theDate.minute, theDate.second,
		   theData->mIterations++);

	CFRelease(tz);

 done:
	return;
}

int
main(int argc, const char * const argv[])
{
	bool status = false;
	double timerInterval = 0;
	double timerLimit = 0;
	long timerIterations = 0;
	char *end = NULL;
	TimerData theData = { 0 };
    CFRunLoopTimerRef theTimer = NULL;
    CFRunLoopTimerContext theContext = { 0, &theData, NULL, NULL, NULL };
	CFStringRef theMode = CFSTR("TimerMode");

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <interval / s> <limit / s>\n", argv[0]);
		goto done;
	}

	timerInterval = strtod(argv[1], &end);
	verify(errno != ERANGE);

	if (errno != 0 || timerInterval <= 0) {
		fprintf(stderr, "Timer interval must be greater than zero.\n");
		goto done;
	}

	timerLimit = strtod(argv[2], &end);
	verify(errno != ERANGE);

	if (errno != 0 || timerLimit <= 0) {
		fprintf(stderr, "Timer limit must be greater than zero.\n");
		goto done;
	}

	timerIterations = timerLimit / timerInterval;

	printf("Will fire the timer every %f seconds for %f seconds, "
		   "up to %ld time%s.\n",
		   timerInterval, timerLimit, timerIterations,
		   timerIterations == 1 ? "" : "s");

	theTimer = CFRunLoopTimerCreate(kCFAllocatorDefault,
									CFAbsoluteTimeGetCurrent() + timerInterval,
									timerInterval,
									0,
									0,
									TimerCallback,
									&theContext);
	require(theTimer != NULL, done);

	CFRunLoopAddTimer(CFRunLoopGetCurrent(),
					  theTimer,
					  theMode);
	CFRunLoopRunInMode(theMode, timerLimit, false);

	CFRunLoopRun();

	CFRunLoopTimerInvalidate(theTimer);
	CFRelease(theTimer);

	status = (theData.mIterations == timerIterations);

 done:
	return (status ? EXIT_SUCCESS : EXIT_FAILURE);
}
