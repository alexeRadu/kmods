#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *fp;
	int i = 10;
	size_t ret;

	fp = fopen("/dev/sbulla", "w+");
	if (!fp) {
		perror("error opening file");
		return 1;
	}

	printf("opened file\n");

	ret = fwrite(&i, sizeof(int), 1, fp);
	printf("written: %lu\n", ret);
	
	fclose(fp);

	return 0;
}
