import numpy as np
from pathlib import Path
import pickle
import copy
import sys
from PIL import Image


def sigmoid(intrare,derivate=False):
    if(not derivate):
        f=lambda x: np.sin(x)
    else:
        f=lambda x:  np.cos(x)
    return f(intrare)

def mutate(x,Rate):
    if(np.random.random()<Rate):
        return x+np.random.normal(0,1)*x
    elif np.random.random()<Rate/2:
        return np.random.random()*x
    else:
        return x
class Layer:
    #nr de noduri precedent ,nr de noduri curent ,layerul urmator
    def __init__(self,pNoNodes,cNoNodes,nLayer,funct=sigmoid,learningRate=0.001,b=1,mutateRate=0.8):
        self.nLayer=nLayer
        if(np.random.random()>mutateRate):
            self.weights=np.random.normal(0,10,(pNoNodes,cNoNodes))
        else:
            self.weights=np.random.rand(pNoNodes,cNoNodes)
        self.b=b
        self.f=funct
        self.learningRate=learningRate
        self.mutateRate=mutateRate

    def forward(self,inA):
        xIn,yIn=inA.shape
        xW,yW=self.weights.shape
        if(yIn != xW):
            mesage='Input shape {} don\'t match the Weights shape {} to be multiplied'.format(inA.shape,self.weights.shape)
            raise ValueError(mesage)
            
        self.nodes=np.dot(inA,self.weights)+self.b
        self.fNodes=self.f(self.nodes)
        if self.nLayer is not None:
            return self.nLayer.forward(self.fNodes)
        else:
            return self.fNodes
        
    def backpropag(self,target):
        if self.nLayer is None:
            self.delta=-(target-self.fNodes)*self.f(self.nodes,derivate=True)
        else:
            w,d=self.nLayer.backpropag(target)
            self.delta=np.dot(d,(w.T))*self.f(self.nodes,derivate=True)
        w=self.weights
        dW=np.dot(self.delta,self.fNodes.T)
        self.weights-=self.learningRate* dW
        self.b-=self.learningRate* self.delta
        return (w,self.delta)
    
    def __deepcopy__(self):
        pNoNodes,cNoNodes=self.weights.shape
        if self.nLayer is None :
            nLayer=None
        else:
            nLayer=self.nLayer.__deepcopy__()
        
        new_object=Layer(pNoNodes,cNoNodes,nLayer,self.f,self.learningRate,self.b)
        new_object.weights=copy.deepcopy(self.weights)
        return new_object
    def mutate(self,):
        if not isinstance(self.b,list):
            _,c=self.weights.shape
            self.b=[self.b for _ in range(c)]
        #mutate Bias
        self.b=[mutate(x,self.mutateRate) for x in self.b]
        #mutate Weights
        mf= np.vectorize(mutate,otypes=[np.float64])
        self.weights=mf(self.weights,self.mutateRate)
class NN:
    """Neuronal Network class"""
    fileName='neuronalNet.pkl'
    def __init__(self,net,loadIfExist=True):
        if loadIfExist and Path(self.fileName).exists():
            this=self.load()
            self.layere=this.layere[:]
        elif isinstance(net,list):
            pNoNodes=net[-2]
            self.layere=[Layer(net[-2],net[-1],None,sigmoid)]
            for i in net[-3::-1]:
                pNoNodes,cNoNodes=i,pNoNodes
                self.layere.insert(0,Layer(pNoNodes,cNoNodes,self.layere[0]))
        elif isinstance(net,NN):
            self.layere=[net.layere[0].__deepcopy__()]
            l=self.layere[-1].nLayer
            while l is not None:
                self.layere.append(l)
                l=self.layere[-1].nLayer
    def load(self):
        with open(self.fileName, 'rb') as input:
            return pickle.load(input)
    def save(self):
        with open(self.fileName, 'wb') as output:  # Overwrites any existing file.
            pickle.dump(self, output, pickle.HIGHEST_PROTOCOL)
    def forward(self,inputM):
        return self.layere[0].forward(inputM)
    def bacpropag(self,X):
        return self.layere[0].backpropag(X)
    def sError(self,target,inM):
        output=self.forward(inM)
        return output,self.error(output,target)
    def error(self,output,target):
        f=lambda x: x**2
        err=(1/2)*np.sum(f(output-target))
        return err
    def yTrain(self,inM,X,no=1):
        for i in range(no):
            o,e=self.sError(X,inM)
            w,d=self.bacpropag(X)
            yield (i+1,o,e,w,d)
            
    def __enter__(self):
        return self
    def __exit__(self, exc_type, exc_value, traceback):
        self.save()
    def __deepcopy__(self):
        return NN(self,False)
    def mutate(self,no=1):
        for _ in range(no):
            map(lambda x:x.mutate(),self.layere)

def readImage(i):
    pi=Image.open(i)
    a=list(pi.getdata())
    a=np.array([[x for t in a for x in t]])
    #a=np.asarray(a).reshape(-1)
    return a
def aproxValue(fileName):
    p=(readImage(fileName))
    lp=(len(p))
    with NN(None,True)as n:
        return n.forward(p)

def train(fileName,expectedRsult):
    expectedRsult=1 if expectedRsult==1 else -1
    p=(readImage(fileName))
    lp=(len(p))
    with NN(None,True)as n:
        for i,o,e,w,d in n.yTrain(p,expectedRsult):
            return o,e
argv=sys.argv[1:]
l=len(argv)
if(l==0):
    lp=30000
    n=NN([lp,int(lp/100),int(lp/600),int(lp/1200),10,5,1],False)
    n.save()
elif(l==1):
    r=aproxValue(argv[0])
    print ((r[0][0]+1)/2)
else:
    o,e=train(argv[0],np.float64(argv[1]))
    print((o[0][0]+1)/2)

        
