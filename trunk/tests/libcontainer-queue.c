#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <../src/libcontainer.h>

void print_all(Queue_header *queue) {
  List_node *tmp_node;
  static int print_num = 0;

  printf("---------- list print #%d ------------\n", ++print_num);

  QUEUE_FOREACH(queue, tmp_node) {
	puts(LIST_GET_DATA_FROM_NODE(tmp_node));
  }
}

int compare_nodes(List_data *data1, List_data *data2) {
  return (strcmp((char *)data1, (char *)data2));
}

void free_data(List_data *data) {
  /* free something here */
  printf("I would normally use a free() here: %s\n", (char *)data);
}

int main(void) {
  Queue_header *queue = queue_init();
  List_node *node;

  queue_enqueue(queue, "node1");
  queue_enqueue(queue, "node2");
  queue_enqueue(queue, "node3");

  print_all(queue);

  /* poping an item does _not_ remove it from the memory */
  node = queue_dequeue(queue);
  list_del_node(QUEUE_EXTRACT_LIST(queue), node, free_data);

  /* pop items _without_ removing them (they're still in the list) */
  queue_dequeue(queue);

  print_all(queue);

  queue_destroy(queue, free_data);

  return (EXIT_SUCCESS);
}
