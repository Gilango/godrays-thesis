#!/usr/bin/python

import os,sys
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

objPath = "/DPA/moosefs/PeanutButterJelly/prod/share/assets/oceanSurfaceObj/0010/256x256/obj/"
objFiles = glob.glob1(objPath,"*.obj")
objFiles.sort()
renderRange = CmdLineFind("-range", "1-200")
startFrame, endFrame = renderRange.split("-")
submittingList = []
baseName = ''
num = ''
type = ''
name = CmdLineFind("-name", "gilango_godraysDeep")
#print "Total files: " + str(len(objFiles))
for objFile in objFiles:
        baseName, num, type = objFile.split(".", 2)
        if int(num)>= int(startFrame) and int(num)<= int(endFrame):
                submittingList.append(objFile)
for i,file in enumerate(submittingList):
	baseName, num, type = file.split(".", 2)
	currentFrame = num	
	
	for x in range(0,10):
		newName = name + "." + str(x)
		cmd = "./godrays -deep 1 -divide 1 -eye 1600.0476 42.61104 866.57688 -aim 46.72584 313.09056 -104.05872 -up 0.11355763145562091 0.99021434114989937 0.081118573207744599 -lpos 1929.81072 4226.47872 -2330.71416 -laim 0.0 1219.2 0.0 -path /DPA/moosefs/PeanutButterJelly/prod/shot01/assets/godrays/0003/1920x817/exr/ -objf " + str(file) + " -CF " + currentFrame + " -objpath " + objPath + " -interval " + str(x) + " -name " + str(newName)
	#print "Rendering " + num + " frame"	
	os.system(cmd)
