#!/bin/sh

# -x Print shell command before executing it.
# -e Exit if any command exits with non-zero without handling it.
set -xe

# Create a BUILD directory if needed
if [ ! -d "./BUILD" ]; then
        mkdir ./BUILD
fi


FLAGS="-Wall -Wextra -Werror -pedantic -std=c11 -ggdb"

# Build by giving source code file(s) as argument to build.sh = `./build.sh main.c module.c ...`
for name in "$@"; do
        gcc $FLAGS -o ./BUILD/$(basename "$name") $name;
done;
