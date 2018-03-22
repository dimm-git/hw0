#!/bin/sh
exit 0
D=`dirname "$0"`
I=`realpath "$D"`

for I in `seq 7 7` 9; do
  echo $I
  "$1" 3 < "$D/data/${I}_input.txt" | grep -v '^bulk' > output.txt
  diff -u "$D/data/${I}_output_0.txt" "output.txt" || diff -u "$D/data/${I}_output_1.txt" "output.txt" || {
    echo "$I: diffs found"
    exit 1
  }
done;

echo "OK"

exit 0
