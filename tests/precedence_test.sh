#!/bin/sh

cd ..

bin/rshell << 'EOF'
((echo A && echo B) && echo C)
(echo A && (echo B && echo C))
(echo A && echo B) || (echo C && echo B)
(echo A) && echo B
((echo A && echo B)
exit
EOF
