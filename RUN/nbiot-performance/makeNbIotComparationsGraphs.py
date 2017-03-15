#!/usr/bin/python
# coding: utf-8

from sys import argv, exit
from glob import glob
import os
import pylab as pl
from natsort import natsorted
from graphs_utils import *
from itertools import cycle

if __name__ == "__main__":

	clean = lambda x: float( filter( lambda x: x.isdigit() or x=='.', x ) )

	xlabel = "Numero de UEs"
	experimentDirectory = "Executions/default"

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

	markers = cycle(['*','o','v','s','d'])

	comparators = glob('*=*')
	plots = { c:dict() for c in comparators }
	cdfs = { c:dict() for c in comparators }
	X = set()

	for c in comparators:
		scenarios = natsorted(glob(c+'/*=*'))
		for scenario in scenarios:
			x = clean(scenario.split('/')[-1])
			X.add(x)

			statistics, cdf = getStatisticsForScenario (scenario)

			for k in statistics.keys():
				y, ci = statistics[k]
				if k not in plots[c]: plots[c][k] = {"y":list(), "ci":list()}
				plots[c][k]["y"].append(y)
				plots[c][k]["ci"].append(ci)
	X = natsorted(X)


	if not os.path.isdir("graphs"):	os.mkdir("graphs")
	os.chdir("graphs")

	figure = dict();
	rect = [0.1, 0.1, 0.8, 0.8]
	for c in comparators:
		#print "cdfs:", cdfs[sch].keys()
		#print "plots:", plots[sch].keys()
		#if "queueDelay" not in figure:
			#f = pl.figure()
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)''
			#ax.set_xlabel(xlabel)
			#ax.set_ylabel("Segundos")
			#figure["queueDelay"] = {'figure': f, 'axes': ax}
		#ax = figure["queueDelay"]['axes']
		#ax.errorbar(X, plots[sch]["queueDelay"]["y"], plots[sch]["queueDelay"]["ci"], label=sch_labels[sch], marker=markers.next())

		#if "droppedDelay" not in figure:
			#f = pl.figure()
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)
			#ax.set_xlabel(xlabel)
			#ax.set_ylabel("Millisegundos")
			#figure["droppedDelay"] = {'figure': f, 'axes': ax}
		#ax = figure["droppedDelay"]['axes']
		#ax.errorbar(X, plots[sch]["droppedDelay"]["y"], plots[sch]["droppedDelay"]["ci"], label=sch_labels[sch], marker=markers.next())


		if "rxDelay" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Millisegundos")
			figure["rxDelay"] = {'figure': f, 'axes': ax}
		ax = figure["rxDelay"]['axes']
		ax.errorbar(X, plots[c]["rxDelay"]["y"], plots[c]["rxDelay"]["ci"], label=c, marker=markers.next())

		#if "rxDelayCDF" not in figure:
			#f = pl.figure()
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)
			#ax.set_xlabel("Millisegundos")
			#figure["rxDelayCDF"] = {'figure': f, 'axes': ax}
		#ax = figure["rxDelayCDF"]['axes']
		#plotCDF(cdfs[c]["rxDelay"], c, target=ax, mark=markers.next())


		if "userThroughput" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["userThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["userThroughput"]['axes']
		ax.errorbar(X, plots[c]["userThroughput"]["y"], plots[c]["userThroughput"]["ci"], label=c, marker=markers.next())

		#if "userThroughputCDF" not in figure:
			#f = pl.figure()
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)
			#ax.set_xlabel("Kbit/s")
			#figure["userThroughputCDF"] = {'figure': f, 'axes': ax}
		#ax = figure["userThroughputCDF"]['axes']
		#plotCDF(cdfs[c]["userThroughput"], c, target=ax, mark=markers.next())

		if "RxThroughput" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["RxThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["RxThroughput"]['axes']
		ax.errorbar(X, plots[c]["RxThroughput"]["y"], plots[c]["RxThroughput"]["ci"], label=c, marker=markers.next())

		if "justiceRatio" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Jain Justice Ratio")
			figure["justiceRatio"] = {'figure': f, 'axes': ax}
		ax = figure["justiceRatio"]['axes']
		ax.errorbar(X, plots[c]["justiceRatio"]["y"], plots[c]["justiceRatio"]["ci"], label=c, marker=markers.next())

		if "TxThroughput" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["TxThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["TxThroughput"]['axes']
		ax.errorbar(X, plots[c]["TxThroughput"]["y"], plots[c]["TxThroughput"]["ci"], label=c, marker=markers.next())


		if "deliveryRate" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.set_ylim(-5,105)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("%")
			figure["deliveryRate"] = {'figure': f, 'axes': ax}
		ax = figure["deliveryRate"]['axes']
		ax.errorbar(X, plots[c]["deliveryRate"]["y"], plots[c]["deliveryRate"]["ci"], label=c, marker=markers.next())


		if "transmittedPkgs" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			#ax.set_title("Nb transmitted PKGs")
			figure["transmittedPkgs"] = {'figure': f, 'axes': ax}
		ax = figure["transmittedPkgs"]['axes']
		ax.errorbar(X, plots[c]["transmittedPkgs"]["y"], plots[c]["transmittedPkgs"]["ci"], label=c, marker=markers.next())

		#if "lostPkgs" not in figure:
			#f = pl.figure()
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)
			#ax.set_xlabel(xlabel)
			##ax.set_title("Nb lost packages")
			#figure["lostPkgs"] = {'figure': f, 'axes': ax}
		#ax = figure["lostPkgs"]['axes']
		#ax.errorbar(X, plots[c]["lostPkgs"]["y"], plots[c]["lostPkgs"]["ci"], label=c_labels[c], marker=markers.next())


		if "droppedPkgs" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			#ax.set_title("Nb dropped PKGs in L2")
			figure["droppedPkgs"] = {'figure': f, 'axes': ax}
		ax = figure["droppedPkgs"]['axes']
		ax.errorbar(X, plots[c]["droppedPkgs"]["y"], plots[c]["droppedPkgs"]["ci"], label=c, marker=markers.next())

		if "h2hDeliveryRate" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.set_ylim(-5,105)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("%")
			figure["h2hDeliveryRate"] = {'figure': f, 'axes': ax}
		ax = figure["h2hDeliveryRate"]['axes']
		ax.errorbar(X, plots[c]["h2hDeliveryRate"]["y"], plots[c]["h2hDeliveryRate"]["ci"], label=c, marker=markers.next())

		if "h2hThroughput" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["h2hThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["h2hThroughput"]['axes']
		ax.errorbar(X, plots[c]["h2hThroughput"]["y"], plots[c]["h2hThroughput"]["ci"], label=c, marker=markers.next())

		if "m2mDeliveryRate" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			#ax.set_ylim(-5,105)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("%")
			figure["m2mDeliveryRate"] = {'figure': f, 'axes': ax}
		ax = figure["m2mDeliveryRate"]['axes']
		ax.errorbar(X, plots[c]["m2mDeliveryRate"]["y"], plots[c]["m2mDeliveryRate"]["ci"], label=c, marker=markers.next())

		if "m2mThroughput" not in figure:
			f = pl.figure()
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["m2mThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["m2mThroughput"]['axes']
		ax.errorbar(X, plots[c]["m2mThroughput"]["y"], plots[c]["m2mThroughput"]["ci"], label=c, marker=markers.next())

		for app in ['VIDEO','CBR','VOIP','M2M_ED','M2M_TD']:
			app = 'throughput_'+app
			if plots[c][app]:
				if app not in figure:
					f = pl.figure()
					ax = f.add_axes(rect)
					ax.margins(0.05, 0.05)
					ax.set_xlabel(xlabel)
					ax.set_ylabel("Kbit/s")
					#ax.set_title("Nb dropped PKGs in L2")
					figure[app] = {'figure': f, 'axes': ax}
				ax = figure[app]['axes']
				ax.errorbar(X, plots[c][app]["y"], plots[c][app]["ci"], label=c, marker=markers.next())

		for app in ['VIDEO','CBR','VOIP','M2M_ED','M2M_TD']:
			app = 'justiceRatio_'+app
			if plots[c][app]:
				if app not in figure:
					f = pl.figure()
					ax = f.add_axes(rect)
					ax.margins(0.05, 0.05)
					ax.set_xlabel(xlabel)
					ax.set_ylabel("Jain Justice Ratio")
					#ax.set_title("Nb dropped PKGs in L2")
					figure[app] = {'figure': f, 'axes': ax}
				ax = figure[app]['axes']
				ax.errorbar(X, plots[c][app]["y"], plots[c][app]["ci"], label=c, marker=markers.next())


	del plots

	for m in figure.keys():
		figure[m]['axes'].legend(loc='best')
		figure[m]['figure'].savefig(m+'.png')


