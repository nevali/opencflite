xcopy /Y ..\bin\*.dll .

cl /I.. /D"WIN32" /D"_WIN32_WINNT=0x0501" /D"WINVER=0x0501" date_test.c ../lib/CFLite_Debug.lib

