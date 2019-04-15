#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main()
{
	FILE *pfile = NULL;//파일 구조체 선언
	FILE *cfile = NULL;

	pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("output.txt", "wb");
	///파일의 binary 값 읽어오기

	int resolution_size = 1280 * 720;//해상도 크기
	unsigned char *read_data = new unsigned char[resolution_size*3/2];
	
	size_t n_size;

	if (pfile == NULL)
	{
		fputs("File error", stderr);
		exit(1);
	}
	
	
		fread(read_data, sizeof(unsigned char), resolution_size * 3 / 2, pfile);
		//yuv 파일을 읽어 read_data 배열에 복사
		//n_size = 읽은 바이트 스트림의 개수
		
			//if (i > resolution_size)
				//read_data[i] = 0;
		fwrite(read_data, 1, 100, cfile);
			

		fclose(pfile);
		fclose(cfile);

	return 0;
}