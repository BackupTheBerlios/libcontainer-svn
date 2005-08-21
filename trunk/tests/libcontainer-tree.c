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

int compare_nodes(Tree_data *data1, Tree_data *data2) {
  return (strcmp((char *)data1, (char *)data2));
}

void free_data(Tree_data *data) {
  /* free something here */
  printf("I would normally use a free() here: %s\n", (char *)data);
}

int main(int argc, char *argv[]) {
  int i;
  Tree_header *tree = tree_init(compare_nodes);
  Tree_node *node;
  
  for (i = 1; i < argc; i++)
	tree_add_node(tree, argv[i]);

  node = tree_find_by_data(tree, "g", compare_nodes);
  if (node != TREE_NULL_NODE)
	tree_del_node(tree, node, free_data);

  tree_destroy(tree, free_data, TREE_GET_ROOT_NODE(tree));

  return (EXIT_SUCCESS);
}
