#!/bin/bash

time ./program &
programPid=$!
(for(( ; ; )); 
do 
uptime; sleep 5; 
done ) &
loopPid=$!
readarray -t pidTab <<< "$(jobs -p)"
wait $programPid
kill $loopPid