#!/bin/bash

D=`dirname "$0"`
I=`realpath "$D"`
B=`basename "$1"`

for I in `seq 0 2`; do
  killall "$B"
  sleep 5
  "$1" 9000 &
  sleep 3
  cat "$D/data/input.txt" | nc -q 5 127.0.0.1 9000 1>output.txt 2>&1 || {
    echo "$I: fill failed" 1>&2
    exit 1
  }
  diff "$D/data/fill.txt" output.txt 1>/dev/null 2>&1 || {
    echo "$I: fill diff failed" 1>&2
    exit 1
  }
  cat "$D/data/${I}_cmd.txt" | nc -q 5 127.0.0.1 9000 1>output.txt 2>&1 || {
    echo "$I: cmd exec failed" 1>&2
    exit 1
  }
  diff "$D/data/${I}_output.txt" output.txt 1>/dev/null 2>&1 || {
    echo "$I: cmd logic failed" 1>&2
    exit 1
  }
done;

echo "OK"

exit 0
