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

	xlabel = "Number of UEs"
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

	#markers = cycle(['*','o','v','s','d'])
	#markers = cycle(['*','o','v','d','o','v','d','*','v','d','*','o'])
	markers = cycle(['*','o','v','d','s','^','p','H','<','>','h','.'])

	comparators = glob('*=*/*=*')
	plots = { c:dict() for c in comparators }
	cdfs = { c:dict() for c in comparators }
	X = set()

	for c in natsorted(comparators):
		scenarios = natsorted(glob(c+'/*=*'))
		for scenario in natsorted(scenarios):
			x = clean(scenario.split('/')[-1])
			if x > 200: continue
			X.add(x)

			statistics, cdf = getStatisticsForScenario (scenario)

			for k in statistics.keys():
				y, ci = statistics[k]
				if k not in plots[c]: plots[c][k] = {"y":list(), "ci":list()}
				plots[c][k]["y"].append(y)
				plots[c][k]["ci"].append(ci)
	X = natsorted(X)


	if not os.path.isdir("../superGraphs"):	os.mkdir("../superGraphs")
	os.chdir("../superGraphs")

	figure = dict();
	figsize = 10, 5
	rect = [0.1, 0.15, 0.4, 0.75]
	
		#print "cdfs:", cdfs[sch].keys()
		#print "plots:", plots[sch].keys()
		#if "queueDelay" not in figure:
			#f = pl.figure(figsize=figsize)
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)''
			#ax.set_xlabel(xlabel)
			#ax.set_ylabel("Segundos")
			#figure["queueDelay"] = {'figure': f, 'axes': ax}
		#ax = figure["queueDelay"]['axes']
		#ax.errorbar(X, plots[sch]["queueDelay"]["y"], plots[sch]["queueDelay"]["ci"], label=sch_labels[sch], marker=markers.next())

		#if "droppedDelay" not in figure:
			#f = pl.figure(figsize=figsize)
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)
			#ax.set_xlabel(xlabel)
			#ax.set_ylabel("Millisegundos")
			#figure["droppedDelay"] = {'figure': f, 'axes': ax}
		#ax = figure["droppedDelay"]['axes']
		#ax.errorbar(X, plots[sch]["droppedDelay"]["y"], plots[sch]["droppedDelay"]["ci"], label=sch_labels[sch], marker=markers.next())

	#for c in natsorted(comparators):
	#	if "rxDelay" not in figure:
	#		f = pl.figure(figsize=figsize)
	#		ax = f.add_axes(rect)
	#		ax.margins(0.05, 0.05)
	#		ax.set_xlabel(xlabel)
	#		ax.set_ylabel("Millisegundos")
	#		figure["rxDelay"] = {'figure': f, 'axes': ax}
	#	ax = figure["rxDelay"]['axes']
	#	ax.errorbar(X, plots[c]["rxDelay"]["y"], plots[c]["rxDelay"]["ci"], label=c, marker=markers.next())

		#if "rxDelayCDF" not in figure:
			#f = pl.figure(figsize=figsize)
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)
			#ax.set_xlabel("Millisegundos")
			#figure["rxDelayCDF"] = {'figure': f, 'axes': ax}
		#ax = figure["rxDelayCDF"]['axes']
		#plotCDF(cdfs[c]["rxDelay"], c, target=ax, mark=markers.next())

	for c in natsorted(comparators):
		if "userThroughput" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["userThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["userThroughput"]['axes']
		ax.errorbar(X, plots[c]["userThroughput"]["y"], plots[c]["userThroughput"]["ci"], label=c, marker=markers.next())

		#if "userThroughputCDF" not in figure:
			#f = pl.figure(figsize=figsize)
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)
			#ax.set_xlabel("Kbit/s")
			#figure["userThroughputCDF"] = {'figure': f, 'axes': ax}
		#ax = figure["userThroughputCDF"]['axes']
		#plotCDF(cdfs[c]["userThroughput"], c, target=ax, mark=markers.next())

	for c in natsorted(comparators):
		if "RxThroughput" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["RxThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["RxThroughput"]['axes']
		ax.errorbar(X, plots[c]["RxThroughput"]["y"], plots[c]["RxThroughput"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "justiceRatio" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Jain Justice Ratio")
			figure["justiceRatio"] = {'figure': f, 'axes': ax}
		ax = figure["justiceRatio"]['axes']
		ax.errorbar(X, plots[c]["justiceRatio"]["y"], plots[c]["justiceRatio"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "TxThroughput" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["TxThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["TxThroughput"]['axes']
		ax.errorbar(X, plots[c]["TxThroughput"]["y"], plots[c]["TxThroughput"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "deliveryRatio" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.set_ylim(-5,105)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("%")
			figure["deliveryRatio"] = {'figure': f, 'axes': ax}
		ax = figure["deliveryRatio"]['axes']
		ax.errorbar(X, plots[c]["deliveryRatio"]["y"], plots[c]["deliveryRatio"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "transmittedPkgs" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			#ax.set_title("Nb transmitted PKGs")
			figure["transmittedPkgs"] = {'figure': f, 'axes': ax}
		ax = figure["transmittedPkgs"]['axes']
		ax.errorbar(X, plots[c]["transmittedPkgs"]["y"], plots[c]["transmittedPkgs"]["ci"], label=c, marker=markers.next())

		#if "lostPkgs" not in figure:
			#f = pl.figure(figsize=figsize)
			#ax = f.add_axes(rect)
			#ax.margins(0.05, 0.05)
			#ax.set_xlabel(xlabel)
			##ax.set_title("Nb lost packages")
			#figure["lostPkgs"] = {'figure': f, 'axes': ax}
		#ax = figure["lostPkgs"]['axes']
		#ax.errorbar(X, plots[c]["lostPkgs"]["y"], plots[c]["lostPkgs"]["ci"], label=c_labels[c], marker=markers.next())

	#for c in natsorted(comparators):
	#	if "droppedPkgs" not in figure:
	#		f = pl.figure(figsize=figsize)
	#		ax = f.add_axes(rect)
	#		ax.margins(0.05, 0.05)
	#		ax.set_xlabel(xlabel)
	#		#ax.set_title("Nb dropped PKGs in L2")
	#		figure["droppedPkgs"] = {'figure': f, 'axes': ax}
	#	ax = figure["droppedPkgs"]['axes']
	#	ax.errorbar(X, plots[c]["droppedPkgs"]["y"], plots[c]["droppedPkgs"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "h2hDeliveryRatio" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.set_ylim(-5,105)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("%")
			figure["h2hDeliveryRatio"] = {'figure': f, 'axes': ax}
		ax = figure["h2hDeliveryRatio"]['axes']
		ax.errorbar(X, plots[c]["h2hDeliveryRatio"]["y"], plots[c]["h2hDeliveryRatio"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "h2hThroughput" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["h2hThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["h2hThroughput"]['axes']
		ax.errorbar(X, plots[c]["h2hThroughput"]["y"], plots[c]["h2hThroughput"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "m2mDeliveryRatio" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			#ax.set_ylim(-5,105)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("%")
			figure["m2mDeliveryRatio"] = {'figure': f, 'axes': ax}
		ax = figure["m2mDeliveryRatio"]['axes']
		ax.errorbar(X, plots[c]["m2mDeliveryRatio"]["y"], plots[c]["m2mDeliveryRatio"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "m2mThroughput" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("Kbit/s")
			figure["m2mThroughput"] = {'figure': f, 'axes': ax}
		ax = figure["m2mThroughput"]['axes']
		ax.errorbar(X, plots[c]["m2mThroughput"]["y"], plots[c]["m2mThroughput"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "h2hDropped" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			figure["h2hDropped"] = {'figure': f, 'axes': ax}
		ax = figure["h2hDropped"]['axes']
		ax.errorbar(X, plots[c]["h2hDropped"]["y"], plots[c]["h2hDropped"]["ci"], label=c, marker=markers.next())
		
	for c in natsorted(comparators):
		if "m2mDropped" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			figure["m2mDropped"] = {'figure': f, 'axes': ax}
		ax = figure["m2mDropped"]['axes']
		ax.errorbar(X, plots[c]["m2mDropped"]["y"], plots[c]["m2mDropped"]["ci"], label=c, marker=markers.next())
		
	for c in natsorted(comparators):
		if "m2mFragSum" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("KBytes")
			figure["m2mFragSum"] = {'figure': f, 'axes': ax}
		ax = figure["m2mFragSum"]['axes']
		ax.errorbar(X, plots[c]["m2mFragSum"]["y"], plots[c]["m2mFragSum"]["ci"], label=c, marker=markers.next())

	for c in natsorted(comparators):
		if "h2hFragSum" not in figure:
			f = pl.figure(figsize=figsize)
			ax = f.add_axes(rect)
			ax.margins(0.05, 0.05)
			ax.set_xlabel(xlabel)
			ax.set_ylabel("KBytes")			
			figure["h2hFragSum"] = {'figure': f, 'axes': ax}
		ax = figure["h2hFragSum"]['axes']
		ax.errorbar(X, plots[c]["h2hFragSum"]["y"], plots[c]["h2hFragSum"]["ci"], label=c, marker=markers.next())

	for app in ['VIDEO','CBR','VOIP','M2M_ED','M2M_TD']:
		app = 'throughput_'+app
		if app in plots[c]:
			for c in natsorted(comparators):
				if app not in figure:
					f = pl.figure(figsize=figsize)
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
		if app in plots[c]:
			for c in natsorted(comparators):
				if app not in figure:
					f = pl.figure(figsize=figsize)
					ax = f.add_axes(rect)
					ax.margins(0.05, 0.05)
					ax.set_xlabel(xlabel)
					ax.set_ylabel("Jain Justice Ratio")
					#ax.set_title("Nb dropped PKGs in L2")
					figure[app] = {'figure': f, 'axes': ax}
				ax = figure[app]['axes']
				ax.errorbar(X, plots[c][app]["y"], plots[c][app]["ci"], label=c, marker=markers.next())

	for app in ['VIDEO','CBR','VOIP','M2M_ED','M2M_TD']:
		app = 'deliveryRatio_'+app
		if app in plots[c]:
			for c in natsorted(comparators):
				if app not in figure:
					f = pl.figure(figsize=figsize)
					ax = f.add_axes(rect)
					ax.margins(0.05, 0.05)
					ax.set_xlabel(xlabel)
					ax.set_ylabel("%")
					#ax.set_title("Nb dropped PKGs in L2")
					figure[app] = {'figure': f, 'axes': ax}
				ax = figure[app]['axes']
				ax.errorbar(X, plots[c][app]["y"], plots[c][app]["ci"], label=c, marker=markers.next())

	del plots

	for m in figure.keys():
		figure[m]['axes'].legend(loc='center left', bbox_to_anchor=(1, 0.5))
		#figure[m]['axes'].legend(loc='best')
		figure[m]['figure'].savefig(m+'.png')


