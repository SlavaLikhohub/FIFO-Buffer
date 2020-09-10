#ifndef _FIFO_BUFFER_H_
#define _FIFO_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef BUFF_INT_T
#define BUFF_INT_T size_t
#endif
/** 
 * Data type that used for navigating over the buffer (default size_t).
 * To redefine this define **BUFF_INT_T** while compilation:
 *
 * **-D BUFF_INT_T=<desired type>**
 */
typedef BUFF_INT_T buff_int_t;


#ifndef BUFF_STORED_T
#define BUFF_STORED_T uint32_t
#endif
/**
 * Data type that is stored by the buffer (default uint32_t).
 * To redefine this define **BUFF_STORED_TYPE** while compilation:
 *
 * **-D BUFF_STORED_TYPE=<desired type>**
 */
typedef BUFF_STORED_T stored_data_t;

/** Error codes */
enum fifo_errors 
{
    /** Ok */
    FIFO_OK = 0,
    /** Unknown error */
    FIFO_UNKNOWN_ERROR,
    /** Buffer is full */
    FIFO_FULL,
    /** Buffer is empty */
    FIFO_EMPTY,
    /** Buffer is too small */
    FIFO_TOO_SMALL,
    /** Buffer contains not enought elements */
    FIFO_NOT_ENOUGHT_ELEMENTS,
};

/**
 * FIFO buffer. Initializing should be done by :c:func:`fifo_init`.
 */
struct fifo_buffer 
{
    /** Maximum size of FIFO buffer */
    buff_int_t max_size;

    /** Index of the first element */
    buff_int_t first_el;

    /** Index of the last element */
    buff_int_t last_el;

    /** Flag that specifies if buffer is full */
    bool is_full;    

    /** Pointer to the memory where space of buffer is starting */
    stored_data_t *start_addr;

    /** Pointer to the function that should be used to allocate memory */
    void *(*fifo_malloc)(size_t N);

    /** Pointer to the function that should be used to free memory */
    void *(*fifo_free)(size_t N);
};

struct fifo_buffer fifo_init(buff_int_t max_size, 
                             void *(*fifo_malloc)(size_t N), 
                             void *(*fifo_free)(size_t N));

enum fifo_errors fifo_add_elements(struct fifo_buffer *buff, stored_data_t elements[], buff_int_t N);

stored_data_t *fifo_read_elements(struct fifo_buffer *buff, buff_int_t N);

stored_data_t fifo_read_element(struct fifo_buffer *buff);

#endif // _FIFO_BUFFER_H_
