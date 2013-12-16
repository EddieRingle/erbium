#include <stdio.h>

#include "include/erbium_version.h"

int main(void)
{
	printf("\n== Erbium v%d.%d.%d ==\n\n",
		   er_version_major, er_version_minor, er_version_patch);
	printf("Full version info: %s\n", er_version_full);
	return 0;
}

