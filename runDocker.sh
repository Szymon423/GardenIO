#!/bin/bash

while getopts ":w" option; do
    case $option in
    w)
        echo "Running Docker from wsl."
        docker run -p 502:502 -v /mnt/c/Projects/GardenIO:/home/szymon/GardenIO -w /home/szymon/GardenIO -i -t --rm armhf-compiler-image /bin/bash
        exit
        ;;
    *)
        ;;
    esac
done

echo "Running Docker from Linux."
docker run -v /home/szymon/projects/GardenIO/:/home/szymon/projects/GardenIO/ -w /home/szymon/projects/GardenIO/ -i -t --rm armhf-compiler-image /bin/bash
 
