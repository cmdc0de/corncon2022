#include <iostream>
#include <cstdlib>
#include <cstdio>

int main(int arc, char *argv[]) {
	char *fileName = argv[1];
	int color = atoi(argv[2]);	

	FILE *f = fopen(fileName,"wb");
	if(f) {
		fwrite(&color,sizeof(color),1, f);
		fclose(f);
	}
	return 0;

}
