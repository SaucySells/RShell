#!/bin/sh

cd ..

bin/rshell << 'EOF'
echo hello there #this won't be echoed; echo none of this will be run; git status;
ls -1; echo hello there # general kenobi; FakeCommand O_o
exit
EOF
