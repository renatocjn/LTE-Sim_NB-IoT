#!/bin/bash
for f in */*/*/; do echo $f `tail -qn15 $f/*/traceLteSim.txt|grep Stop|wc -l`; done|egrep -v 30$
