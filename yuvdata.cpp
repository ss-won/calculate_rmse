#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main()
{
	FILE *pfile = NULL;//���� ����ü ����
	FILE *cfile = NULL;

	pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("output.txt", "wb");
	///������ binary �� �о����

	int resolution_size = 1280 * 720;//�ػ� ũ��
	unsigned char *read_data = new unsigned char[resolution_size*3/2];
	
	size_t n_size;

	if (pfile == NULL)
	{
		fputs("File error", stderr);
		exit(1);
	}
	
	
		fread(read_data, sizeof(unsigned char), resolution_size * 3 / 2, pfile);
		//yuv ������ �о� read_data �迭�� ����
		//n_size = ���� ����Ʈ ��Ʈ���� ����
		
			//if (i > resolution_size)
				//read_data[i] = 0;
		fwrite(read_data, 1, 100, cfile);
			

		fclose(pfile);
		fclose(cfile);

	return 0;
}