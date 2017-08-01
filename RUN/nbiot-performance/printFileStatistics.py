#!/usr/bin/python

from pprint import pprint
import graphs_utils as gu
from sys import argv, stdin

if len(argv) == 1: f = stdin
else: f = argv[1]

s = gu.getMetricsForFile(f)
#s['rxDelay'] = gu.CalculateStatistics(s['rxDelay'])

if 'userThroughput' in s: 
	s['userThroughput'] = gu.CalculateStatistics(s['userThroughput'])
if 'throughput_M2M_ED' in s: 
	s['throughput_M2M_ED'] = gu.CalculateStatistics(s['throughput_M2M_ED'])
if 'throughput_M2M_TD' in s:
	s['throughput_M2M_TD'] = gu.CalculateStatistics(s['throughput_M2M_TD'])

pprint(s)
 
