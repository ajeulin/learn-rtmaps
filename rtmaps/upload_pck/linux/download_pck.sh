#!/bin/bash
# **********************************************************
#     2024 : Arthur JEULIN
#     Copyright (c) Coast Autonomous 
#     Created by Arthur JEULIN on 11/11/2024
# **********************************************************
#
# Module : Download a specific version of a PCK
#

# Path to RTMAPS_SDKDIR
RTMAPS_SDKDIR="/opt/rtmaps"

# Package name
PCK="learn_rtmaps"
VERSION="0.0.1"

# Generate the archive from the build script
"$RTMAPS_SDKDIR/bin/rtmaps_updater" get "pck_${PCK}" -minversion "$VERSION" -maxversion "$VERSION" -install

# Indicate success
echo "Success."

# Pause equivalent (wait for user input)
read -p "Press any key to continue..." -n1 -s