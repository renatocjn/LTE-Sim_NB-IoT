#!/usr/bin/python

from pprint import pprint
import graphs_utils as gu
from sys import argv, stdin

if len(argv) == 1: f = stdin
else: f = argv[1]

s, _ = gu.getStatisticsForScenario(f)

pprint(s)
 
