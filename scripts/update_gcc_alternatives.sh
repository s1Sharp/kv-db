#!/bin/sh
if [ -z "$1" ]; then
    echo "usage: $0 version" 1>&2
    exit 1
fi
if [ ! -f "/usr/bin/gcc-$1" ] || [ ! -f "/usr/bin/g++-$1" ]; then
    echo "no such version gcc/g++ installed" 1>&2
    exit 1
fi
update-alternatives --install /usr/bin/gcc gcc "/usr/bin/gcc-$1" "$1"
update-alternatives --install /usr/bin/g++ g++ "/usr/bin/g++-$1" "$1"
