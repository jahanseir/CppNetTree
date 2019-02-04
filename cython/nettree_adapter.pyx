
from node import Node as PyNode
from node import dist
from point import Point as PyPoint
from metric import Euclidean as PyEuclidean
import math
import time

from libcpp.vector cimport vector
from cython.operator cimport dereference as deref
from nettree_api cimport *

cdef class SNT:
    cdef NetTree[EagerPointLocation] *nt
    cdef Metric *metric
    cdef dict __dict__
    cdef public float tau, cp, cc, cr
    
    def __cinit__(self, float tau, float cp, float cc, float cr = 0):
        self.metric = new Euclidean[float]()
        self.nt = new NetTree[EagerPointLocation](self.metric, tau, cp, cc, cr)
        self.root = None
        self.nodedict = {}
        self.tau = tau
        self.cp = cp
        self.cc = cc
        self.cr = self.nt.GetCR()
         
    def __dealloc__(self):
        del self.nt
        del self.metric
         
    def construct(self, points, pointlocation):
        cdef vector[const BasePoint*] cpoints
        cdef vector[float] coords
        for p in points:
            coords=p.coords
            cpoints.push_back(new Point[float](coords))
        t0 = time.time()
        self.nt.Construct(cpoints)
        t1 = time.time()
        cdef Node *croot = self.nt.GetRoot()
        coords = self.getcoords(<Point[float]*>croot.GetPoint())
        pyroot = PyNode(PyPoint(coords, PyEuclidean()), croot.GetLevel())
        self.generatetree(croot, pyroot)
        self.root = pyroot
        return t1 - t0
    
    def minlevelrelatives(self, n1, n2):
        return math.ceil(math.log(dist(n1, n2) / self.cr) / math.log(self.tau))         
    
    cdef generatetree(self, Node *croot, pyroot):
        if croot == NULL:
            return
        cdef set_nodes children = croot.GetChildren()
        cdef Node *cchild = NULL
        cdef set_nodes relatives
        cdef Node *crel = NULL
        for cchild in children:
            pychild = self.getpynodefromcnode(cchild)
            pyroot.addch(pychild)
            relatives = cchild.GetRelatives()
            for crel in relatives:
                if crel != cchild:
                    pyrel = self.getpynodefromcnode(crel)
                    pychild.addrel(pyrel)
            self.generatetree(cchild, pychild)
                
    cdef getpynodefromcnode(self, Node *cnode):
        coords = self.getcoords(<Point[float]*>cnode.GetPoint())
        cdef float level = <int>cnode.GetLevel()
        if level == INF_N:
            level = float('-inf')
        if level == INF_P:
            level = float('inf')
        pynode = PyNode(PyPoint(coords, PyEuclidean()), level)
        self.nodedict[pynode] = self.nodedict.get(pynode, pynode)
        return self.nodedict[pynode]
        
    cdef getcoords(self, Point[float] *p):
        lst = []
        cdef size_t i = 0
        for i in range(p.Dimension()):
            lst.append(deref(<float*>deref(p)[i]))
        return lst

            
            
            
            
