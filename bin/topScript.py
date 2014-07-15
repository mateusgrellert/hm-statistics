from os import system
from utilities import *

qps = ['22','32']
videos = [l.split()[0] for l in open('video_sequences.inp').readlines()]
frames = [l.split()[1].strip('\n') for l in open('video_sequences.inp').readlines()]

video_cfgs = getConfigFiles(videos, "/home/grellert/hm-cfgs/")

structures = ['encoder_randomaccess_main.cfg']

bin_line = './TAppEncoderStatic'

for arg in argv[1:]:
	scripts.append('python '+arg)

for struct in structures:
	struct_line = bin_line + ' -c ../cfg/' + struct
	for cfg in video_cfgs:
		video_line = struct_line + ' -c ../cfg/per-sequence/' + cfg + ' --IntraPeriod=-1'
		for qp in qps:
			path = '_'.join([videos[video_cfgs.index(cfg)], struct.strip('.cfg'), qp])
			
			final_line = video_line + ' --QP='+qp+' --FramesToBeEncoded='+frames[video_cfgs.index(cfg)]
			final_line += ' > ' + path + '.txt'
			print final_line
			system(final_line)
			system('python getFrameDetails.py '+ path + '.txt')	
			
			system('python parseStats.py *bestChoices.csv '+str(int(frames[video_cfgs.index(cfg)])-1))

			moveFilesToDir(path)

