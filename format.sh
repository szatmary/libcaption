#!/bin/bash
cd "$(dirname "$0")"
find . \( -name '*.c' -o -name '*.h' \) -exec clang-format -i -style=WebKit {} \;
