#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

using namespace std;

int main()
{
	FILE *pfile,*cfile,*output = NULL;//���� ����ü ����

	pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("PeopleOnStreet_1280x720_30_Recons.yuv", "rb");
	output = fopen("output.txt", "wb");
	///������ binary �� �о����

	int resolution_size = 1280 * 720;//�ػ� ũ��
	unsigned char *read_data_o = new unsigned char[resolution_size* 3 / 2];
	unsigned char *read_data_r = new unsigned char[resolution_size * 3 / 2];	
	size_t n_size;//1 frame ũ��

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

		//yuv ������ �о� read_data �迭�� ����
	}

	double result = 0;
	int subtr = 0;
		for (int i = 0; i < n_size; i++) {
			subtr = read_data_o[i] - read_data_r[i];
			if (i >= resolution_size) { subtr = 0; }
			result += pow(subtr, 2);
			fprintf(output,"%.f ",result);//���Ȯ�ο� ��¹� ����� �ڵ�
			//UVindex ù ���ڰ� Ȯ�� if (i == resolution_size) printf("%d %d\n",read_data_o[i],read_data_r[i]);
		}
		
		printf("%.f\n", (result / resolution_size));
		printf("%f", sqrt((result / resolution_size)));
			
		fclose(output);
		fclose(cfile);
		fclose(pfile);
		

	return 0;
}