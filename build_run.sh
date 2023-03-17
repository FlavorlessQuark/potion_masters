#!/bin/bash

set -e

base=$(pwd)
wscp="$base/../../LC/WSCPServ/"
c_cp="$base/../../LC/c_controlpads"
browser="google-chrome"

# kill processes from last run
grep_kill -f "target/.*/server"
grep_kill -f "node.*index.js"
grep_kill -f "console_bin"

if [[ $1 == "-x" ]]; then
    exit
fi


cp $c_cp/target/debug/libc_controlpads.a client/
cp $c_cp/target/debug/libc_controlpads.a console/

cd $base/console
if [[ $1 == "--re" ]]; then
    make re
else
    make
fi

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

sleep 1

# run a controller
$browser 127.0.0.1:3000 &
$browser 127.0.0.1:3000?subid=1 &

