import unittest
import random

from snt_verify import SNTVerify
from nettree_adapter import SNT
from point import Point
from metric import Euclidean

class TestSNT(unittest.TestCase):
    def testconstruct(self):
        T = SNT(2, 1, 1, 4)
        p1 = Point([0], Euclidean())
        p2 = Point([2], Euclidean())
        p3 = Point([11], Euclidean())
        p4 = Point([28], Euclidean())
        print(T.construct([p2, p3, p4, p1], None))
        points = [p1, p2, p3, p4]
        ver = SNTVerify(T, points)
        ver.populate()
        self.assertTrue(ver.relativescorrect())
        self.assertTrue(ver.issemicompressed())
        self.assertTrue(ver.islocalnettree())
    
    def testrandom(self):
        metric = Euclidean()
        points = [Point([random.randint(-10000, 10000) for _ in range(3)], metric) for _ in range(2000)] 
        tmp = list()
        for p in points:
            if p in tmp:
                print('duplicate:', p)
            else:
                tmp.append(p)
        points = tmp
        tau = 5
        T = SNT(tau, 1, 1)
        print(T.construct(points, None))
        ver = SNTVerify(T, points)
        ver.populate()
        self.assertTrue(ver.relativescorrect())        
        self.assertTrue(ver.islocalnettree())
        self.assertTrue(ver.issemicompressed())
        T.cc = tau / (tau - 1)
        T.cp = (tau - 3) / (2 * (tau - 1))
        self.assertTrue(ver.isglobalnettree())
    
if __name__ == '__main__':
    unittest.main()