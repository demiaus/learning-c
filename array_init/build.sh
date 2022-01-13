#!/bin/sh

set -xe

gcc array_init.c -Wall -Wextra -Werror -std=c11 -ggdb -o array_init
# We are going to initialize one array using a forbidden range method,
# so this won't compile with -pedantic flag
