#!/bin/sh

[ -x "$1" ] || {
  echo "app not provided" 1>&2
  exit 1;
}

if [ -z "$TRAVIS_BUILD_NUMBER" ]; then
  TRAVIS_BUILD_NUMBER=0;
fi

"$1" -v | grep "Version: 1.0.$TRAVIS_BUILD_NUMBER"
