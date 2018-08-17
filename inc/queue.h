/*! \brief a simple implement of ADT queue(dynamic)
 *  \author Shylock Hg
 *  \date 2018-05-12
 *  \email tcath2s@gmail.com
 * */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <stddef.h>

///! \defgroup queue_field
/// @{

typedef struct queue {
	void * value;
	struct queue * _pre;
	struct queue * _next;
} queue_t;

typedef struct queue_class {
	struct queue * _head;  //!< current head
	struct queue * _tail;  //!< current tail
	size_t size_value;
} queue_class_t;

///  nil <-  ele <--> ele <--> ele <--> ele -> nil

/// @}
//
//
///! \defgroup queue_method
/// @{

/*! \brief create a queue
 *  \param size_value size(byte) of queue element value
 *  \retval created queue instance , NULL for fail
 * */
struct queue_class * queue_class_new(size_t size_value);

/*! \brief create a queue element
 *  \param instance instance of queue
 *  \param value value of element
 *  \retval pointer to created element , NULL for fail
 * */
struct queue * queue_class_element_new(const struct queue_class * instance, const void * value);

/*! \brief release the whole queue
 *  \param instance instance of queue
 * */
void queue_class_release(struct queue_class * instance);

/*! \brief release a element of queue
 *  \param instance instance of queue
 *  \param element pointer to element to release
 * */
void queue_class_element_release(const struct queue_class * instance, struct queue * element);

/*! \brief append a element to tail of queue
 *  \param instance instance of queue
 *  \param value pointer to value to enqueue
 *  \retval 0 for ok, -1 for fail
 * */
int queue_class_enqueue(struct queue_class * instance, const void * value);
 
/*! \brief pop a element from queue head
 *  \param instance instance of queue
 *  \retval pointer to new alloc element , must manually free after use
 *          NULL for empty queue
 * */
struct queue * queue_class_dequeue(struct queue_class * instance);

/*! \brief get the element from queue head but not remove
 *  \param instance instance of queue
 *  \retval queue pointer to head , NULL for empty queue
 * */
struct queue * queue_class_peek(const struct queue_class * instance);

/*! \brief get the count of element in queue
 *  \param instance instance of queue
 *  \retval count of element in queue
 * */
size_t queue_class_count(const struct queue_class * instance);

/*! \brief check queue if empty
 *  \param instance instance of queue
 *  \retval bool 0 for not empty, 1 for empty
 * */
int queue_class_is_empty(const struct queue_class * instance);

//< dynamic queue not 
//int queue_class_is_full(const struct queue_class * instance);

/// @}

#ifdef __cplusplus
	}
#endif

#endif  //!< _QUEUE_H_

