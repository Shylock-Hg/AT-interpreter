#! /usr/bin/env sh

clang-format -i --style=file \
$(git ls-files | grep '.*\.h$\|.*\.c$')

