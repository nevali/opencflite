//
// Apple's "Read a PList" example program.
// Taken from http://developer.apple.com/opensource/cflite.html
//
#include <CoreFoundation/CoreFoundation.h>

void readPropertyListFromFile (void);

const char * kFilename = "./schema.xml";

int main (int argc, const char* argv[]) {
    // Read the plist.
    readPropertyListFromFile ();	

    return 0;
}

void readPropertyListFromFile (void) {
    CFDataRef data = NULL;
		
    FILE* file = fopen (kFilename, "r");

    if (file != NULL) {
        int result = fseek (file, 0, SEEK_END);
        result = ftell (file);
        rewind (file);

        char* buffer = (char*)calloc (1, result);

        if (buffer != NULL) {
            int rc = fread (buffer, result, 1, file);
            if (rc > 0 || !ferror (file)) {
                data = CFDataCreate (NULL, (const UInt8*)buffer, result);
            }

            free (buffer);
        } 

        fclose (file);
    }

    if (data != NULL) {
        CFPropertyListRef propertyList = CFPropertyListCreateFromXMLData (NULL, data, kCFPropertyListImmutable, NULL);

        CFShow (CFSTR ("Property list (as read from file):"));
        CFShow (propertyList);
    }

    CFRelease (data);
}

