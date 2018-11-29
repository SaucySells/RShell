#!/bin/sh

cd ..


bin/rshell << 'EOF'
echo test1
exit
EOF
echo 

bin/rshell << 'EOF2'
echo test2; exit; git status
EOF2




