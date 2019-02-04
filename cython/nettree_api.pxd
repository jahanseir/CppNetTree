from libcpp.vector cimport vector
from libcpp.unordered_set cimport unordered_set
from libcpp.limits cimport numeric_limits
from libcpp cimport bool

cdef extern from "../node.cpp":
    pass

cdef extern from "../pointlocation.cpp":
    pass

cdef extern from "../utils.hpp":
    cdef cppclass KeyHash[T]:
        size_t operator()(const T *obj) const
    cdef cppclass KeyEqual[T]:
        bool operator()(const T *obj1, const T *obj2) const

cdef extern from "../point.hpp":
    cdef cppclass BasePoint:
        size_t Dimension()
        void* operator[](int index)
    cdef cppclass Point[T](BasePoint):
        Point(vector[T] coords) except +
        Point(T *coords, size_t dimension) except +

cdef extern from "../metric.hpp":
    cdef cppclass Metric:
        float Distance(const BasePoint &p1, const BasePoint &p2)
    cdef cppclass Euclidean[T](Metric):
        Euclidean() except +
    cdef cppclass Manhattan[T](Metric):
        Manhattan() except +

ctypedef Node* nodeptr
        
cdef extern from "../node.hpp":
    ctypedef unordered_set[nodeptr] set_nodes
    cdef cppclass Node:
        set_nodes GetRelatives()
        set_nodes GetChildren()
        Node* GetParent()
        const BasePoint* GetPoint()
        int GetLevel()
          
cdef extern from "../nettree.hpp":
    cdef const int INF_P = numeric_limits[int].max()
    cdef const int INF_N = numeric_limits[int].min()
    cdef cppclass NetTree[T]:
        NetTree(Metric *metric, float tau, float c_p, float c_c, float c_r)
        void Construct(vector[const BasePoint*] points)
        float GetTau()
        float GetCR()
        Node* GetRoot()
       
cdef extern from "../pointlocation.hpp":
    cdef cppclass PointLocation[T]:
        pass
    cdef cppclass EagerPointLocation(PointLocation[EagerPointLocation]): 
        pass