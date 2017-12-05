#!/bin/sh

[ -x "$1" ] || {
  echo "app not provided" 1>&2
  exit 1;
}

"$1" | grep "Hello, World!"
