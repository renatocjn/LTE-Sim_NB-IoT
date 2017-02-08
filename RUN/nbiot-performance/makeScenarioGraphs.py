#!/usr/bin/python
# coding: utf-8

from sys import argv, exit
from glob import glob
import os
import pylab as pl
from natsort import natsorted
from graphs_utils import *
from pprint import pprint as pp

if __name__ == "__main__":

	clean = lambda x: float( filter( lambda x: x.isdigit() or x=='.', x ) )

	xlabel = "Number of UEs"
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
		print "Usage:", argv[0], "<experiment-direcory-path>", "[xlabel for graphs]", "[Super title]"
		print "\t", "The xlabel and super title is optional"
		print
		exit(1)

	os.chdir(experimentDirectory)
	scenarios = natsorted(glob('*=*'))

	X = list()
	plots = dict()
	cdfs = dict()

	for scenario in scenarios:
		x = clean(scenario)
		X.append(x)
		statistics, cdf = getStatisticsForScenario (scenario)

		for k in cdf.keys():
			if k not in cdfs: cdfs[k] = dict()
			cdfs[k][scenario] = cdf[k]

		for k in statistics.keys():
			y, ci = statistics[k]
			if k not in plots: plots[k] = {"y":list(), "ci":list()}
			plots[k]["y"].append(y)
			plots[k]["ci"].append(ci)


	#pp(plots)
	#pp(cdfs)

#	pl.clf()

	#pl.figure(figsize=(30, 15))
	#pl.suptitle(superTitle, fontsize="x-large")
	#i=0
	#j, k = 4, 4

	if not os.path.isdir("graphs"):	os.mkdir("graphs")
	os.chdir("graphs")

	#i+=1
	#pl.subplot(j,k,i)
	#pl.clf()
	#pl.margins(0.05, 0.05)
	#pl.xlabel(xlabel)
	#pl.ylabel("Segundos")
	#pl.errorbar(X, plots["queueDelay"]["y"], plots["queueDelay"]["ci"])
	#pl.savefig("queueDelay.png")

	#i+=1
	#pl.subplot(j,k,i)
	#pl.clf()
	#pl.margins(0.05, 0.05)
	#pl.xlabel("Segundos")
	#for scenario in natsorted(cdfs["queueDelay"].keys()):
	#	plotCDF(cdfs["queueDelay"][scenario], scenario)
	#pl.legend(loc="best")
	#pl.savefig("queueDelayCDF.png")


	#i+=1
	#pl.subplot(j,k,i)
	#pl.clf()
	#pl.margins(0.05, 0.05)
	#pl.xlabel(xlabel)
	#pl.ylabel("Millisegundos")
	#pl.errorbar(X, plots["droppedDelay"]["y"], plots["droppedDelay"]["ci"])
	#pl.savefig("droppedDelay.png")

	#i+=1
	#pl.subplot(j,k,i)
	#pl.clf()
	#pl.margins(0.05, 0.05)
	#pl.xlabel("Millisegundos")
	#for scenario in natsorted(cdfs["droppedDelay"].keys()):
	#	plotCDF(cdfs["droppedDelay"][scenario], scenario)
	#pl.legend(loc="best")
	#pl.savefig("droppedDelayCDF.png")


	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.ylabel("Millisegundos")
	pl.errorbar(X, plots["rxDelay"]["y"], plots["rxDelay"]["ci"])
	pl.savefig("rxDelay.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel("Millisegundos")
	for scenario in natsorted(cdfs["rxDelay"].keys()):
		plotCDF(cdfs["rxDelay"][scenario], scenario)
	pl.legend(loc="best")
	pl.savefig("rxDelayCDF.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.ylabel("Kbit/s")
	pl.errorbar(X, plots["userThroughput"]["y"], plots["userThroughput"]["ci"])
	pl.savefig("userThroughput.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel("Kbit/s")
	for scenario in natsorted(cdfs["userThroughput"].keys()):
		plotCDF(cdfs["userThroughput"][scenario], scenario)
	pl.legend(loc="best")
	pl.savefig("userThroughputCDF.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.ylabel("Kbit/s")
	pl.errorbar(X, plots["RxThroughput"]["y"], plots["RxThroughput"]["ci"])
	pl.savefig("RxThroughput.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.ylabel("Kbit/s")
	pl.errorbar(X, plots["TxThroughput"]["y"], plots["TxThroughput"]["ci"])
	pl.savefig("TxThroughput.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.ylabel("%")
	pl.errorbar(X, plots["deliveryRate"]["y"], plots["deliveryRate"]["ci"])
	pl.savefig("deliveryRate.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	#pl.ylabel("%")
	pl.errorbar(X, plots["justiceRatio"]["y"], plots["justiceRatio"]["ci"])
	pl.savefig("justiceRatio.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	#pl.title("Nb transmitted PKGs")
	pl.errorbar(X, plots["transmittedPkgs"]["y"], plots["transmittedPkgs"]["ci"])
	pl.savefig("transmittedPkgs.png")

	#i+=1
	#pl.subplot(j,k,i)
	#pl.clf()
	#pl.margins(0.05, 0.05)
	#pl.xlabel(xlabel)
	##pl.title("Nb lost packages")
	#pl.errorbar(X, plots["lostPkgs"]["y"], plots["lostPkgs"]["ci"])
	#pl.savefig("lostPkgs.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	#pl.title("Nb dropped PKGs in L2")
	pl.errorbar(X, plots["droppedPkgs"]["y"], plots["droppedPkgs"]["ci"])
	pl.savefig("droppedPkgs.png")

	#i+=1
	#pl.subplot(j,k,i)
	#pl.clf()
	#pl.margins(0.05, 0.05)
	#pl.xlabel("SINR")
	#for scenario in natsorted(cdfs["SINRs"].keys()):
		#plotCDF(cdfs["SINRs"][scenario], scenario)
	#pl.legend(loc="best")
	#pl.savefig("SINRs_CDF.png")

	#pl.subplots_adjust(.05, .1, .975, .9, .2, .45)

	for app in ['VIDEO','CBR','VOIP','M2M_ED','M2M_TD']:
		app = 'throughput_'+app
		if plots[app]:
			pl.clf()
			pl.margins(0.05, 0.05)
			pl.xlabel(xlabel)
			pl.ylabel("Kbit/s")
			pl.errorbar(X, plots[app]["y"], plots[app]["ci"])
			pl.savefig(app+".png")

	for app in ['VIDEO','CBR','VOIP','M2M_ED','M2M_TD']:
		app = 'throughput_'+app
		if plots[app]:
			pl.clf()
			pl.margins(0.05, 0.05)
			pl.xlabel(xlabel)
			pl.ylabel("Kbit/s")
			pl.errorbar(X, plots[app]["y"], plots[app]["ci"])
			pl.savefig(app+".png")

	del cdfs, plots

	#pl.savefig("graphs.png")
	#pl.show()




