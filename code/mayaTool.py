#!/usr/bin/python
#import sys;sys.path.append("/home/gilango/gowthamScripts/");import godrays;reload(godrays);godrays.main()

import maya.cmds as cmds
import os,sys
import glob
import re
import DPAfs 
from dpapipetools import *
import DPANamedTasker as dpaTasker

pattern = re.compile("^(\-?[0-9]+)\-?(\-?[0-9]+)?:?(\-?[0-9]+)?$")

position=0
aim=0
up=0
focalLength=0
aspectRatio=0
lPos=0
laim=0
totalObjFiles=0
rootPath = 0
outputPath = 0
unit = 0
renderall = 0
lColor = 0
colorShiftFlag = 0
closeMainWin = 0
assetFolder = 0
dpaPipeFlag = 0
queueName = 0
resolution = 0
version = 0
godraysFolderPath = 0
assetString = 0

def getVersionCallback( fspattern ):
    v = '0001'
    vnb = 0
    version = DPAfs.whatsNext(fspattern)
    if version != []:
        lastv = version[-1]
        if lastv[0] == '0':
            if int(lastv) > vnb:
                vnb = int(lastv)
    if vnb > 0 :
        v = "%04d" % (vnb + 1)
    return v
    
def browseFiles(*args):
	global rootPath
	window2 = cmds.fileDialog2(caption="Please select folder to render godrays", dir=rootPath ,fileFilter="*.obj", dialogStyle=2, fm=2, okc= "obj folder")
	rootPath = str(window2[0]) + "/"
	cmds.textField("objPath", e=1, text=rootPath)
	global totalObjFiles
	totalObjFiles = len(glob.glob1(rootPath,"*.obj"))
	cmds.textField("objFiles", e=1, en=False, text=totalObjFiles)
	if totalObjFiles > 0:
		cmds.checkBox("renderAll", edit=True, value=True)
		cmds.textField("renderFiles", edit=True, en = False)
		
def outputFiles(*args):
	global outputPath
	window3 = cmds.fileDialog2(caption="Please select asset folder", dir=outputPath, dialogStyle=2, fm=2, okc= "asset folder")
	outputPath = str(window3[0]) + "/"
	cmds.textField("imagePath", e=1, text=outputPath)
	
def findCamParam(camname):	
	global position, aim, up, focalLength, aspectRatio
	position = cmds.camera(camname, q=True, p=True)
	aim = cmds.camera(camname, q=True, wci=True)
	up = cmds.camera(camname, q=True, wup=True)
	focalLength = cmds.camera(camname, q=True, fl=True)
	aspectRatio = cmds.camera(camname, q=True, ar=True)
	
	
def findLightParam(lightName):
	global lPos, lColor
	lPos=cmds.xform(lightName,q=1,ws=1,rp=1)
	colorAttr = str(lightName) + ".color"
	tmp = str(cmds.getAttr(colorAttr))
	lColor = re.findall("\d+.\d+",tmp)
	
	cmds.textField("red", e=1, text=str(lColor[0]))
	cmds.textField("green", e=1, text=str(lColor[1]))
	cmds.textField("blue", e=1, text= str(lColor[2]))
		
def printCamItem(item):
	camname = item
	findCamParam(camname)
	
def printLightItem(item):
	lightName = item
	findLightParam(lightName)
		
def renderallOn(*args):
	cmds.textField("renderFiles", edit=True, en = False, text=None)
	global renderall
	renderall = 1
	
def renderallOff(*args):
	cmds.textField("renderFiles", edit=True, en = True)	
	global renderall
	renderall = 0
	
def iorOn(*args):
	cmds.textField("ior", edit=True, en = True, text="1.33")	
		
def nbigOn(*args):
	cmds.textField("nbig", edit=True, en = True, text="20")	
	
def thresOn(*args):
	cmds.textField("threshold", edit=True, en = True, text="0.00001")	
	
def colorOn(*args):
	cmds.textField("redShift", edit=True, en = True, text="0.0758")
	cmds.textField("greenShift", edit=True, en = True, text="0.02197")
	cmds.textField("blueShift", edit=True, en = True, text="0.0025")		
	
def dpaPipeOn(*args):
	global dpaPipeFlag
	dpaPipeFlag = 1
	cmds.textField("imagePath", edit=True, en=False, text=outputPath)
	cmds.disable( 'select' )
	cmds.textField("versionNo", edit=True, en=False, text=version)	
	
def dpaPipeOff(*args):
	global dpaPipeFlag
	dpaPipeFlag = 0	
	cmds.textField("imagePath", edit=True, en=True, text=assetFolder)
	cmds.disable( 'select' , v=False )
	cmds.textField("versionNo", edit=True, en=True, text="")
		
def deleteWindow(*args):
	cmds.deleteUI("error")
	global closeMainWin
	closeMainWin = 0
	
def errorWindow(msg):
	win = cmds.window("error", title="Error", w=600, h=100, mnb=False, mxb=False, sizeable=False)
	cmds.rowColumnLayout( numberOfColumns=1 )
	cmds.text(label=msg, w=300, h=50)
	cmds.button(label='OK', command=deleteWindow)
	cmds.showWindow(win)
		
def getArray(elemList):
	if len(elemList) == 1:
		print elemList[0]
		list1=[]
		list1.append(int(elemList[0]))
		return list1
	if len(elemList) == 2:
		list1=[]
		x1 = int(elemList[0])
		x2 = int(elemList[1])
		list1 = xrange(x1, x2+1)
		return list1
	if len(elemList) == 3:
		list1=[]
		x1 = int(elemList[0])
		x2 = int(elemList[1])
		inc = int(elemList[2])
		list1 = xrange(x1, x2+1, inc)
		return list1			
		
def updateAsset(*args):
	newAssetName = cmds.textField("assetName", query=True,text=True)
	
	if newAssetName == "":
		newAssetName = "godrays"
	
	global assetString
	assetString = DPAfs.DpaPipeShow() + DPAfs.fsseparator + DPAfs.DpaPipeStage() + DPAfs.fsseparator + DPAfs.DpaPipeShot() + DPAfs.fsseparator + "assets" + DPAfs.fsseparator + newAssetName
	global version
	version = getVersionCallback(assetString)
	global godraysFolderPath
	godraysFolderPath = assetFolder + newAssetName + "/" + version + "/" + resolution + "/exr/" 
	global assetString
	assetString = assetString + DPAfs.fsseparator + version + DPAfs.fsseparator + resolution + DPAfs.fsseparator + "exr"
	cmds.textField("imagePath", edit=True, en=False, text=godraysFolderPath)
	cmds.textField("versionNo", edit=True, en=False, text=version)
		
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
		cmd = "cqsubmittask " + queueName + " " + outputFile
		#os.system(cmd)
		print cmd
		
def selectQueue(item):
	global queueName
	queueName = item
				
def render(*args):
	
	renderFlag = 1
	scriptFolder = ""
	frameNumber = cmds.textField("renderFiles", query=True,text=True)
		
	if totalObjFiles == 0:
		errorWindow("No obj files to render. Select a folder which has obj files.")
		renderFlag = 0
	
	else:	
	
		unitMeasure = 1.0
		
		if unit == "ft":
			unitMeasure = 0.032808
			
		if unit == "mm":
			unitMeasure = 10.0
			
		if unit == "m":
			unitMeasure = 0.01	
				
		laimx = cmds.textField("laimx",query=True,text=True)
		laimy = cmds.textField("laimy",query=True,text=True)
		laimz = cmds.textField("laimz",query=True,text=True)
		
		if not all([laimx, laimy, laimz]):
			errorWindow("Enter light aim value")
			renderFlag = 0
			
		else:	
			laimx = float(laimx)
			laimy = float(laimy)
			laimz = float(laimz)
			
			
			
			lposx = float(lPos[0])
			lposy = float(lPos[1])
			lposz = float(lPos[2])
			
			lpos = " -lpos " + str(lposx/unitMeasure) + " " + str(lposy/unitMeasure) + " " + str(lposz/unitMeasure)
			lAim = " -laim " + str(laimx/unitMeasure) + " " + str(laimy/unitMeasure) + " " + str(laimz/unitMeasure)
			lcolor = " -rgb " + str(lColor[0]) + " " + str(lColor[1]) + " " + str(lColor[2])
			camPos = " -eye " + str(position[0]/unitMeasure) + " " + str(position[1]/unitMeasure) + " " + str(position[2]/unitMeasure)
			camAim = " -aim " + str(aim[0]/unitMeasure) + " " + str(aim[1]/unitMeasure) + " " + str(aim[2]/unitMeasure)
			camUp = " -up " + str(up[0]/unitMeasure) + " " + str(up[1]/unitMeasure) + " " + str(up[2]/unitMeasure) 
			assetName = cmds.textField("assetName", query=True,text=True)
			name=""
			if assetName == "":
				errorWindow("Enter Asset name")
				renderFlag = 0
			else:
				global version
				imgName = assetName + "-" + version
				
				name = " -name " + imgName
			path = " -path " + outputPath
			objpath = " -objpath " + rootPath
			renderCmd = "/home/gilango/Thesis/svnCode/godrays" + name + path + objpath + camPos + camAim + camUp + lpos + lAim + lcolor
			
			ior = cmds.textField("ior", query=True,text=True)			
			if ior == "":
				errorWindow("Enter index of refraction value")
				renderFlag = 0
			elif ior != "1.33":	
				renderCmd += " -ior " + ior
				renderFlag = 1
					
			nbig = cmds.textField("nbig", query=True,text=True)
			if nbig == "":
				errorWindow("Enter nBig value")	
				renderFlag = 0
			elif nbig != "20":	
				renderCmd += " -nbig " + nbig
				renderFlag = 1
					
			threshold = cmds.textField("threshold", query=True,text=True)
			if threshold == "":
				errorWindow("Enter threshold value")
				renderFlag = 0
			elif threshold != "0.00001":	
				renderCmd += " -threshold " + threshold
				renderFlag = 1
					
			rShift = cmds.textField("redShift", query=True,text=True)
			gShift = cmds.textField("greenShift", query=True,text=True)
			bShift = cmds.textField("blueShift", query=True,text=True)
			if not all([rShift, gShift, bShift]):
				errorWindow("Enter color shift value")
				renderFlag = 0
			else:							
				renderCmd += " -b " + rShift + " " + gShift + " " + bShift
				renderFlag = 1
				
			global renderall
			renderall = cmds.checkBox("renderAll", query = True, v=True)
			objFiles = glob.glob1(rootPath,"*.obj")
			objFiles.sort()
			submittingList = objFiles
			if renderall == False:
				objFiles = glob.glob1(rootPath,"*.obj")
				objFiles.sort()
				del submittingList[:]
				baseName = ''
				num = ''
				type = ''	
				frameCount = []
				for objFile in objFiles:
					baseName, num, type = objFile.split(".", 2)
					frameCount.append(int(num))
				
				frameList = []
				frameList = frameNumber.split(',')
				for i in range (len(frameList)):
					match = pattern.match(frameList[i])
					if(match):
						mylist = match.groups()
						elemList = [s for s in mylist if s != None and s.isdigit()]
						frameNumbers = getArray(elemList)
						for id in frameNumbers:
							if id in frameCount:
								fileName = baseName + "." + DpaPipeFormattedFrame(id) + "." + type
								submittingList.append(fileName)
								
							else:
								errorWindow("Entered frame numbers are not in the range")
								renderFlag = 0
					
			if renderall == 1:
				renderFlag = 1
					
			if renderFlag:
				
				if dpaPipeFlag == 1:
					components = DPAfs.DPApwd()
					currentShowStageShotRender = components["show"] + DPAfs.fsseparator + components["stage"] + DPAfs.fsseparator + components["shot"] + DPAfs.fsseparator + "render" 
					elem = outputPath.split('/')
					elem = elem[3:]
					fsString = DPAfs.joinFSString(elem)
					components = DPAfs.evaluateFSString(fsString)
				
					if components["matches"] == 0:
						errorWindow("Show=stage=shot is not set correctly")
						renderFlag = 0
					else:	
						renderScripts = currentShowStageShotRender + DPAfs.fsseparator + assetName
						cmd = "dpacreateworkitem " + renderScripts
						os.system(cmd)
						components = DPAfs.evaluateFSString(renderScripts)
						global version
						scriptFolder = components["unixpath"] + "/maya/" + version + "/"
						cmd = "mkdir " + scriptFolder
						os.system(cmd)
						cmd = "chmod 777 -R " + scriptFolder
						os.system(cmd)
						renderFlag = 1
			
				if dpaPipeFlag == 0:
					cmd = "mkdir " + outputPath + "scripts/"
					os.system(cmd)
					scriptFolder = outputPath + "scripts/"
				
				cmd = "dpacreateasset " + assetString
				os.system(cmd)			
				createPBSFiles(submittingList, scriptFolder, renderCmd, outputPath)
				closeMainWin = 1			
			
	if closeMainWin:
		cmds.deleteUI("godRaysOptions")

def setParam(lights):
	
	components = DPAfs.DPApwd()
	currentShowStageShotAssets = components["show"] + DPAfs.fsseparator + components["stage"] + DPAfs.fsseparator + components["shot"] + DPAfs.fsseparator + "assets"
	components = DPAfs.evaluateFSString(currentShowStageShotAssets)
	if components == " ":
		errorWindow("Show=stage=shot is not set correctly")
	global assetFolder
	assetFolder = components["unixpath"] + "/"
	
	godraysComp = currentShowStageShotAssets + DPAfs.fsseparator + "godrays"
	global version
	version = getVersionCallback(godraysComp)
	
	imgWidth = str(cmds.getAttr('defaultResolution.width'))
	imgHeight = str(cmds.getAttr('defaultResolution.height'))
	global resolution
	resolution = str(imgWidth) + "x" + str(imgHeight)
	
	global assetString
	assetString = godraysComp + DPAfs.fsseparator + version + DPAfs.fsseparator + resolution + DPAfs.fsseparator + "exr"
	global godraysFolderPath
	godraysFolderPath = assetFolder + "godrays/" + version + "/" + resolution + "/exr/"
	
	if cmds.window("godRaysOptions", exists=True):
		cmds.deleteUI("godRaysOptions") #delete it
    
	# create window
	window = cmds.window("godRaysOptions", title="God rays render", w=430, h=800, mnb=False, mxb=False, sizeable=True)
	
	#this is to select a folder which has all the obj files for godrays
	#window2 = cmds.fileDialog2(caption="Please select folder to render godrays",fileFilter="*.obj", dialogStyle=1, fm=3)
	
	# create layout
	mainUI = cmds.scrollLayout("scrollLayout")
		
	#########################
	# obj Section
	frame = cmds.frameLayout(label="Obj path Selection", borderStyle="in", p=mainUI, w=400, h=200)
	assetLayout = cmds.rowLayout(p=frame, w=400, nc=3)
	cmds.text(label="Obj folder path", p=assetLayout, w=100, h=50)
	global rootPath
	rootPath = assetFolder
	#rootPath = cmds.workspace(query=1, dir=1)
	cmds.textField("objPath", p=assetLayout, w=200, text=rootPath)
	selectButton=cmds.button(label='select', command=browseFiles, p=assetLayout)
	objLayout = cmds.rowLayout(p=frame, w=400, nc=2)
	global totalObjFiles
	totalObjFiles = len(glob.glob1(rootPath,"*.obj"))
	cmds.text(label="Total obj files", p=objLayout, w=100, h=50)
	cmds.textField("objFiles", p=objLayout, w=200, en=False, text=totalObjFiles)
	ckLayout = cmds.rowLayout(p=frame, w=400, nc=2)
	cmds.checkBox("renderAll", label='Render All',enable=True,value=False, p=ckLayout, onc=renderallOn, ofc=renderallOff)
	frameLayout = cmds.rowLayout(p=frame, w=400, nc=2)
	cmds.text(label="Frames", p=frameLayout, w=100, h=50)
	cmds.textField("renderFiles", p=frameLayout, w=200, text="")
	
	#########################
	# output image Section
	renderImages = cmds.frameLayout(label="Output image properties", borderStyle="in", p=mainUI, w=400, h=225)
	dpaPipeLayout = cmds.rowLayout(p=renderImages, w=400, nc=1)
	cmds.checkBox("useDPAPipe", label='Use DPA Pipe',enable=True,value=True, p=dpaPipeLayout, onc=dpaPipeOn, ofc=dpaPipeOff)
	global dpaPipeFlag
	dpaPipeFlag = 1	
	renderImageLayout = cmds.rowLayout(p=renderImages, w=400, nc=3)
	cmds.text(label="Output folder path", p=renderImageLayout, w=100, h=50)
	global outputPath
	outputPath = godraysFolderPath
	#outputPath = cmds.workspace(query=1, dir=1)
	cmds.textField("imagePath", p=renderImageLayout, w=200, en=False, text=outputPath)
	selectButton=cmds.button('select', command=outputFiles, p=renderImageLayout, en=False)
	imgLayout = cmds.rowLayout(p=renderImages, w=400, nc=2)
	cmds.text(label="Asset name", p=imgLayout, w=100, h=50)
	cmds.textField("assetName", p=imgLayout, w=200, text="godrays", cc=updateAsset)
	versionLayout = cmds.rowLayout(p=renderImages, w=400, nc=2)
	cmds.text(label="Version", p=versionLayout, w=100, h=50)
	cmds.textField("versionNo", p=versionLayout, w=200, text=version, en=False)
	
	
	#########################
	# camera Section
	camFrame = cmds.frameLayout(label="Camera attributes", borderStyle="in", p=mainUI, w=400, h=100)
	camAssetLayout = cmds.rowLayout(p=camFrame, w=400, nc=2)
	cmds.text(label="Camera", p=camAssetLayout, w=100, h=50)
	#cmds.textField("cameraName", p=camAssetLayout, w=200, en=False, text=camname)
	camList = cmds.ls(cameras=1)
	renCam = []
	camname = ["nocamera"]
	for cam in camList:
		renderable = cam + '.renderable'
		if cmds.getAttr(renderable): #and not cmds.getAttr(ortho):
			renCam.append(cam)
			#print renCam
			camname = cmds.listRelatives(renCam[0], p=1)
	if renCam == []:
		camname = cmds.listRelatives(camList[0], p=1)
	camname = camname[0]

	cammenu = cmds.optionMenu( "comboBox_cam", changeCommand=printCamItem)
	cmds.menuItem(label=camname, p=cammenu)

	for cam in camList:
		name = cmds.listRelatives(cam, p=1)
		cname = str(name[0])
		if cname != camname:
			cmds.menuItem(label=cname, p=cammenu)
			
	findCamParam(camname)
					
	resAssetLayout = cmds.rowLayout(p=camFrame, w=400, nc=2)
	cmds.text(label="Resolution", p=resAssetLayout, w=100, h=50)
	cmds.textField("camRes", p=resAssetLayout, w=200, en=False, text=resolution)
	
	#########################
	# light Section
	lightFrame = cmds.frameLayout(label="Light parameters", borderStyle="in", p=mainUI, w=400, h=200)
	lightAssetLayout = cmds.rowLayout(p=lightFrame, w=400, nc=2)
	cmds.text(label="Light Name", p=lightAssetLayout, w=100, h=50)
	transformList = cmds.listRelatives(lights, parent=True, fullPath=True)
	#cmds.textField("lightName", p=lightAssetLayout, w=200, en=False, text=str(transformList[0]))
	lightmenu = cmds.optionMenu( "comboBox_light", changeCommand=printLightItem)
	for light in transformList:
		lightName = str(light)
		cmds.menuItem(label=lightName, p=lightmenu)
	unitLayout = cmds.rowLayout(p=lightFrame, w=400, nc=2)
	#cmds.text(label="Units", p=unitLayout, w=100, h=50)
	global unit
	unit = cmds.currentUnit( query=True, linear=True )
	lightAimLayout = cmds.rowLayout(p=lightFrame, w=300, nc=4)
	cmds.text(label="Point on Ocean Surface", p=lightAimLayout, w=120, h=50, ann="The point on the ocean surface to calculate directional vector for light")
	cmds.textField("laimx", p=lightAimLayout, w=70, text="0.0")
	cmds.textField("laimy", p=lightAimLayout, w=70, text="40.0")
	cmds.textField("laimz", p=lightAimLayout, w=70, text="0.0")
	
	lightColorLayout = cmds.rowLayout(p=lightFrame, w=300, nc=4)
	cmds.text(label="Light Color", p=lightColorLayout, w=100, h=50)
	cmds.textField("red", p=lightColorLayout, w=70, text=None)
	cmds.textField("green", p=lightColorLayout, w=70, text=None)
	cmds.textField("blue", p=lightColorLayout, w=70, text=None)
	firstOption = transformList[0]
	findLightParam(firstOption)
	
	#########################
	# Parameters Section
		
	paramFrame = cmds.frameLayout( label='Other Parameters', borderStyle='in', p=mainUI, w=400)
	ParamLayout1 = cmds.rowLayout(p=paramFrame, w=400, nc=3)
	cmds.text(label="Index Of Refraction", p=ParamLayout1, w=100, h=50, ann="Index of Refraction Value")
	cmds.textField("ior", p=ParamLayout1, en=True, w=50, text= "1.33")
	cmds.button('iorDefault', label='IOR Default', command=iorOn, p=ParamLayout1, en=True)
	
	ParamLayout2 = cmds.rowLayout(p=paramFrame, w=400, nc=3)
	cmds.text(label="Max Iterations", p=ParamLayout2, w=100, h=50, ann="Max iteration value for finding the root")
	cmds.textField("nbig", p=ParamLayout2, en=True, w=50, text= "20")
	cmds.button('nbigDefault', label='Max Iterations Default', command=nbigOn, p=ParamLayout2, en=True)
	
	ParamLayout3 = cmds.rowLayout(p=paramFrame, w=400, nc=3)
	cmds.text(label="Iteration Threshold", p=ParamLayout3, w=100, h=50, ann="The min threshold value to stop the root finding process")
	cmds.textField("threshold", p=ParamLayout3, en=True, w=50, text="0.00001")
	cmds.button('thresholdDefault', label='Iteration Threshold Default', command=thresOn, p=ParamLayout3, en=True)
	
	ParamLayout4 = cmds.rowLayout(p=paramFrame, w=400, nc=5)
	cmds.text(label="Color shift value", p=ParamLayout4, w=100, h=50, ann="RGB value for color attenuation")
	cmds.textField("redShift", p=ParamLayout4, en=True, w=70, text="0.0758")
	cmds.textField("greenShift", p=ParamLayout4, en=True, w=70, text="0.02197")
	cmds.textField("blueShift", p=ParamLayout4, en=True, w=70, text="0.0025")
	cmds.button('colorDefault', label='Color Default', command=colorOn, p=ParamLayout4, en=True)

	
	#########################
	# render button Section
		
	buttonFrame = cmds.frameLayout( label='Render', borderStyle='in', p=mainUI, w=400)
	queueList = []
	for q in dpaTasker.getNamedQueues():
		queueList.append(q)
	queueMenu = cmds.optionMenu( "comboBox_queue", changeCommand=selectQueue)
	cmds.menuItem(label="cheddar", p=queueMenu)
	for queue in queueList:
		if queue is not "cheddar":
			cmds.menuItem(label=queue, p=queueMenu)
	global queueName		
	queueName = "cheddar"		
	exportButton=cmds.button(label='Render', command=render, p=buttonFrame, w=80, h=40)
  
	cmds.showWindow(window)
				
def main():

	#lights = cmds.ls(sl = True, dag = True, type = "light")
	lights = cmds.ls(type = "light")
	if lights == "":
		errorWindow("Please create a light source to use the tool.")
	else:		
		setParam(lights)		

	
