#!bin/bash
gcc -o hello_world hello_world.c
gcc -o hello_client hello_client.c
nohup ./hello_world 8888 > nohup.log 2>&1 &
./hello_client 127.0.0.1 8888