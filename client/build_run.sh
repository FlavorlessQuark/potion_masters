#!/bin/bash

set -e

gcc target.c libc_controlpads.a -o target
./target
