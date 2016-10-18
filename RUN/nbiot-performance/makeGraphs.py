#!/usr/bin/python
# coding: utf-8

#TODO utilizar parametro markevery nos CDFs e diminuir amostragem do CDF de SINRs

from sys import argv, exit
from numpy import mean, array, sqrt, cumsum, sort, arange
from pprint import PrettyPrinter as pp
from glob import glob
import os
import pylab as pl
import itertools as it
from natsort import natsorted

def readDataFromFile(filePath):
	inputfile = open(filePath)
	lines = inputfile.readlines()
	inputfile.close()

	pkgLines = filter (lambda x: x.startswith("RX") or x.startswith("TX") or x.startswith("DROP_QUEUE") or x.startswith("UL_SINR"), lines)

	pkgs = dict()
	sinr = list()
	for l in pkgLines:
		l = l.split()

		if l[0] == "TX":
			stats = dict()
			stats["txSize"] = int(l[7])
			stats["src"] = int(l[9])
			stats["dst"] = int(l[11])
			stats["txTime"] = float(l[13])
			stats["rxSize"] = None
			stats["delay"] = None
			stats["dropped"] = False
			stats["user"] = int(l[5])
			pkgs[l[3]] = stats

		if l[0] == "RX":
			pkgs[l[3]]["rxSize"] = int(l[7])
			pkgs[l[3]]["delay"] = float(l[13])

		if l[0] == "DROP_QUEUE":
			pkgs[l[3]]["dropped"] = True
			pkgs[l[3]]["droppedTime"] = float(l[7])

		if l[0] == "UL_SINR":
			sinr.append(float(l[4]))

	del lines
	del pkgLines
	return pkgs.values(), sinr


def getMetricsForFile(filePath):
	pkgs, sinr = readDataFromFile(filePath)

	firstTx = 999999
	lastRx = 0

	lostPkgs = 0
	transmittedData = 0
	dataReceived = 0

	rxDelayList = list()
	queueDelayList = list()
	droppedDelayList = list()
	users = dict()


	for p in pkgs:
		transmittedData += p["txSize"]
		if p["txTime"] < firstTx:
				firstTx = p["txTime"]
		if p["delay"] == None:
			lostPkgs += 1
			if p["dropped"] == True:
				droppedDelayList.append((p["droppedTime"] - p["txTime"]) * 1000)
			else:
				queueDelayList.append(5.0-p["txTime"])
		else:
			if p["txTime"] + p["delay"] > lastRx:
				lastRx = p["txTime"] + p["delay"]

			rxDelayList.append(p["delay"] * 1000.0)
			dataReceived += p["rxSize"]
			if p["user"] not in users: users[p["user"]] = 0
			users[p["user"]] += 8*p["rxSize"]/1000

	transmittedPkgs = len(pkgs)
	droppedPkgs = len(droppedDelayList)
	d = lastRx - firstTx
	users_throughput = [ rxData/d for rxData in users.values() ]

	stats = {#"rxDelayMean": mean(rxDelayList),
			"rxDelay": rxDelayList,
			#"queueDelayMean": mean(queueDelayList),
			"queueDelay": queueDelayList,
			#"droppedDelayMean": mean(droppedDelayList),
			"droppedDelay": droppedDelayList,
			#"userThroughputMean": mean(users_throughput),
			"userThroughput": users_throughput,
			"RxThroughput": 8.0*dataReceived/d,
			"TxThroughput": 8.0*transmittedData/d,
			"deliveryRate": 100.0*(transmittedPkgs-lostPkgs)/transmittedPkgs,
			"transmittedPkgs": transmittedPkgs,
			"lostPkgs": lostPkgs,
			"droppedPkgs": droppedPkgs,
			"SINRs": sinr}

	del pkgs
	del users
	return stats


def CalculateStatistics(lvals):
	vals = array(lvals)
	if vals.size == 0 : return 0, 0
	mean = vals.mean()
	#if lvals.count(vals[0]) == len(vals): return mean, 0
	confidenceInterval = 1.96 * vals.std() / sqrt(vals.size)
	return mean, confidenceInterval

def getStatisticsForScenario(scenarioDirPath):
	metrics = None
	cdfs = set()
	metrics_cdfs = dict()

	runs = glob( os.path.join(scenarioDirPath,"*") )
	for r in runs:
		traceFile = os.path.join(r, "traceLteSim.txt")
		runMetrics = getMetricsForFile(traceFile)
		if metrics == None:
			metrics = {key:list() for key in runMetrics.keys()}

		for m in runMetrics.keys():
			if type(runMetrics[m]) == list:
				metrics[m] += runMetrics[m]
				cdfs.add(m)
			else:
				metrics[m].append (runMetrics[m])

	for m in cdfs:
		metrics_cdfs[m] = metrics[m]
	for m in metrics.keys():
		metrics[m] = CalculateStatistics (metrics[m])
	return metrics, metrics_cdfs

def plotCDF(data, plabel):
	sortedData = sort(data)
	s = len(data)
	p = 1. * arange(s) / (s - 1)
	pl.plot(sortedData, p, label=plabel, markevery=int(.1*s))

#######################################################################################################$
#######################################################################################################$
#######################################################################################################$


if __name__ == "__main__":

	clean = lambda x: float( filter( lambda x: x.isdigit() or x=='.', x ) )
	cmpf = lambda x,y: int(clean(x) - clean(y))

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
		print "Usage:", argv[0], "<experiment-direcory-path>", "[xlabel for graphs]", "[Super title]"
		print "\t", "The xlabel and super title is optional"
		print
		exit(1)

	os.chdir(experimentDirectory)
	scenarios = sorted(glob('*=*'), cmp=cmpf)

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




#	pl.clf()

	#pl.figure(figsize=(30, 15))
	#pl.suptitle(superTitle, fontsize="x-large")
	#i=0
	#j, k = 4, 4

	if not os.path.isdir("graphs"):	os.mkdir("graphs")
	os.chdir("graphs")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Delay for queued PKGs (s)")
	pl.errorbar(X, plots["queueDelay"]["y"], plots["queueDelay"]["ci"])
	pl.savefig("queueDelay.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	#pl.xlabel(xlabel)
	pl.title("CDF total delay for queued PKGs (s)")
	for scenario in natsorted(cdfs["queueDelay"].keys()):
		plotCDF(cdfs["queueDelay"][scenario], scenario)
	pl.legend(loc="best")
	pl.savefig("queueDelayCDF.png")


	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Delay for dropped PKGs(ms)")
	pl.errorbar(X, plots["droppedDelay"]["y"], plots["droppedDelay"]["ci"])
	pl.savefig("droppedDelay.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.title("CDF total delay for dropped PKGs (ms)")
	for scenario in natsorted(cdfs["droppedDelay"].keys()):
		plotCDF(cdfs["droppedDelay"][scenario], scenario)
	pl.legend(loc="best")
	pl.savefig("droppedDelayCDF.png")


	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Received delay (ms)")
	pl.errorbar(X, plots["rxDelay"]["y"], plots["rxDelay"]["ci"])
	pl.savefig("rxDelay.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.title("CDF total delay for received PKGs (ms)")
	for scenario in natsorted(cdfs["rxDelay"].keys()):
		plotCDF(cdfs["rxDelay"][scenario], scenario)
	pl.legend(loc="best")
	pl.savefig("rxDelayCDF.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("User throughput(Kbit/s)")
	pl.errorbar(X, plots["userThroughput"]["y"], plots["userThroughput"]["ci"])
	pl.savefig("userThroughput.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.title("CDF of user throughput(Kbit/s)")
	for scenario in natsorted(cdfs["userThroughput"].keys()):
		plotCDF(cdfs["userThroughput"][scenario], scenario)
	pl.legend(loc="best")
	pl.savefig("userThroughputCDF.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Aggregated received throughput(Kbit/s)")
	pl.errorbar(X, plots["RxThroughput"]["y"], plots["RxThroughput"]["ci"])
	pl.savefig("RxThroughput.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Aggregated transmitted throughput (Kbit/s)")
	pl.errorbar(X, plots["TxThroughput"]["y"], plots["TxThroughput"]["ci"])
	pl.savefig("TxThroughput.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Delivery rate(%)")
	pl.errorbar(X, plots["deliveryRate"]["y"], plots["deliveryRate"]["ci"])
	pl.savefig("deliveryRate.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Nb transmitted PKGs")
	pl.errorbar(X, plots["transmittedPkgs"]["y"], plots["transmittedPkgs"]["ci"])
	pl.savefig("transmittedPkgs.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Nb lost packages")
	pl.errorbar(X, plots["lostPkgs"]["y"], plots["lostPkgs"]["ci"])
	pl.savefig("lostPkgs.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.xlabel(xlabel)
	pl.title("Nb dropped PKGs in L2")
	pl.errorbar(X, plots["droppedPkgs"]["y"], plots["droppedPkgs"]["ci"])
	pl.savefig("droppedPkgs.png")

	#i+=1
	#pl.subplot(j,k,i)
	pl.clf()
	pl.margins(0.05, 0.05)
	pl.title("CDF UL-SINRs reported")
	for scenario in natsorted(cdfs["SINRs"].keys()):
		plotCDF(cdfs["SINRs"][scenario], scenario)
	pl.legend(loc="best")
	pl.savefig("SINRs_CDF.png")

	#pl.subplots_adjust(.05, .1, .975, .9, .2, .45)

	del cdfs, plots

	#pl.savefig("graphs.png")
	#pl.show()




