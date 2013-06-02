from pyassimp import *
import sys
scene=load('/home/yanni/C++/SpaceGame/assets/models/MD5/Bob.md5mesh')

def printNodes(curNode, tabtrack):
    for tab in range(0,tabtrack):
        sys.stdout.write('---')
    print ">" + str(curNode) + ' [' + str(len(curNode.meshes)) + ']'
    tabtrack+=1
    for i in curNode.children:
        printNodes(i, tabtrack)

printNodes(scene.rootnode, 0)
