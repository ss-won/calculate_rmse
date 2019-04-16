#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define _CRT_SECURE_NO_WARNINGS 
#define FRAME_NUM 30

int main()
{
	/*-----------������ ���� ������ ������ �޸� �Ҵ�--------------*/
	FILE *pfile,*cfile,*fcheck = NULL;//���� ����ü ����

	pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("PeopleOnStreet_1280x720_30_Recons.yuv", "rb");
	///������ �����б� �������� ����

	size_t n_size;//1 frame ũ��, �о�� ����Ʈ ��Ʈ�� ���� ���� �ϴ� ����
	double mse_s = 0, rmse_s = 0;
	int resolution_size = 1280 * 720;//�ػ� ũ��
	unsigned char **read_data_o = new unsigned char*[FRAME_NUM];
	unsigned char **read_data_r = new unsigned char*[FRAME_NUM];
	//������ ������ŭ ���������Ϳ� ���� �޸� �Ҵ� -> read_data[30][] ����
	/*-------------------------------------------------------------*/

	/*-----------���� �����͸� ������ �޸� �Ҵ� + �����Ӹ��� y�� �����Ͽ� MSE, RMSE(=Root(MSE)), PSNR ���--------------*/
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
			printf("%d %d ", read_data_o[i][0],read_data_o[i][resolution_size-1]);
		   /* y���� ������ ���� y�� ������ ���� ������ ��� break ó�� */
			double result = 0;
			//int subtr = 0;
			for (int j = 0; j < resolution_size; j++) {
				//if (j >= resolution_size) break; 
				// �������迭�� [frame][0-(resolution-1)]:y ������, [frame][resolution-(resolution*3/2-1)]:u&v ������
				//==yuv420 ������ Ư�� ����
				//subtr = read_data_o[i][j] - read_data_r[i][j];
				result += pow(read_data_o[i][j] - read_data_r[i][j], 2);
			}

			mse_s += (result / resolution_size);
			rmse_s += sqrt(result / resolution_size);
			printf("MSE: %.f RMSE: %f\n", (result / resolution_size),sqrt(result / resolution_size));//�����Ӵ� MSE,RMSE �� ���
		}

		printf("mse�� ���: %.f\nrmse�� ���: %f\n", mse_s / 30, rmse_s / 30);//30������ ������ MSE,RMSE���� ���
		double psnr = 0;
		psnr = 10*log10(255*255/(mse_s/30));//30�������� ��� ������ PSNR ���
		printf("PSNR = %f(db)", psnr);
	}
	/*--------------------------------------------------------------------------------------------------------------------*/
	/*-------------y���� �� ���� �Ѱ����� Ȯ��---------------------*/
	FILE *fout = NULL;
	fout = fopen("output.yuv", "wb");
	if (fout == NULL) { exit(3); }
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j<int(n_size); j++) {
			if(j>=resolution_size)
			read_data_o[i][j] = 0;
		}
		fwrite(read_data_o[i], 1, resolution_size * 3 / 2, fout);
	}
	/*----------------------------------------------------------*/
	    fclose(cfile);//���� �ݱ�
		fclose(pfile);
		fclose(fout);
    

	return 0;
}