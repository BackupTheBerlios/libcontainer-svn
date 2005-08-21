noinst_PROGRAMS = libcontainer-list libcontainer-stack \
			   libcontainer-queue libcontainer-tree
LDADD = ../src/libcontainer.la

libcontainer_list_SOURCES = libcontainer-list.c
libcontainer_list_DEPENDENCIES = ../src/list.c ../src/libcontainer.h

libcontainer_stack_SOURCES = libcontainer-stack.c
libcontainer_stack_DEPENDENCIES = ../src/stack.c ../src/libcontainer.h

libcontainer_queue_SOURCES = libcontainer-queue.c
libcontainer_queue_DEPENDENCIES = ../src/queue.c ../src/libcontainer.h

libcontainer_tree_SOURCES = libcontainer-tree.c
libcontainer_tree_DEPENDENCIES = ../src/bin_tree.c ../src/libcontainer.h