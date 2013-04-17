
target-y = \
	interface-00 \
	interface-01 \
	socket-00

define test-defaults
	$1_files-y = \
		$(addsuffix .cpp, $1) \
		../src/libssdp.o

	$1_includes-y = \
		../src
	
	$1_ldflags-y = \
		-lpthread
	
ifeq ($(uname_S), Linux)
	$1_ldflags-y += \
		-lrt
endif
endef

$(eval $(foreach T,$(target-y), $(eval $(call test-defaults,$T))))

include ../Makefile.lib
