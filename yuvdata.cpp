#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define _CRT_SECURE_NO_WARNINGS 
#define FRAME_NUM 30

int main()
{
	/*파일 데이터를 가져와 메모리 할당*/
	FILE *pfile,*cfile = NULL;//파일 구조체 선언

	pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("PeopleOnStreet_1280x720_30_Recons.yuv", "rb");
	///파일을 이진읽기 전용으로 열기

	int resolution_size = 1280 * 720;//해상도 크기
	unsigned char **read_data_o = new unsigned char*[FRAME_NUM];//프레임 개수만큼의 동적 메모리 할당
	unsigned char **read_data_r = new unsigned char*[FRAME_NUM];	
	size_t n_size;//1 frame 크기, 읽어온 바이트 스트림 개수 저장 하는 변수
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
			read_data_o[i] = new unsigned char[resolution_size * 3 / 2];//프레임 갯수 만큼의 동적 메모리 할당
			read_data_r[i] = new unsigned char[resolution_size * 3 / 2];

			n_size = fread(read_data_o[i], sizeof(unsigned char), resolution_size * 3 / 2, pfile);
			fread(read_data_r[i], sizeof(unsigned char), resolution_size * 3 / 2, cfile);
			//yuv 파일을 읽어 read_data 배열에 복사
           //n_size는 읽어온 바이트 스트림 개수를 의미
				
		   /* y값만 가져와 연산 y를 제외한 값의 연산은 모두 break 처리 */
			double result = 0;
			int subtr = 0;
			for (int j = 0; j < (int)n_size; j++) {
				//UVindex 첫 인자값 확인 
				//if (j == resolution_size-1) printf("%d %d\n", read_data_o[i][j], read_data_r[i][j]);
				if (j >= resolution_size) break;
				subtr = read_data_o[i][j] - read_data_r[i][j];
				result += pow(subtr, 2);


			}
			mse_s += (result / resolution_size);
			rmse_s += sqrt((result / resolution_size));
			printf("mse: %.f rmse: %f\n", (result / resolution_size),sqrt((result / resolution_size)));
		}
	
		printf("mse의 평균: %.f\nrmse의 평균: %f\n", mse_s / 30, rmse_s / 30);
		double psnr = 0;
		psnr = 10*log10(255*255/(mse_s/30));
		printf("psnr = %f(db)", psnr);

	}
	
	    fclose(cfile);
		fclose(pfile);
		

	return 0;
}