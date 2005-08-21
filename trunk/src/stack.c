#include <stdio.h>
#include <stdlib.h>

#include <libcontainer.h>

Stack_header *stack_init(void) {
  Stack_header *header;

  /* create the actual header */
  if (!(header = (Stack_header *)malloc(sizeof(Stack_header))))
	return ((Stack_header *)0);

  /* init the internal list */
  if (!(header->list = list_init()))
  	return ((Stack_header *)0);

  /* pointer points the the head first (null basically) */
  header->cur_pointer = header->list->head;

  return (header);
}

List_node *stack_push(Stack_header *header, List_data *data) {
  header->cur_pointer = list_insert_node(header->list, data, 0);

  return (header->cur_pointer);
}

List_node *stack_pop(Stack_header *header) {
  List_node *tmp_node = header->cur_pointer;

  /* shift the current pointer to the right */
  header->cur_pointer = header->cur_pointer->next;

  return (tmp_node);
}

void stack_destroy(Stack_header *header, List_data_free_func free_func) {
  list_destroy(header->list, free_func);
  free(header);
}
