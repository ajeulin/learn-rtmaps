#!/bin/bash
# **********************************************************
#     2024 : Arthur JEULIN
#     Copyright (c) Coast Autonomous 
#     Created by Arthur JEULIN on 11/01/2024
# **********************************************************
#
# Module : Send the PCK archive to Deployment server
#

# Package name
PCK="learn_rtmaps"
VERSION="0.0.1"

# Define paths
KEY_PATH="/home/arthur/ssh/arthur.pem"
FILE_PATH="$(dirname "$0")/rtmaps_pck_${PCK}_${VERSION}_ubuntu2204_x86_64.zip"
DESTINATION="ubuntu@ec2-18-191-253-234.us-east-2.compute.amazonaws.com:/home/ubuntu/data/downloads/update_site_incoming/"

# Execute scp command
scp -i "$KEY_PATH" "$FILE_PATH" "$DESTINATION"

# Indicate success
echo "Success."

# Pause equivalent (wait for user input)
read -p "Press any key to continue..." -n1 -s