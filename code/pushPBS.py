#!/usr/bin/python
#To push PBS scripts to the queue
import sys
import os
import glob

def CmdLineFindIndex( tag ):
        for i in range(len(sys.argv)):
                if sys.argv[i] == tag:
                        return i
        return -1

def CmdLineFind( tag, defaultValue ):
        i = CmdLineFindIndex(tag)
        if i > 0:
                if i < len(sys.argv)-1:
                        return sys.argv[i+1]
        return defaultValue
        
basePath = CmdLineFind("-base","/newscratch/gilango/godrays/0006/" )
PBSPath = basePath + "PBSFiles/"
eFilePath = basePath + "eFiles/"
oFilePath = basePath + "oFiles/"

renderRange = CmdLineFind("-range", "1-200")
startFrame, endFrame = renderRange.split("-")
PBSFiles = glob.glob1(PBSPath,"*.pbs")
PBSFiles.sort()
submittingList = []
baseName = ''
num = ''
type = ''


for PBSFile in PBSFiles:
        baseName, num, type = PBSFile.split(".", 2)
        if int(num)>= int(startFrame) and int(num)<= int(endFrame):
                submittingList.append(objFile)
                
for i, File in enumerate(submittingList):
        baseName, num, type = File.split(".", 2)
        padframe = num

        oFile = "-o " + oFilePath + baseName + "." + padframe + ".txt"
        eFile = "-e " + eFilePath + baseName + "." + padframe + ".txt"
        
        print "\nSubmitting job " + baseName + "." + padframe + ".pbs to queue"
        cmd = "qsub " + oFile + " " + eFile + " " + File
        os.system(cmd)
