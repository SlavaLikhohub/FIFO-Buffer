#include "fifo_buffer.h"
#include <stddef.h>

static buff_int_t fifo_size(const struct fifo_buffer *buff)
{
    buff_int_t el_left;
    if (buff->last_el >= buff->first_el)
        el_left = buff->last_el - buff->first_el;
    else
        el_left = buff->first_el - buff->last_el;
    
    return buff->max_size - el_left;
}

/**
 * Initialize the FIFO buffer
 *
 * :param max_size: Capacity of a buffer.
 * :param fifo_malloc: Pointer to the function that should be used to allocate memory
 * :param fifo_free: Pointer to the function that should be used to free memory
 * :return: Initialized buffer :c:type:`fifo_buffer`.
 */
struct fifo_buffer fifo_init(buff_int_t max_size, 
                             void *(*fifo_malloc)(buff_int_t N), 
                             void *(*fifo_free)(buff_int_t N))
{
    struct fifo_buffer buff;

    buff.max_size = max_size;
    buff.fifo_malloc = fifo_malloc;
    buff.fifo_free = fifo_free;

    buff.start_addr = fifo_malloc(max_size * sizeof(void *));
    buff.first_el = 0;
    buff.last_el = 0;
    buff.is_full = 0;

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
enum fifo_errors fifo_add_elements(struct fifo_buffer *buff, void *elements[], buff_int_t N)
{
    if (buff->is_full)
        return FIFO_FULL;
    
    if (buff->max_size < N)
        return FIFO_TOO_SMALL;

    
    buff_int_t el_left = buff->max_size - fifo_size(buff);

    if (el_left < N)
        return FIFO_TOO_SMALL;


    for (buff_int_t i = 0; i < N; i++) {
        buff_int_t buff_i = (buff->last_el + i + 1) % buff->max_size;

        buff->start_addr[buff_i] = elements[i];
    }
    buff->last_el = (buff->last_el + N) % buff->max_size;

    if (fifo_size(buff) == buff->max_size)
        buff->is_full = true;

    return FIFO_OK;
}

/**
 * Read **N** elements from the buffer
 *
 * :param buff: Pointer to the buffer :c:type:`fifo_buffer`.
 * :param N: Number of elements to read.
 * :return: Array of pointer that was read from the buffer or **NULL** if reading failed.
 *      Array located in a **HEAP** and should be freed after using.
 */
void *fifo_read_elements(struct fifo_buffer *buff, buff_int_t N)
{
    if (fifo_size(buff) < N || N == 0)
        return NULL;
    
    void **result = buff->fifo_malloc(N * sizeof(N));
    for (buff_int_t i = 0; i < N; i++) {
        buff_int_t buff_i = (buff->first_el + i) % buff->max_size;
        result[i] = buff->start_addr[buff_i];
    }

    buff->first_el = (buff->first_el + N) % buff->max_size;

    return result;
}

/**
 * Read **1** element from the buffer
 *
 * :param buff: Pointer to the buffer :c:type:`fifo_buffer`.
 * :return: Pointer that was read from the buffer or **NULL** if reading failed.
 */
void *fifo_read_element(struct fifo_buffer *buff)
{
    if (fifo_size(buff) == 0)
        return NULL;
    
    buff_int_t buff_i = (buff->first_el + 1) % buff->max_size;
    void *result = buff->start_addr[buff_i];
    buff->first_el = buff_i;

    return result;
}
