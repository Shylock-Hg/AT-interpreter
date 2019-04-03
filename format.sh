#! /usr/bin/env sh

clang-format -i --style=file \
$(git ls-files | grep '.*\.h$') \
$(git ls-files | grep '.*\.c$')

