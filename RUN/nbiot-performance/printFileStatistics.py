#!/usr/bin/python

from pprint import pprint
import graphs_utils as gu
from sys import argv

s = gu.getMetricsForFile(argv[1])
#s['rxDelay'] = gu.CalculateStatistics(s['rxDelay'])
s['userThroughput'] = gu.CalculateStatistics(s['userThroughput'])

pprint(s)
 
