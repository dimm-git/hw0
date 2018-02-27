#!/bin/sh

D=`dirname "$0"`
I=`realpath "$D"`

for I in `seq 0 6`; do
  echo $I
  "$1" 3 < "$D/data/${I}_input.txt" | grep '^bulk' > output.txt
  diff -u "$D/data/${I}_output.txt" "output.txt" || {
    echo "$I: diffs found"
    exit 1
  }
done;

echo "OK"

exit 0
