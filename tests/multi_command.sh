#!/bin/sh

cd .. 

bin/rshell << 'EOF'
echo command1; git status && echo this will output || echo but this won't || echo but this will; ls -1 && touch HeyLookAtThis
echo A && echo B || echo C && echo D && FakeCommand Q || echo E
exit
EOF

