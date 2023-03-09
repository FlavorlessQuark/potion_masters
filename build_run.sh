#!/bin/bash

base=$(pwd)
wscp="$base/../WSCPServ/"
c_cp="$base/../c_controlpads"
browser="firefox"

# kill processes from last run
grep_kill -f "target/.*/server"
grep_kill -f "node.*index.js"
grep_kill -f "console_bin"

if [[ $1 == "-x" ]]; then
    exit
fi


cd $base/console
if [[ $1 == "--re" ]]; then
    make re
else
    make
fi

cp $c_cp/target/debug/libc_controlpads.a client/
cd $base/client
make html

# run potion master
cd $base/console/
./console_bin &

# run controlpads server
cd $wscp
cargo run &

# run node server
cd $base/client
node node/index.js &

# run a controller
$browser 127.0.0.1:3000 &
$browser 127.0.0.1:3000?subid=1 &

