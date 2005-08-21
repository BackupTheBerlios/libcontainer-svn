#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <../src/libcontainer.h>

void print_all(Stack_header *stack) {
  List_node *tmp_node;
  static int print_num = 0;

  printf("---------- list print #%d ------------\n", ++print_num);

  STACK_FOREACH(stack, tmp_node) {
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
  Stack_header *stack = stack_init();
  List_node *node;

  stack_push(stack, "node1");
  stack_push(stack, "node2");
  stack_push(stack, "node3");

  print_all(stack);

  /* poping an item does _not_ remove it from the memory */
  node = stack_pop(stack);
  list_del_node(STACK_EXTRACT_LIST(stack), node, free_data);

  /* pop items _without_ removing them (they're still in the list) */
  stack_pop(stack);

  print_all(stack);

  stack_destroy(stack, free_data);

  return (EXIT_SUCCESS);
}
