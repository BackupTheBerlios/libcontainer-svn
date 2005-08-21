#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <../src/libcontainer.h>

void print_all(List_header *linked_list) {
  List_node *tmp_node;
  static int print_num = 0;

  printf("---------- list print #%d ------------\n", ++print_num);

  LIST_FOREACH(linked_list, tmp_node) {
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
  List_header *linked_list = list_init();
  List_node *node;

  /* append two nodes */
  list_append_node(linked_list, "b");
  list_append_node(linked_list, "c");
  print_all(linked_list);

  /* prepend a node */
  list_prepend_node(linked_list, "d");
  print_all(linked_list);

  /* sort them */
  list_bubble_sort(linked_list, compare_nodes);
  print_all(linked_list);

  /* remove a node (hopefully 'c' */
  node = list_node_by_index(linked_list, 1);
  if (node != LIST_NULL_NODE) {
	list_del_node(linked_list, node, free_data);
	print_all(linked_list);
  }

  /* free the list */
  list_destroy(linked_list, free_data);

  return (EXIT_SUCCESS);
}
