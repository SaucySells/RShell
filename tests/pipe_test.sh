#!/bin/sh

cd ..

bin/rshell << 'EOF'
touch output.txt
touch output2.txt
touch f.txt
touch g.txt
echo hello | tr a-z A-Z > output.txt
echo C | tr A-Z a-z > f.txt && cat f.txt | tr a-z A-Z >> g.txt
exit
EOF
