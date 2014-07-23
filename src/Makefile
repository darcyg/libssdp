
target.o-y = \
	libssdp.o

target.so-y = \
	libssdp.so

libssdp.o_files-y = \
	ssdp.cpp

ifeq ($(uname_S), Linux)
subdir-y += \
		posix

libssdp.o_files-y = \
		posix/libposix.o
endif

ifeq ($(uname_S), Darwin)
subdir-y += \
		posix

libssdp.o_files-y = \
		posix/libposix.o
endif

libssdp.so_files-y = \
	libssdp.o

libssdp.so_ldflags-y = \
	-lstdc++

include ../Makefile.lib
