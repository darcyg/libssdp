
target.o-y = \
	libssdp.o

target.so-y = \
	libssdp.so

libssdp.o_files-y = \
	ssdp.cpp

ifeq ($(uname_S), Linux)
libssdp.o_files-y += \
	interface-posix.cpp \
	socket-posix.cpp \
	thread-posix.cpp
endif

ifeq ($(uname_S), Darwin)
libssdp.o_files-y += \
	interface-posix.cpp \
	socket-posix.cpp \
	thread-posix.cpp
endif

libssdp.so_files-y = \
	libssdp.o

libssdp.so_ldflags-y = \
	-lstdc++

include ../Makefile.lib
