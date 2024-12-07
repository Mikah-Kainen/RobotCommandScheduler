#!/bin/bash

./docker_build.sh
./deploy_code.sh
sshpass -p 'maker' ssh robot@ev3dev ./main_build