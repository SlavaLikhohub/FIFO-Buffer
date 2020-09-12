import unittest

import py_fifo_buff as F

class TestFifoBuffer(unittest.TestCase):
    def testInit(self):
        max_size = 50
        buff = F.FIFO_Buffer_py(max_size)
        self.assertTrue(1)
         
    def testAdd(self):
        max_size = 50
        ell = [1, 3, 54]
        buff = F.FIFO_Buffer_py(max_size)
        buff.add_elements(ell)
        self.assertTrue(1)
    
    def testAddRead(self):
        max_size = 50
        ell = [1, 3, 54]
        buff = F.FIFO_Buffer_py(max_size)
        buff.add_elements(ell)
        ell_r = buff.read_elements(len(ell))
        self.assertEqual(ell_r, ell)
    
    def testAddReadOneElement(self):
        max_size = 50
        ell = [1]
        buff = F.FIFO_Buffer_py(max_size)
        buff.add_elements(ell)
        ell_r = []
        ell_r.append(buff.read_element())
        self.assertEqual(ell_r, ell)
     
    def testIsFull(self):
        max_size = 50
        ell = list(range(max_size))
        buff = F.FIFO_Buffer_py(max_size)
        buff.add_elements(ell)
        with self.assertRaises(F.FIFO_exeption):
            buff.add_elements([1])
    
    def testIsFullAfterRead(self):
        max_size = 50
        ell = list(range(max_size))
        buff = F.FIFO_Buffer_py(max_size)
        buff.add_elements(ell)
        buff.read_elements(20)
        with self.assertRaises(F.FIFO_exeption):
            buff.add_elements(list(range(30)))
    
    
    def testNotFull(self):
        max_size = 50
        ell = list(range(max_size))
        buff = F.FIFO_Buffer_py(max_size)
        buff.add_elements(ell)

        read = 20
        ell_r = buff.read_elements(read)
        
        ell = list(range(read))
        buff.add_elements(ell)
        self.assertTrue(1)
    
    def testReadFromEmpty(self):
        max_size = 50
        buff = F.FIFO_Buffer_py(max_size)
        buff.add_elements([1])
        buff.read_element()
        with self.assertRaises(Exception):
            buff.read_element()
    
    def testBigData(self):
        max_size = int(1e4) # Tested with 1e8 with uint32_t
        ell = list(range(max_size))
        buff = F.FIFO_Buffer_py(max_size)
        buff.add_elements(ell)
        
        ell_r = buff.read_elements(max_size)
        self.assertEqual(ell_r, ell)
        
if __name__ == "__main__":
    unittest.main()


