lib_LTLIBRARIES = libcontainer.la
include_HEADERS = libcontainer.h
noinst_HEADERS = extra.h

libcontainer_la_SOURCES = hash_table.c bin_tree.c queue.c \
	stack.c list.c extra.c
libcontainer_la_LDFLAGS = -version-info "${LIBLT_VERSION}"