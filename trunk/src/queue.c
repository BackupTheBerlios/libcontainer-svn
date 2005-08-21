#include <stdio.h>
#include <stdlib.h>

#include <libcontainer.h>

Queue_header *queue_init(void) {
  Queue_header *header;

  /* create the actual header */
  if (!(header = (Queue_header *)malloc(sizeof(Queue_header))))
	return ((Queue_header *)0);

  /* init the internal list */
  if (!(header->list = list_init()))
  	return ((Queue_header *)0);

  /* pointer points the the head first (null basically) */
  header->cur_pointer = header->list->head;

  return (header);
}

List_node *queue_enqueue(Queue_header *header, List_data *data) {
  header->cur_pointer = header->list->head;

  return (list_insert_node(header->list, data,
						   header->list->count));
}

List_node *queue_dequeue(Queue_header *header) {
  List_node *tmp_node = header->cur_pointer;

  /* shift the current pointer to the right */
  header->cur_pointer = header->cur_pointer->next;

  return (tmp_node);
}

void queue_destroy(Queue_header *header, List_data_free_func free_func) {
  list_destroy(header->list, free_func);
  free(header);
}
