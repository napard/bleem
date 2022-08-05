#!/bin/bash

echo 's" forth/core.fs" included' > tmp.fs
node repl.js < $1 >> tmp.fs && gforth tmp.fs
