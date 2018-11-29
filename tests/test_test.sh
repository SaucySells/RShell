#!/bin/sh


cd ..


bin/rshell << 'EOF'
test -e src/main.cpp
test -f src/And.cpp
test -d src/Or.cpp
mkdir testdir
test -d testdir
test -f testdir
test -e src/main.cpp && echo A
test -e src/main.cpp || echo B
[ -e src/main.cpp ] && (echo A && echo B)
exit
EOF
