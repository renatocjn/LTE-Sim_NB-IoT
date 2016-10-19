#!/usr/bin/python
# coding: utf-8

from sys import argv, exit
from glob import glob
import os
import pylab as pl
from natsort import natsorted
from graphs_utils import *

if __name__ == "__main__":

	clean = lambda x: float( filter( lambda x: x.isdigit() or x=='.', x ) )

	xlabel = "Number of UE"
	experimentDirectory = "Executions/default"
	superTitle = "Default Execution"

	if len(argv) > 3:
		superTitle = argv[3]
	if len(argv) > 2:
		xlabel = argv[2]
	if len(argv) > 1:
		experimentDirectory = argv[1]
	else:
		print
		print "\t\t*** BAD PARAMETERS ***   "
		print "Usage:", argv[0], "<experiment-direcory-path>", "[xlabel for graphs]"
		print "\t", "The xlabel is optional"
		print
		exit(1)

	os.chdir(experimentDirectory)
	schedulers = ['rr', 'mt', 'pf']

	X = set()
	plots = {'rr': dict(), 'pf': dict(), 'mt': dict()}

	for sch in schedulers:
		scenarios = natsorted(glob('scheduler='+sch+'/*=*'))
		for scenario in scenarios:
			x = clean(scenario)
			X.add(x)
			statistics, cdf = getStatisticsForScenario (scenario)

			#for k in cdf.keys():
				#if k not in cdfs: cdfs[k] = dict()
				#cdfs[k][scenario] = cdf[k]

			for k in statistics.keys():
				y, ci = statistics[k]
				if k not in plots[sch]: plots[sch][k] = {"y":list(), "ci":list()}
				plots[sch][k]["y"].append(y)
				plots[sch][k]["ci"].append(ci)
	X = natsorted(X)


	if not os.path.isdir("graphs"):	os.mkdir("graphs")
	os.chdir("graphs")

	figure = dict();
	rect = [0.1, 0.1, 0.8, 0.8]
	for sch in schedulers:

		if "queueDelay" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Delay for queued PKGs (s)")
			figure["queueDelay"] = {'figure': f, 'axes': ax}
		ax = figure["queueDelay"]['axes']
		ax.errorbar(X, plots[sch]["queueDelay"]["y"], plots[sch]["queueDelay"]["ci"], label="scheduler "+sch)


		if "droppedDelay" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Delay for dropped PKGs(ms)")
			figure["droppedDelay"] = {'figure': f, 'axes': ax}
		ax = figure["droppedDelay"]['axes']
		ax.errorbar(X, plots[sch]["droppedDelay"]["y"], plots[sch]["droppedDelay"]["ci"], label="scheduler "+sch)


		if "rxDelay" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Received delay (ms)")
			figure["rxDelay"] = {'figure': f, 'axes': ax}
		ax = figure["rxDelay"]['axes']
		ax.errorbar(X, plots[sch]["rxDelay"]["y"], plots[sch]["rxDelay"]["ci"], label="scheduler "+sch)


		if "userThroughput" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("User throughput(Kbit/s)")
			figure["userThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["userThroughput"]['axes']
		ax.errorbar(X, plots[sch]["userThroughput"]["y"], plots[sch]["userThroughput"]["ci"], label="scheduler "+sch)


		if "RxThroughput" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Aggregated received throughput(Kbit/s)")
			figure["RxThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["RxThroughput"]['axes']
		ax.errorbar(X, plots[sch]["RxThroughput"]["y"], plots[sch]["RxThroughput"]["ci"], label="scheduler "+sch)


		if "TxThroughput" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Aggregated transmitted throughput (Kbit/s)")
			figure["TxThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["TxThroughput"]['axes']
		ax.errorbar(X, plots[sch]["TxThroughput"]["y"], plots[sch]["TxThroughput"]["ci"], label="scheduler "+sch)


		if "deliveryRate" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Delivery rate(%)")
			figure["deliveryRate"] = {'figure': f, 'axes': ax}
		ax = figure["deliveryRate"]['axes']
		ax.errorbar(X, plots[sch]["deliveryRate"]["y"], plots[sch]["deliveryRate"]["ci"], label="scheduler "+sch)


		if "transmittedPkgs" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Nb transmitted PKGs")
			figure["transmittedPkgs"] = {'figure': f, 'axes': ax}
		ax = figure["transmittedPkgs"]['axes']
		ax.errorbar(X, plots[sch]["transmittedPkgs"]["y"], plots[sch]["transmittedPkgs"]["ci"], label="scheduler "+sch)


		if "lostPkgs" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Nb lost packages")
			figure["lostPkgs"] = {'figure': f, 'axes': ax}
		ax = figure["lostPkgs"]['axes']
		ax.errorbar(X, plots[sch]["lostPkgs"]["y"], plots[sch]["lostPkgs"]["ci"], label="scheduler "+sch)


		if "droppedPkgs" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_title("Nb dropped PKGs in L2")
			figure["droppedPkgs"] = {'figure': f, 'axes': ax}
		ax = figure["droppedPkgs"]['axes']
		ax.errorbar(X, plots[sch]["droppedPkgs"]["y"], plots[sch]["droppedPkgs"]["ci"], label="scheduler "+sch)


	del plots

	for m in figure.keys():
		figure[m]['axes'].legend(loc='best')
		figure[m]['figure'].savefig(m+'.png')


