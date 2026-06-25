#!/bin/bash

# Navigate to the demo directory
# This assumes the script is placed in /home/charan/Downloads/NEWRFID-LINUX-C/
DEMO_DIR="/home/charan/Downloads/2NEWRFID-LINUX-C (1)/NEWRFID-LINUX-C/demos/普通模式盘点"

echo "Navigating to demo directory: $DEMO_DIR"
cd "$DEMO_DIR" || { echo "Directory not found! Check path."; exit 1; }

echo "Cleaning previous build..."
make clean

echo "Building the project..."
make

# Check if the executable was created
if [ -f "./ReadTags" ]; then
    echo "----------------------------------------"
    echo "Build successful! Starting RFID Reader..."
    echo "----------------------------------------"
    
    # Set the library path to include the local ./lib directory and run
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
    ./ReadTags
else
    echo "Error: Build failed. 'ReadTags' executable was not found."
    exit 1
fi
