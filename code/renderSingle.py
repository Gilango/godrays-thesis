#!/usr/bin/python

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

nbCores = CmdLineFind("-nbcores", 24)
wally = CmdLineFind("-wall", 15)
frames = int(CmdLineFind("-TF", 119 ) )
name = CmdLineFind("-name", "gilango_godraysDeep")
email = int(CmdLineFind("-email", 0 ) )
objName = CmdLineFind("-obj", "/home/gilango/projects/godrays/triangle.0003.obj")

fileName = "/newscratch/gilango/godraysDeepImage/0001/PBSFiles/" + name + ".0001.pbs"
oFile = "-o /newscratch/gilango/godraysDeepImage/0001/oFiles/" + name + ".0001.txt"
eFile = "-e /newscratch/gilango/godraysDeepImage/0001/eFiles/" + name + ".0001.txt"
        
boilerplate = "#PBS -l select=1:ncpus=" + str(nbCores) + ":mem=24gb\n#PBS -l walltime=" + str(wally) + ":00:00\n"

if email == 1:
	boilerplate = boilerplate + "#PBS -m abe\n#PBS -M gilango@g.clemson.edu\n"
			
cmd = "/home/gilango/projects/godraysDeepImage/godrays -eye 1600.0476 42.61104 866.57688 -aim 46.72584 313.09056 -104.05872 -up 0.11355763145562091 0.99021434114989937 0.081118573207744599 -lpos 1929.81072 4226.47872 -2330.71416 -laim 0.0 1219.2 0.0 -deep 1 -divide 0 -name " + str(name) + " -objf " + objName 

boilerplate = boilerplate +  cmd

file = open(fileName, 'w')
file.write(boilerplate)
file.close()

print "\nSubmitting job " + name + ".0001.pbs to queue"
cmd = "qsub " + oFile + " " + eFile + " " + fileName
os.system(cmd)



