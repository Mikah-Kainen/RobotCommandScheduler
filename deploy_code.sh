#!/bin/bash

# For first time users when connecting from WSL, you might need to edit the /etc/hosts file to explicitly map the ev3's IP address to ev3dev
# This uses sshpass. Install with: sudo apt install sshpass

# ldd --version

sshpass -p 'maker' scp ./main_build robot@ev3dev:~
sshpass -p 'maker' ssh robot@ev3dev "chmod 777 ./main_build"
echo "Success!"