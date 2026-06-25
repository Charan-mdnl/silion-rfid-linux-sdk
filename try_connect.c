/*
 * ✅ STATUS: WORKING
 * This C program is a verified working utility to test the serial connection to the RFID reader.
 */

#include "ModuleReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int hreader;
    int err;
    char *port = "/dev/ttyACM0"; // Default port

    if (argc > 1) {
        port = argv[1];
    }

    printf("Attempting to connect to RFID reader on %s...\n", port);

    // Try to initialize reader with auto-detection/no type specified
    // signature: READER_ERR EXTERN InitReader_Notype(int *hReader, char * src, int antscnt);
    // antscnt = 1 for now (assuming single antenna or not critical for init)
    err = InitReader_Notype(&hreader, port, 1);

    if (err == MT_OK_ERR) {
        printf("SUCCESS: Connected to reader!\n");
        printf("Handle: %d\n", hreader);
        
        printf("SUCCESS: Connected to reader! (Details skipped)\n");
        CloseReader(hreader);
        return 0;

        CloseReader(hreader);
        return 0;
    } else {
        printf("FAILURE: Could not connect to reader. Error code: %d\n", err);
        return 1;
    }
}
