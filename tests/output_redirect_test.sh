#!/bin/sh

cd ..

bin/rshell << 'EOF'
touch output1.txt
touch output2.txt
touch output3.txt
echo hello > output1.txt
ls -1 > output2.txt
cat output1.txt > output3.txt || echo whats the deal with airline food
exit
EOF
