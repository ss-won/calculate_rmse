#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define _CRT_SECURE_NO_WARNINGS 
#define FRAME_NUM 30

int main()
{
	/*���� �����͸� ������ �޸� �Ҵ�*/
	FILE *pfile,*cfile = NULL;//���� ����ü ����

	pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("PeopleOnStreet_1280x720_30_Recons.yuv", "rb");
	///������ �����б� �������� ����

	int resolution_size = 1280 * 720;//�ػ� ũ��
	unsigned char **read_data_o = new unsigned char*[FRAME_NUM];//������ ������ŭ�� ���� �޸� �Ҵ�
	unsigned char **read_data_r = new unsigned char*[FRAME_NUM];	
	size_t n_size;//1 frame ũ��, �о�� ����Ʈ ��Ʈ�� ���� ���� �ϴ� ����
	double mse_s = 0,rmse_s = 0;

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
		for (int i = 0; i < FRAME_NUM; i++) {
			read_data_o[i] = new unsigned char[resolution_size * 3 / 2];//������ ���� ��ŭ�� ���� �޸� �Ҵ�
			read_data_r[i] = new unsigned char[resolution_size * 3 / 2];

			n_size = fread(read_data_o[i], sizeof(unsigned char), resolution_size * 3 / 2, pfile);
			fread(read_data_r[i], sizeof(unsigned char), resolution_size * 3 / 2, cfile);
			//yuv ������ �о� read_data �迭�� ����
           //n_size�� �о�� ����Ʈ ��Ʈ�� ������ �ǹ�
				
		   /* y���� ������ ���� y�� ������ ���� ������ ��� break ó�� */
			double result = 0;
			int subtr = 0;
			for (int j = 0; j < (int)n_size; j++) {
				//UVindex ù ���ڰ� Ȯ�� 
				//if (j == resolution_size-1) printf("%d %d\n", read_data_o[i][j], read_data_r[i][j]);
				if (j >= resolution_size) break;
				subtr = read_data_o[i][j] - read_data_r[i][j];
				result += pow(subtr, 2);


			}
			mse_s += (result / resolution_size);
			rmse_s += sqrt((result / resolution_size));
			printf("mse: %.f rmse: %f\n", (result / resolution_size),sqrt((result / resolution_size)));
		}
	
		printf("mse�� ���: %.f\nrmse�� ���: %f\n", mse_s / 30, rmse_s / 30);
		double psnr = 0;
		psnr = 10*log10(255*255/(mse_s/30));
		printf("psnr = %f(db)", psnr);

	}
	
	    fclose(cfile);
		fclose(pfile);
		

	return 0;
}