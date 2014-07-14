from sys import argv

f = open(argv[1], 'r')
fout = open(argv[1] + '_HM_details.csv','w')
print >> fout, "\t".join(["POC", "ET", "bits"])

for l in f.readlines():
	if '[ET' in l:
		l = l.split()
		poc = l[1]
		et = l[l.index('[ET')+1]
		bits = l[l.index('bits')-1]
		print >> fout, "\t".join([poc, et, bits])

f.close()
