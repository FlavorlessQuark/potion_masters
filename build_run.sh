#!/bin/bash

base=$(pwd)
wscp="$base/../../LC/WSCPServ/"
c_cp="$base/../../LC/c_controlpads"


# kill processes from last run
grep_kill -f "target/.*/server"
grep_kill -f "node.*index.js"
grep_kill -f "console_bin"

if [[ $1 == "-x" ]]; then
    exit
fi


cd $base/console
make

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
google-chrome 127.0.0.1:3000 &
google-chrome 127.0.0.1:3000?subid=1 &

