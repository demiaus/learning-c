#!/bin/sh

# -x Print shell command before executing it.
# -e Exit if any command exits with non-zero without handling it.
set -xe

input=$1
filename="$(basename $1 .c )"
project="$(echo $1 | awk -F/ '{ print $(NF-1)}')"

echo "Input:    $file"
echo "Filename: $filename"
echo "Project:  $project"

mkdir -p $project/BUILD
# # Create a BUILD directory if needed
# if [ ! -d "$project/BUILD" ]; then
#         echo "No BUILD/ directory found in $project"
#         mkdir "$project/BUILD"
#         echo "New BUILD folder ./$project/BUILD made"
# fi

FLAGS="-Wall -Wextra -Werror -pedantic -std=c11 -ggdb"

for file in "$@"; do
        gcc $FLAGS -o $project/BUILD/$filename $file;
done;

# Run
./$project/BUILD/$filename
