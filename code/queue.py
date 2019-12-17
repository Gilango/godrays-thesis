#!/usr/bin/python
import os,sys
import glob
import re
from dpapipetools import *

def createPBSFiles(submittingList, scriptFolder, renderCmd, outputPath):
	for File in submittingList:
		script = "#!/bin/bash\nsource /group/dpa/scripts/dpabashrcFile\n"
		scriptFileName, num, type = File.split(".", 2)
		outputFile = scriptFolder + scriptFileName + DpaPipeFormattedFrame(int(num)) + ".sh"
		fp = open(outputFile, "w")
		script += renderCmd + " -objf " + File + " -CF " + num
		script += "\nchmod 777 " + outputPath + "*.exr"
		fp.write(script)
		fp.close()
		cmd = "chmod 777 " + outputFile
		os.system(cmd)
		cmd = "cqsubmittask cheddar " + outputFile
		#print cmd
		os.system(cmd)
		
def main():
	outputPath = "/DPA/moosefs/PeanutButterJelly/prod/shot01/assets/godrays/0003/1920x817/exr/"
	scriptFolder = "/DPA/moosefs/PeanutButterJelly/prod/shot01/render/godrays/0003/"
	objPath = sys.argv[1]
	renderRange = sys.argv[2]
	startFrame, endFrame = renderRange.split("-")
	objFiles = glob.glob1(objPath,"*.obj")
	objFiles.sort()
	submittingList = []
	baseName = ''
	num = ''
	type = ''
	print startFrame
	print endFrame
	for objFile in objFiles:
		baseName, num, type = objFile.split(".", 2)
		if int(num)>= int(startFrame) and int(num)<= int(endFrame):
			submittingList.append(objFile)
	renderCmd = "/home/gilango/Thesis/svnCode/godrays -eye 1600.0476 42.61104 866.57688 -aim 46.72584 313.09056 -104.05872 -up 0.11355763145562091 0.99021434114989937 0.081118573207744599 -lpos 1929.81072 4226.47872 -2330.71416 -laim 0.0 1219.2 0.0 -path " + outputPath + " -objpath " + objPath
	createPBSFiles(submittingList, scriptFolder, renderCmd, outputPath)

if __name__ == "__main__":
    main()
