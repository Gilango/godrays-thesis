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

objPath = "/DPA/moosefs/PeanutButterJelly/prod/share/assets/oceanSurfaceObj/0018/256x256/obj/"
objFiles = glob.glob1(objPath,"*.obj")
objFiles.sort()
renderRange = CmdLineFind("-range", "1-200")
startFrame, endFrame = renderRange.split("-")
submittingList = []
baseName = ''
num = ''
type = ''
threshold = 0.000000001
#print "Total files: " + str(len(objFiles))
for objFile in objFiles:
        baseName, num, type = objFile.split(".", 2)
        if int(num)>= int(startFrame) and int(num)<= int(endFrame):
                submittingList.append(objFile)
for i,file in enumerate(submittingList):
	baseName, num, type = file.split(".", 2)
	currentFrame = num			
	#cmd = "./godrays -name gilango_godrays_farplane -deep 0 -eye 1600.0476 42.61104 866.57688 -aim 46.72584 313.09056 -104.05872 -up 0.11355763145562091 0.99021434114989937 0.081118573207744599 -lpos 1929.81072 4226.47872 -2330.71416 -laim 0.0 1219.2 0.0 -path /DPA/moosefs/PeanutButterJelly/prod/shot01/assets/godrays/0003/1920x817/exr/ -objf " + str(file) + " -CF " + currentFrame + " -objpath " + objPath + " -path /DPA/moosefs/PeanutButterJelly/prod/share/assets/godrays/0006/1920x817/exr/"
	cmd = "./godrays -name godrays-0004 -deep 0 -eye -240.3990 859.74757 255.73768 -aim 7367.2429 3911.26963 -7804.039392 -up -0.18221034882498 0.964124043612407 0.19303941905615387 -lpos 1929.81072 4226.47872 -2330.71416 -laim 0.0 1219.2 0.0 -nplane 30.48 -path /DPA/moosefs/PeanutButterJelly/prod/s03/assets/godrays/0004/1920x817/exr/ -objf " + str(file) + " -CF " + currentFrame + " -objpath " + objPath
	#print "Rendering " + num + " frame"	
	os.system(cmd)
