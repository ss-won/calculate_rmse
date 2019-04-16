#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define _CRT_SECURE_NO_WARNINGS 
#define FRAME_NUM 30

int main()
{
	/*-----------파일을 열고 프레임 개수별 메모리 할당--------------*/
	FILE *pfile,*cfile,*fcheck = NULL;//파일 구조체 선언

	pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("PeopleOnStreet_1280x720_30_Recons.yuv", "rb");
	///파일을 이진읽기 전용으로 열기

	size_t n_size;//1 frame 크기, 읽어온 바이트 스트림 개수 저장 하는 변수
	double mse_s = 0, rmse_s = 0;
	int resolution_size = 1280 * 720;//해상도 크기
	unsigned char **read_data_o = new unsigned char*[FRAME_NUM];
	unsigned char **read_data_r = new unsigned char*[FRAME_NUM];
	//프레임 개수만큼 이중포인터에 동적 메모리 할당 -> read_data[30][] 형태
	/*-------------------------------------------------------------*/

	/*-----------파일 데이터를 가져와 메모리 할당 + 프레임마다 y값 산출하여 MSE, RMSE(=Root(MSE)), PSNR 계산--------------*/
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
			read_data_o[i] = new unsigned char[resolution_size * 3 / 2];//프레임 갯수 만큼의 동적 메모리 할당
			read_data_r[i] = new unsigned char[resolution_size * 3 / 2];

			n_size = fread(read_data_o[i], sizeof(unsigned char), resolution_size * 3 / 2, pfile);
			fread(read_data_r[i], sizeof(unsigned char), resolution_size * 3 / 2, cfile);
			//yuv 파일을 읽어 read_data 배열에 복사
           //n_size는 읽어온 바이트 스트림 개수를 의미
			printf("%d %d ", read_data_o[i][0],read_data_o[i][resolution_size-1]);
		   /* y값만 가져와 연산 y를 제외한 값의 연산은 모두 break 처리 */
			double result = 0;
			//int subtr = 0;
			for (int j = 0; j < resolution_size; j++) {
				//if (j >= resolution_size) break; 
				// 이차원배열의 [frame][0-(resolution-1)]:y 데이터, [frame][resolution-(resolution*3/2-1)]:u&v 데이터
				//==yuv420 파일의 특성 때문
				//subtr = read_data_o[i][j] - read_data_r[i][j];
				result += pow(read_data_o[i][j] - read_data_r[i][j], 2);
			}

			mse_s += (result / resolution_size);
			rmse_s += sqrt(result / resolution_size);
			printf("MSE: %.f RMSE: %f\n", (result / resolution_size),sqrt(result / resolution_size));//프레임당 MSE,RMSE 값 출력
		}

		printf("mse의 평균: %.f\nrmse의 평균: %f\n", mse_s / 30, rmse_s / 30);//30프레임 동안의 MSE,RMSE값의 평균
		double psnr = 0;
		psnr = 10*log10(255*255/(mse_s/30));//30프레임의 평균 값으로 PSNR 계산
		printf("PSNR = %f(db)", psnr);
	}
	/*--------------------------------------------------------------------------------------------------------------------*/
	/*-------------y값을 잘 산출 한것인지 확인---------------------*/
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
	    fclose(cfile);//파일 닫기
		fclose(pfile);
		fclose(fout);
    

	return 0;
}