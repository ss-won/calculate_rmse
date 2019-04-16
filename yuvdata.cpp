#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

using namespace std;

int main()
{
	FILE *pfile,*cfile,*output = NULL;//파일 구조체 선언

	pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("PeopleOnStreet_1280x720_30_Recons.yuv", "rb");
	output = fopen("output.txt", "wb");
	///파일의 binary 값 읽어오기

	int resolution_size = 1280 * 720;//해상도 크기
	unsigned char *read_data_o = new unsigned char[resolution_size* 3 / 2];
	unsigned char *read_data_r = new unsigned char[resolution_size * 3 / 2];	
	size_t n_size;//1 frame 크기

	if (pfile == NULL)
	{
		fputs("File error", stderr);
		exit(1);
	}
	else if (cfile == NULL)
	{
		fputs("File error", stderr);
		exit(2);
	}
	else {
		n_size = fread(read_data_o, sizeof(unsigned char), resolution_size * 3 / 2, pfile);
		fread(read_data_r, sizeof(unsigned char), resolution_size * 3 / 2,cfile);

		//yuv 파일을 읽어 read_data 배열에 복사
	}

	double result = 0;
	int subtr = 0;
		for (int i = 0; i < n_size; i++) {
			subtr = read_data_o[i] - read_data_r[i];
			if (i >= resolution_size) { subtr = 0; }
			result += pow(subtr, 2);
			fprintf(output,"%.f ",result);//계산확인용 출력물 만드는 코드
			//UVindex 첫 인자값 확인 if (i == resolution_size) printf("%d %d\n",read_data_o[i],read_data_r[i]);
		}
		
		printf("%.f\n", (result / resolution_size));
		printf("%f", sqrt((result / resolution_size)));
			
		fclose(output);
		fclose(cfile);
		fclose(pfile);
		

	return 0;
}