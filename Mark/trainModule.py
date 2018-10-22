from os import listdir
from os.path import isfile, join
import numpy as np
from NN4 import train
def getFilesFromFolder(path):
    return [join(path, f) for f in listdir(path) if isfile(join(path, f))]
def makeMap():
    l0=getFilesFromFolder("./savedImgs/0/")
    l1=getFilesFromFolder("./savedImgs/1/")
    d0= [(x, 0) for x in l0]
    d1= [(x, 1) for x in l1]
    d0.extend(d1)
    np.random.shuffle(d0)
    return d0
def tr():
    m=makeMap()
    for (f,o) in m:
        e=[1,0]if o==1 else [0,1]
        o1,e=train(f,e)
        o1=(o1+1)/2
        print(f,o,o1,e)
        #print("r.append("+str(o1[0][0])+")\nex.append("+str(o)+")\nerr.append("+str(e)+")\n")
        if(np.isnan( o1).any() ):
            break;

tr()
