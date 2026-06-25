#!/bin/bash
# Script to run the RFID reader
# It sets the library path so the system can find 'libModuleAPI.so'

# Get the directory where this script is located
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Set the library path to include the x64 folder
export LD_LIBRARY_PATH=$DIR/libs-glibc2.19/x64:$LD_LIBRARY_PATH

# Run the reader program
echo "Starting RFID Reader..."
echo "Press Ctrl+C to stop."
echo "--------------------------------"
$DIR/read_tags /dev/ttyACM0
