/*! \brief a simple implement of ADT queue
 *  \author Shylock Hg
 *  \date 2018-05-12
 *  \email tcath2s@gmail.com
 * */

#define NDEBUG

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

///! \defgroup queue_field
/// @{

/*
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
*/

/// @}
//
//
///! \defgroup queue_method
/// @{

/*! \brief create a queue
 *  \param size_value size(byte) of queue element value
 *  \retval created queue instance
 * */
struct queue_class * queue_class_new(size_t size_value){
	struct queue_class * instance = malloc(sizeof(struct queue_class));

	instance->_head = NULL;
	instance->_tail = NULL;
	instance->size_value = size_value;

	return instance;
}

struct queue * queue_class_element_new(const struct queue_class * instance, const void * value){
	struct queue * element = malloc(sizeof(struct queue));
	assert(NULL != element);
	if(NULL == element)
		return NULL;

	element->value = malloc(instance->size_value);
	assert(NULL != element);
	if(NULL == element)
		return NULL;
	memcpy(element->value, value, instance->size_value);

	element->_pre  = NULL;
	element->_next = NULL;

	return element;
}

/*! \brief release the whole queue
 *  \param instance instance of queue
 * */
void queue_class_release(struct queue_class * instance){
	assert(NULL != instance);

	if(NULL == instance)
		return;

	//< release queue memory
	while(NULL != instance->_head){
		struct queue * temp = instance->_head;
		instance->_head = instance->_head->_next;

		queue_class_element_release(instance, temp);
	}

	//< release handle memory
	free(instance);
}

void queue_class_element_release(const struct queue_class * instance, struct queue * element){
	assert(NULL != instance && NULL != element);

	if(NULL == instance || NULL == element)
		return ;

	free(element->value);
	free(element);
}

/*! \brief append a element to tail of queue
 *  \param instance instance of queue
 *  \param value pointer to value to enqueue
 *  \retval 0 for ok, -1 for fail
 * */
int queue_class_enqueue(struct queue_class * instance, const void * value){
	/*
	struct queue * temp = malloc(sizeof(struct queue));
	assert(NULL != temp);
	if(NULL == temp)
		return -1;

	temp->value = malloc(instance->size_value);
	assert(NULL != temp->value)
	if(NULL == temp)
		return -1;
	memcpy(temp->value, value, instance->size_value);

	temp->next = NULL;
	*/
	struct queue * element = queue_class_element_new(instance, value);
	assert(NULL != element);
	if(NULL == element)
		return -1;

	if(NULL == instance->_tail && NULL == instance->_head)
		//< for null queue
		instance->_head = instance->_tail = element;
	else if(NULL != instance->_tail){
		//< enqueue to tail
		instance->_tail->_next = element;
		element->_pre = instance->_tail;

		//< scroll tail
		instance->_tail = element;
	}

	return 0;
}
 
/*! \brief pop a element from queue head
 *  \param instance instance of queue
 *  \retval pointer to new alloc element , must manually free after use
 * */
struct queue * queue_class_dequeue(struct queue_class * instance){
	/*
	assert(NULL != instance && NULL != instance->_tail);
	if(NULL == instance || NULL == instance->_tail)
		return NULL;

	struct queue * ret = queue_class_element_new(instance, instance->_tail->value);
	assert(NULL != ret);
	if(NULL == ret)
		return NULL;

	struct queue * pre = instance->_tail->_pre;
	
	queue_class_element_release(instance, instance->_tail);

	instance->_tail = pre;
	if(NULL != instance->_tail)
		instance->_tail->_next = NULL;
	*/
	assert(NULL != instance && NULL != instance->_head);
	if(NULL == instance || NULL == instance->_head)
		return NULL;

	struct queue * ret = queue_class_element_new(instance, instance->_head->value);
	assert(NULL != ret);
	if(NULL == ret)
		return NULL;

	struct queue * next = instance->_head->_next;

	queue_class_element_release(instance, instance->_head);

	//< scroll head
	instance->_head = next;
	if(NULL != instance->_head)
		instance->_head->_pre = NULL;
	else
		instance->_tail = NULL;

	return ret;
}

/*! \brief get the element from queue head but not remove
 *  \param instance instance of queue
 *  \retval queue pointer to head
 * */
struct queue * queue_class_peek(const struct queue_class * instance){
	assert(NULL != instance && NULL != instance->_tail);
	if(NULL == instance || NULL == instance->_tail)
		return NULL;

	return instance->_head;
}

/*! \brief get the count of element in queue
 *  \param instance instance of queue
 *  \retval count of element in queue
 * */
size_t queue_class_count(const struct queue_class * instance){
	assert(NULL != instance);
	if(NULL == instance)
		return 0;

	struct queue * temp = instance->_head;
	size_t count = 0;
	while(NULL != temp){
		count++;
		temp = temp->_next;
	}

	return count;
}

/*! \brief check queue if empty
 *  \param instance instance of queue
 *  \retval bool 0 for not empty, 1 for empty
 * */
int queue_class_is_empty(const struct queue_class * instance){
	assert(NULL != instance);
	if(NULL == instance)
		return 1;

	if(NULL == instance->_head || NULL == instance->_tail)
		return 1;
	else 
		return 0;
}

//< dynamic queue not 
//int queue_class_is_full(const struct * queue_class * instance);

/// @}

