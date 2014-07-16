#import numpy as np
#import pylab
#import matplotlib as mpl
#import matplotlib.pyplot as plt
#import matplotlib.cm as cmx
#from matplot_functions import *
from collections import OrderedDict
from sys import argv
from utilities import *


N=int(argv[2]) # number of frames
GOP=8 # GOP Size

fin = open(argv[1],'r')
video = argv[1].split('_')[0]
ext = argv[1].split('_')[-1][:-4]
inp_lines = fin.readlines()
fin.close()

[accumDicts, detailDicts] =  getAllStats(inp_lines,N)

for i in range(0,len(accumDicts)):
	if ext == 'bestChoices': # sort accum-wise
		accumDicts[i] = OrderedDict(sorted(accumDicts[i].items(), key=lambda t: t[1], reverse=True))
		detailDicts[i] = OrderedDict(sorted(detailDicts[i].items(), key=lambda t: t[1][0], reverse=True))
	else: # sort pu-wise
		accumDicts[i] = OrderedDict(sorted(accumDicts[i].items(), key=lambda t: sum([int(x) for x in t[0].split()[-1].split('x') if x.isdigit()]) - len(t[0].split()[0]), reverse=True))
		detailDicts[i] = OrderedDict(sorted(detailDicts[i].items(), key=lambda t: sum([int(x) for x in t[0].split()[-1].split('x') if x.isdigit()]) - len(t[0].split()[0]), reverse=True))

	for key, val in detailDicts[i].items():
		detailDicts[i][key] = val[1:]

printToCsv(accumDicts, video+'_'+ext+"_accumulated.csv")
printToCsv(detailDicts, video+'_'+ext+"_frame_detailed.csv")
printGOPToCsv(detailDicts, video+'_'+ext+"_GOP_detailed.csv", GOP)

#mycolors = makeColors('terrain',N)
#plt.rc('axes', color_cycle=mycolors)
#plotFigures(accumDicts, "accum", N) #accumulated plots
#plotFigures(detailDicts, "detail", N) #frame-detailed plots



