#ifndef _FIFO_BUFFER_H_
#define _FIFO_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t buff_int_t;

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
    void **start_addr;

    /** Pointer to the function that should be used to allocate memory */
    void *(*fifo_malloc)(buff_int_t N);

    /** Pointer to the function that should be used to free memory */
    void *(*fifo_free)(buff_int_t N);
};

struct fifo_buffer fifo_init(buff_int_t max_size, 
                             void *(*fifo_malloc)(buff_int_t N), 
                             void *(*fifo_free)(buff_int_t N));

enum fifo_errors fifo_add_elements(struct fifo_buffer *buff, void *elements[], buff_int_t N);

void *fifo_read_elements(struct fifo_buffer *buff, buff_int_t N);

void *fifo_read_element(struct fifo_buffer *buff);

#endif // _FIFO_BUFFER_H_