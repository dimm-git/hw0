#!/bin/sh

D=`dirname "$0"`
I=`realpath "$D"`

"$1" 3 < "$D/data/${2}_input.txt" > output.txt

diff -u "$D/data/${2}_output.txt" "output.txt" || {
  echo diffs found
  exit 1
}

echo "OK"
exit 0
