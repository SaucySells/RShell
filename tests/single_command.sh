#!/bin/sh

cd ..

bin/rshell << 'EOF'
echo all around me are familiar faces
ls -1
exit
EOF
