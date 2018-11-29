#!/bin/sh

cd ..

bin/rshell << 'EOF'
touch input.txt
echo abcdefg > input.txt
cat < input.txt
tr a-z A-Z < input.txt
exit
EOF
