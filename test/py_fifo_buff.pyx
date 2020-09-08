from libc.stdint cimport *
#from libc.stdbool cimport bool
from libcpp cimport bool
from libc.stdlib cimport malloc, free
from cpython.pycapsule cimport *

cdef extern from "fifo_buffer.h":
    ctypedef uint32_t buff_int_t

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
    
    cdef fifo_errors fifo_add_elements(fifo_buffer *buff, void *elements[], buff_int_t N)

    cdef void *fifo_read_elements(fifo_buffer *buff, buff_int_t N)

    cdef void *fifo_read_element(fifo_buffer *buff)


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
    def __init__(self, buff_int_t max_size):
        cdef fifo_buffer buff = fifo_init(max_size, malloc, free)
        cdef fifo_buffer* buff_ptr = <fifo_buffer*>malloc(sizeof(fifo_buffer))
        buff_ptr[0] = buff

        self._obj = PyCapsule_New(<void *>buff_ptr, "fifo_buffer", NULL)

    def add_elements(self, elements):
        N = len(elements)
        cdef void **c_elements = <void **>malloc(N * sizeof(void *))
        
        for i in range(N):
            c_elements[i] = <void *>elements[i]
        
        cdef fifo_buffer *buff = get_obj_ptr(self)
        cdef fifo_errors err = fifo_add_elements(buff, c_elements, N)

        if (err != fifo_errors.FIFO_OK):
            raise FIFO_exeption(err, "Add elements")
        
        free(c_elements)
        
    def read_elements(self, buff_int_t N):
        cdef fifo_buffer *buff = get_obj_ptr(self)
        cdef void *result = fifo_read_elements(buff, N)

        py_arr = []
        for i in range(N):
            py_arr.append(result[i])
        
        free(result)

        return py_arr

    
def PyInit_py_fifo_buf(): 
    pass