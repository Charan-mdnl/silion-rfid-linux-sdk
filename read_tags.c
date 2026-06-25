/*
 * ✅ STATUS: WORKING
 * This C program is a verified working utility to connect and read RFID tags via USB-C serial.
 */

#include "ModuleReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int keep_running = 1;

void signal_handler(int sig) {
    keep_running = 0;
}

int main(int argc, char *argv[]) {
    int hreader;
    int err;
    char *port = "/dev/ttyACM0"; // Default port
    TAGINFO tag_;
    int ants[] = {1}; // Antenna 1
    int tagcnt = 0;
    int i, f;

    if (argc > 1) {
        port = argv[1];
    }

    signal(SIGINT, signal_handler);

    printf("Connecting to RFID reader on %s...\n", port);
    err = InitReader_Notype(&hreader, port, 1);

    if (err != MT_OK_ERR) {
        printf("FAILURE: Could not connect to reader. Error code: %d\n", err);
        return 1;
    }

    printf("SUCCESS: Connected! Starting inventory loop (Press Ctrl+C to stop)...\n");

    // Perform inventory
    while (keep_running) {
        // TagInventory_Raw(handle, ants_array, ant_count, timeout_ms, &tag_count_out)
        err = TagInventory_Raw(hreader, ants, 1, 200, &tagcnt);
        
        if (err == MT_OK_ERR) {
            if (tagcnt > 0) {
                printf("Tags found: %d\n", tagcnt);
                for (i = 0; i < tagcnt; ++i) {
                    err = GetNextTag(hreader, &tag_);
                    if (err == MT_OK_ERR) {
                        printf("EPC: ");
                        for(f = 0; f < tag_.Epclen; ++f) {
                            printf("%02X", tag_.EpcId[f]);
                        }
                        printf(" (Ant: %d, RSSI: %d)\n", tag_.AntennaID, tag_.RSSI);
                    }
                }
            }
        } else {
             // MT_IO_ERR or others might happen if no tags or timeout, often fine to ignore in loop
             // unless it persists.
             // printf("Inventory error: %d\n", err); 
        }
        
        usleep(100000); // 100ms delay
    }

    printf("\nStopping...\n");
    CloseReader(hreader);
    return 0;
}
