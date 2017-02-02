#!/usr/bin/python

from pprint import pprint
import graphs_utils as gu
from sys import argv

pprint(gu.getMetricsForFile(argv[1]))
 
