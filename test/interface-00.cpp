
#include <stdio.h>
#include <stdlib.h>

#include "interface.h"

int main (int argc, char *argv[])
{
	int i;
	std::string address;
	if (argc <= 1) {
		printf("%s usage:\n", argv[0]);
		printf("  %s interface interface...\n", argv[0]);
		exit(-1);
	}
	for (i = 1; i < argc; i++) {
		address = libssdp::interface::address(argv[i]);
		printf("%-8s: %s\n", argv[i], address.c_str());
	}
	return 0;
}
