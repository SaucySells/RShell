#!/bin/sh

cd ..

bin/rshell << 'EOF'
touch output.txt
echo what's going on here >> output.txt
ls -1 >> output.txt
echo hello there >> output.txt && echo goodbye >> output.txt
exit
EOF
