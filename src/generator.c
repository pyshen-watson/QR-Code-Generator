#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "qrcode.h"

int main()
{
	char str[20];

	system("COLOR 90");
	printf("Please entering a string less than 18 letters.\n");

	while(scanf("%s",str)!=EOF)
		print_qrcode(str);

	return 0;
}
