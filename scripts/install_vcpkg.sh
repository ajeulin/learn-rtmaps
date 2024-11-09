#! /bin/bash
# **********************************************************
#      2024 : Arthur JEULIN
#      Copyright (c) Coast Autonomous 
#      Created by Arthur JEULIN on 07/02/2024.
# **********************************************************
#
# Module : Install VCPKG Folder for Linux User
#

# to run this script on windows
# open wsl terminal and go to cd /mnt/c/dev/rtmaps/inventory
# ./install_vcpkg.sh
#
# CAUTION: THE FILE HAS TO BE LF AND NOTE CRLF
#


git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh