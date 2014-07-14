import numpy as np
import pylab
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.cm as cmx
from collections import OrderedDict
from matplot_functions import *
from sys import argv
from os import system
from os import listdir

# returns all strings that match text for N consecutive frames
def getStrings(lines, text, N):
	s = []
	idx = 0
	for l in lines:
		if len(l) <= 2: continue

		if text in l:
			tok = l.split('\t')
			if tok[0] not in s:
				if 'PU' in tok[0]:
					tok[0] = tok[0].split("PU ")[1]
				s.append(tok[0])
		elif 'END' in l:
			idx += 1
			if idx == N:
				return s


def getAllStats(lines, N):
	allPUStrings = getStrings(lines, "PU", N)
	puDict = {}	
	accPu = {}

	for strr in allPUStrings:
		puDict[strr] = [0]*(N+1)		
		accPu[strr] = 0

	allTUStrings = getStrings(lines, "TU", N)
	tuDict = {}
	accTu = {}

	for strr in allTUStrings:
		tuDict[strr] = [0]*(N+1)		
		accTu[strr] = 0

	allModes = ['Inter', 'Intra', 'SKIP', 'Merge']
	modeDict = {}
	accMode = {}
	for strr in allModes:
		modeDict[strr] = [0]*(N+1)		
		accMode[strr] = 0



	idx = 1 # 0 is for the accumulated results
	for l in lines:
		if len(l) <= 2: continue

		if 'PU' in l:
			tok = l.split('\t')
			tok[0] = tok[0].split("PU ")[1]
			puDict[tok[0]][idx] = float(tok[1])
			puDict[tok[0]][0] += float(tok[1])
			accPu[tok[0]] += float(tok[1])
			
			tok2 = tok[0].split()			
			modeDict[tok2[0]][idx] += float(tok[1])
			modeDict[tok2[0]][0] += float(tok[1])
			accMode[tok2[0]] += float(tok[1])

		elif 'TU' in l:
			tok = l.split('\t')
			tuDict[tok[0]][idx] = float(tok[1])
			tuDict[tok[0]][0] += float(tok[1])
			accTu[tok[0]] += float(tok[1])

		elif 'END' in l:
			idx += 1
			if idx == N+1:
				return [[accPu, accMode, accTu], [puDict, modeDict, tuDict]]



def printToCsv(dicts, path):
	f = open(path, 'w')
	for d in dicts:
		for key, val in d.items():
			print >> f, key, "\t",
			if not isinstance(val, float):
				for v in val:
					print >> f, v, "\t",
			else:
				print >> f, val, "\t",
			print >> f, "\n",
		print >> f, "\n\n"

def printGOPToCsv(dicts, path, GOP=8):
	f = open(path, 'w')
	for d in dicts:
		for key, val in d.items():
			if len(val) < GOP:
				GOP = len(val)
				
			print >> f, key, "\t",
			acum = 0.0
			for i in range (0,len(val)):
				if i % (GOP-1) == 0 and i != 0:
					acum += val[i]
					print >> f, acum, "\t",
					acum = 0.0
				else:
					acum += val[i]

			print >> f, "\n",
		print >> f, "\n\n"
					
def moveFilesToDir(path):
	system("mkdir "+path + "; mv *csv *txt "+path)

def getConfigFiles(videos, cfg_path):
	cfgs = []
	files = listdir(cfg_path)

	for v in videos:
		for f in files:
			if v in f:
				cfgs.append(f)

	return cfgs
		

		
