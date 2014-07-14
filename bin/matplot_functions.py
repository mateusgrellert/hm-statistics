import numpy as np
import pylab
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.cm as cmx
from collections import OrderedDict


def makeColors(name, nColors):
	cmap = cmx.get_cmap(name='terrain') 
	incr = 1.0 / nColors

	mycolors = []
	for i in np.arange(0,1,incr):
		mycolors.append(cmap(i))
	return mycolors


def plotChart(ax,series_labels, x, x_ticklabels, x_label, y, y_ticklabels, y_label):
	if not isinstance(y[0], float):
		for idx in range(0,len(y[0])):
			y_col = [row[idx] for row in y]
			if series_labels:
				ax.plot(x,y_col,'o-',linewidth=1.2, label=series_labels[idx])
			else:
				ax.plot(x,y_col,'o-',linewidth=1.2)
	else:
		if series_labels:
			ax.plot(x,y,'o-',linewidth=1.2, label =series_labels )
		else:
			ax.plot(x,y,'o-',linewidth=1.2)

	ax.set_xticks(x)
	if x_ticklabels:
		ax.set_xticklabels(x_ticklabels)
	if x_label:
		ax.set_xlabel(x_label)
	if y_ticklabels:
		ax.set_yticklabels(y_ticklabels)
	if y_label:
		ax.set_ylabel(y_label)
	plt.legend(fontsize=10,loc=0,ncol=2)

def adjustAxisInterval(x_axis,y_axis):
	if not(isinstance(y_axis[0], float)) :
		pad = 2**(max([max(x) for x in y_axis])/100.0)
		pylab.ylim([min([min(x) for x in y_axis])-pad, max([max(x) for x in y_axis])+pad])
	else:
		pad = 2**(max(y_axis)/100.0)
		pylab.ylim([min(y_axis)-pad, max(y_axis)+pad])
	
	pad = len(x_axis)/20.0
	pylab.xlim([-pad, len(x_axis)-(1-pad)])

def plotFigures(plots,app, N):
	frame_labels = []
	for i in range(0,N):
		frame_labels.append("Frame "+str(i))

	i = 0
	for plot in plots:
		figure = pylab.figure(dpi=200) 
		subplot = figure.add_subplot(111)
		x = range(0, len(plot.keys()))

		if app == "accum":
			plotChart(subplot, 0, x, plot.keys(), 0, plot.values(), 0, "Accumulated Occurrence")
		else:
			plotChart(subplot, frame_labels, x, plot.keys(), 0, plot.values(), 0, "Normalized Occurrence")
		adjustAxisInterval(x,plot.values())
		if len(x) > 10:
			figure.set_size_inches(10,6)
			saveFigure(figure, "matplot_"+ app+"_"+str(i)+".png", 'vertical')
		else:
			figure.set_size_inches(8,4)
			#plt.subplots_adjust(left=0.1, right=0.9, top=0.9, bottom=0.1)
			saveFigure(figure, "matplot_"+ app+"_"+str(i)+".png", '30')
		i+=1

def saveFigure(figure,path, rot):
	plt.tight_layout()
	figure.autofmt_xdate(rotation=rot, ha='center')
	figure.savefig(path)
	plt.close(figure)
