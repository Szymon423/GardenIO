#!/bin/bash
# docker run -v /mnt/e/Crazy-stuff/GardenIO:/home/szymon/GardenIO -w /home/szymon/GardenIO -i -t --rm asuracodes/work:cm4 /bin/bash
# docker run -v /mnt/e/Crazy-stuff/GardenIO:/home/szymon/GardenIO -w /home/szymon/GardenIO -i -t --rm test /bin/bash
docker run -v /mnt/e/Crazy-stuff/GardenIO:/home/szymon/GardenIO -w /home/szymon/GardenIO -i -t --rm armhf-compiler-image /bin/bash