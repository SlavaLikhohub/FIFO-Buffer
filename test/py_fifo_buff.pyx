from libc.stdint cimport *
#from libc.stdbool cimport bool
from libcpp cimport bool
from libc.stdlib cimport malloc, free
from cpython.pycapsule cimport *

cdef extern from "fifo_buffer.h":
    ctypedef uint32_t buff_int_t
    
    ctypedef uint32_t stored_data_t

    cpdef enum fifo_errors:
        FIFO_OK = 0,
        FIFO_UNKNOWN_ERROR
        FIFO_FULL
        FIFO_EMPTY
        FIFO_TOO_SMALL
        FIFO_NOT_ENOUGHT_ELEMENTS

    cdef struct fifo_buffer:
        buff_int_t max_size
        buff_int_t first_el
        buff_int_t last_el
        bool is_full
        void **start_addr
        void *(*fifo_malloc)(buff_int_t N)
        void *(*fifo_free)(buff_int_t N)

    ctypedef void *(*fifo_malloc)(size_t N)
    ctypedef void (*fifo_free)(void *)

    cdef fifo_buffer fifo_init(buff_int_t max_size, fifo_malloc mal, fifo_free fr)
    
    cdef fifo_errors fifo_add_elements(fifo_buffer *buff, stored_data_t elements[], buff_int_t N)

    cdef stored_data_t *fifo_read_elements(fifo_buffer *buff, buff_int_t N)

    cdef stored_data_t fifo_read_element(fifo_buffer *buff)


'''
Destructor for capsule with FIFO buffer
'''
cdef void fifo_buffer_free(object cap):
    cdef fifo_buffer *obj = <fifo_buffer *>PyCapsule_GetPointer(cap, PyCapsule_GetName(cap))
    free(obj)

'''
Exeption for FIFO class
'''
class FIFO_exeption(Exception):
    def __init__(self, fifo_errors c_err, *args):
        err = fifo_errors(c_err)
        super().__init__(*args, err)
        
'''
Helper function to get _obj pointer from class
'''
cdef fifo_buffer *get_obj_ptr(obj):
    return <fifo_buffer *>PyCapsule_GetPointer(obj._obj, PyCapsule_GetName(obj._obj))

'''
FIFO buffer
'''
class FIFO_Buffer_py:
    def __log(self, *msgs):
        if (self.verbouse):
            print(*msgs)

    def __init__(self, buff_int_t max_size, verbouse = 0):
        self.verbouse = verbouse
        
        cdef fifo_buffer buff = fifo_init(max_size, malloc, free)
        cdef fifo_buffer* buff_ptr = <fifo_buffer*>malloc(sizeof(fifo_buffer))
        buff_ptr[0] = buff

        self._obj = PyCapsule_New(<void *>buff_ptr, "fifo_buffer", NULL)


    def add_elements(self, elements):
        N = len(elements)
        cdef stored_data_t *c_elements = <stored_data_t *>malloc(N * sizeof(void *))
        
        self.__log("adding elements")
        for i in range(N):
            c_elements[i] = <stored_data_t>elements[i]
            self.__log(f"El {i}: ", <int>c_elements[i])
        self.__log()
        
        cdef fifo_buffer *buff = get_obj_ptr(self)
        cdef fifo_errors err = fifo_add_elements(buff, c_elements, N)

        if (err != fifo_errors.FIFO_OK):
            raise FIFO_exeption(err, "Add elements")
        
        free(c_elements)
        
    def read_elements(self, buff_int_t N):
        cdef fifo_buffer *buff = get_obj_ptr(self)
        cdef stored_data_t *result = fifo_read_elements(buff, N)

        py_arr = []
        for i in range(N):
            py_arr.append(<size_t>result[i])
        
        free(result)

        return py_arr

    def read_element(self):
        cdef fifo_buffer *buff = get_obj_ptr(self)
        cdef stored_data_t result = fifo_read_element(buff)

        return <size_t>result
    
    def print(self):
        cdef fifo_buffer *buff = get_obj_ptr(self)
        print(f"Max size: {buff.max_size}")
        print(f"Start_addr: {<size_t>buff.start_addr}") 
        print(f"First ind: {buff.first_el}")
        print(f"Last ind: {buff.last_el}")
        print(f"Is full: {buff.is_full}")
        
        elements = []
        cdef size_t data = 0
        for i in range(buff.first_el, buff.last_el):
            buff_i = i % buff.max_size
            data =  <size_t>buff.start_addr[buff_i]
            elements.append(data)
        
        print(elements)
    
def PyInit_py_fifo_buf(): 
    pass
