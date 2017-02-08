

from sys import argv, exit
from numpy import mean, array, sqrt, cumsum, sort, arange, square
from pprint import pprint as pp
from glob import glob
import os
import pylab as pl
import itertools as it
from natsort import natsorted

def readDataFromFile(filePath):
	print filePath
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
			stats["appType"] = l[1]
			pkgs[l[3]] = stats

		if l[0] == "RX":
			pkgs[l[3]]["rxSize"] = int(l[7])
			pkgs[l[3]]["delay"] = float(l[13])

		if l[0] == "DROP_QUEUE":
			pkgs[l[3]]["dropped"] = True
			pkgs[l[3]]["droppedTime"] = float(l[7])

		#if l[0] == "UL_SINR":
			#sinr.append(float(l[4]))

	del lines
	del pkgLines
	#return pkgs.values(), sinr
	return pkgs.values()


def getMetricsForFile(filePath):
	#pkgs, sinr = readDataFromFile(filePath)
	pkgs = readDataFromFile(filePath)

	firstTx = 999999
	lastRx = 0

	lostPkgs = 0
	transmittedData = 0
	dataReceived = 0

	rxDelayList = list()
	#queueDelayList = list()
	droppedDelayList = list()
	users = dict()
	userAppType = dict()
	appTypes = set()

	for p in pkgs:
		transmittedData += 8.0*p["txSize"]/1000.0
		if p["user"] not in users: users[p["user"]] = 0.0
		userAppType[p["user"]] = p["appType"]
		appTypes.add(p["appType"])

		if p["txTime"] < firstTx:
				firstTx = p["txTime"]
		if p["delay"] == None:
			lostPkgs += 1
			if p["dropped"] == True:
				droppedDelayList.append((p["droppedTime"] - p["txTime"]) * 1000.0)
			#else:
				#queueDelayList.append(5.0-p["txTime"])
		else:
			if p["txTime"] + p["delay"] > lastRx:
				lastRx = p["txTime"] + p["delay"]

			rxDelayList.append(p["delay"] * 1000.0)
			dataReceived += 8.0*p["rxSize"]/1000.0
			users[p["user"]] += 8.0*p["rxSize"]/1000.0

	transmittedPkgs = len(pkgs)
	droppedPkgs = len(droppedDelayList)
	d = lastRx - firstTx
	users_throughput = { uid:(rxData/d) for uid,rxData in users.iteritems() }
	appTypeThroughput = { appType:list() for appType in appTypes }

	for uid, throughput in users_throughput.iteritems():
		appTypeThroughput[userAppType[uid]].append(throughput)
	maxThroughput = {appType:max(max(appTypeThroughput[appType]),1.0) for appType in appTypes}

	justiceRatios = [ t/maxThroughput[userAppType[uid]] for uid,t in users_throughput.iteritems() ]
	justiceRatio = square(sum(justiceRatios))/(sum(square(justiceRatios))*len(users))

	#print filePath
	#pp(users_throughput)
	#pp(maxThroughput)
	#print "appTypeThroughput"
	#pp(appTypeThroughput)
	#print

	stats = {#"rxDelayMean": mean(rxDelayList),
			"rxDelay": rxDelayList,
			#"queueDelayMean": mean(queueDelayList),
			#"queueDelay": queueDelayList,
			#"droppedDelayMean": mean(droppedDelayList),
			#"droppedDelay": droppedDelayList,
			#"userThroughputMean": mean(users_throughput),
			"userThroughput": users_throughput.values(),
			"RxThroughput": dataReceived/d,
			"TxThroughput": transmittedData/d,
			"deliveryRate": 100.0*(transmittedPkgs-lostPkgs)/transmittedPkgs,
			"transmittedPkgs": transmittedPkgs,
			"justiceRatio": justiceRatio,
			#"lostPkgs": lostPkgs,
			"droppedPkgs": droppedPkgs
			}
			#"SINRs": sinr}

	#pp(appTypeThroughput)
	for appType, throughputList in appTypeThroughput.iteritems():
		stats["justiceRatio_"+appType] = sum(throughputList)/(sum(square(throughputList)) * len(throughputList))
		stats["throughput_"+appType] = throughputList

	del maxThroughput
	#del appTypeThroughput
	del justiceRatios
	del pkgs
	del users
	return stats


def CalculateStatistics(lvals):
	vals = array(lvals)
	#pp(lvals)
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

def plotCDF(data, plabel, target=pl, mark=''):
	sortedData = sort(data)
	s = len(data)
	p = 100. * arange(s) / (s - 1)
	target.plot(sortedData, p, label=plabel, markevery=int(.1*s), marker=mark)
