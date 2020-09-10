#include "fifo_buffer.h"
#include <stddef.h>

static buff_int_t fifo_size(const struct fifo_buffer *buff)
{
    if (buff->last_el > buff->first_el)
        return buff->last_el - buff->first_el;
    else if (buff->last_el < buff->first_el)
        return buff->max_size - (buff->first_el - buff->last_el);
    else if (buff->is_full)
        return buff->max_size;
    else
        return 0;
}

/**
 * Initialize the FIFO buffer
 *
 * :param max_size: Capacity of a buffer.
 * :param fifo_malloc: Pointer to the function that should be used to allocate memory (**malloc** is the most common)
 * :param fifo_free: Pointer to the function that should be used to free memory (**free** is the most common)
 * :return: Initialized buffer :c:type:`fifo_buffer`.
 */
struct fifo_buffer fifo_init(buff_int_t max_size, 
                             void *(*fifo_malloc)(size_t N), 
                             void *(*fifo_free)(size_t N))
{
    struct fifo_buffer buff;

    buff.max_size = max_size;
    buff.fifo_malloc = fifo_malloc;
    buff.fifo_free = fifo_free;

    buff.start_addr = fifo_malloc(max_size * sizeof(void *));
    buff.first_el = 0;
    buff.last_el = 0;
    buff.is_full = (buff.max_size == buff.last_el);

    return buff;
}

/**
 * Add elements into FIFO buffer.
 *
 * :param buff: Pointer to the buffer :c:type:`fifo_buffer`.
 * :param elements: Array of pointer that should be added to the buffer.
 * :param N: Size of the array.
 * :return: Error code according to :c:type:`fifo_errors`.
 */
enum fifo_errors fifo_add_elements(struct fifo_buffer *buff, stored_data_t elements[], buff_int_t N)
{
    if (buff->is_full)
        return FIFO_FULL;
    
    if (buff->max_size < N)
        return FIFO_TOO_SMALL;

    
    buff_int_t el_left = buff->max_size - fifo_size(buff);

    if (el_left < N)
        return FIFO_TOO_SMALL;


    for (buff_int_t i = 0; i < N; i++) {
        buff_int_t buff_i = (buff->last_el + i) % buff->max_size;

        buff->start_addr[buff_i] = elements[i];
    }
    buff->last_el = (buff->last_el + N) % buff->max_size;

    if (buff->first_el == buff->last_el)
        buff->is_full = true;

    return FIFO_OK;
}

/**
 * Read **N** elements from the buffer
 *
 * :param buff: Pointer to the buffer :c:type:`fifo_buffer`.
 * :param N: Number of elements to read.
 * :param err: Status variable. 
 *      After execution it holds error code acording to :c:type:`fifo_errors`.
 * :return: Array of pointer that was read from the buffer or **NULL** if reading failed.
 *      Array located in a **HEAP** and should be freed after using.
 */
stored_data_t *fifo_read_elements(struct fifo_buffer *buff, buff_int_t N, enum fifo_errors *err)
{
    if (fifo_size(buff) < N || N == 0) {
        *err = FIFO_NOT_ENOUGHT_ELEMENTS;
        return NULL;
    }
    
    stored_data_t *result = buff->fifo_malloc(N * sizeof(N));
    for (buff_int_t i = 0; i < N; i++) {
        buff_int_t buff_i = (buff->first_el + i) % buff->max_size;
        result[i] = buff->start_addr[buff_i];
    }

    buff->first_el = (buff->first_el + N) % buff->max_size;
    buff->is_full = 0;
    
    *err = FIFO_OK;
    return result;
}

/**
 * Read **1** element from the buffer
 *
 * :param buff: Pointer to the buffer :c:type:`fifo_buffer`.
 * :param err: Status variable. 
 *      After execution it holds error code acording to :c:type:`fifo_errors`.
 * :return: Pointer that was read from the buffer or **NULL** if reading failed.
 */
stored_data_t fifo_read_element(struct fifo_buffer *buff, enum fifo_errors *err)
{
    if (fifo_size(buff) == 0) {
        *err = FIFO_EMPTY;
        return NULL;
    }
    
    stored_data_t result = buff->start_addr[buff->first_el];
    buff_int_t buff_i = (buff->first_el + 1) % buff->max_size;
    buff->first_el = buff_i;

    buff->is_full = 0;
    *err = FIFO_OK;
    return result;
}
