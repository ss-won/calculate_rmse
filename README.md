# < 영상의 픽셀 차이의 제곱근(rmse) 계산하기 >
- YUV420 파일의 픽셀 저장 방식
![YUV420](https://upload.wikimedia.org/wikipedia/commons/thumb/0/0d/Yuv420.svg/800px-Yuv420.svg.png "YUV420파일의 픽셀 저장 방식")
- 여기서 MSE = 차이의 제곱의 합 RMSE = MSE의 루트값(제곱근)
- PSNR = Peak Signal-to-Noise Ratio 

![MSE,RMSE,PSNR 공식](https://www.pantechsolutions.net/blog/wp-content/uploads/2013/11/psnr-and-mse.png "MSE, RMSE 공식")

## 그래서 왜 RMSE 구하는데 ? 👀
<ol>1. PSNR값을 구할 수 있으니까!</ol>
    <ol>- 영상은 손실압축을 하는것이 일반적인데 그 손실량을 통해 영상 화질의 정도를 판단할 수 있고, 또 영상인코딩 개발에서는 PSNR을 기준으로 메모리와 시간의 단축을 목표로 개발을 합니다.</ol>
<ol>2. 동영상 코딩의 원리를 깨우칠 수 있다(교수님 피셜)</ol>
    <ol>- 일단 픽셀값을 불러와야 차이값을 연산할 수 있기 때문에 자연스럽게 영상 압축 및 코딩의 근본이 되는 영상 픽셀값 다루는 과정을 익힐 수 있다.</ol>

## 개발환경 & 조건

- 영상 확장자마다 저장방식이 다르겠지만 본 코딩에서는 YUV420형식의 파일을 사용한다. 
- 영상의 크기는 30프레임에, 픽셀 1280*720이다.
- 본 코딩에서는 명암(Y)값에 대해서만 RMSE를 계산한다.
- 수많은 에디터가 있겠지만 파이썬쓸거 아니면 Visual Studio를 쓸 것을 권장합니다.
- 보통의 Visual Studio에서 권장하는 최대 메모리 할당 크기는 1MB 입니다.(잘못하면 오버플로우, 참조값에러의 대환장파티가 열릴 수 있습니다.)
- 사실 영상파일을 가져와서 비교만 할 수 있다면 제약이 없습니다만, 굉장히 많은 값을 읽어오고 처리하기 때문에 왠만하면 8기가 이상의 컴퓨터에 코딩하시길 권장드립니다.
```
o/s : window10(64bits)
tool : Visual Studio 2015
```
- 정신건강을 위해 fopen이나 scanf 당시 뜨는 오류를 방지하는 아래의 코드를 전처리기에 미리 입력하도록 하자!
```c 
#define _CRT_SECURE_NO_WARNINGS
```

## EXPLANATION for CODE

(1) 파일을 열고 프레임 개수별 메모리 할당
```c 
  // rb : byte 형식의 읽기전용으로 파일 열기
  pfile = fopen("PeopleOnStreet_1280x720_30_Original.yuv", "rb");
	cfile = fopen("PeopleOnStreet_1280x720_30_Recons.yuv", "rb");
  
  //1 frame 크기, 읽어온 바이트 스트림 개수 저장 하는 변수
	size_t n_size;
  
  //해상도 크기
  int resolution_size = 1280 * 720;
  
  //프레임 개수만큼 이중포인터에 동적 메모리 할당 -> read_data[30][] 형태
	unsigned char **read_data_o = new unsigned char*[FRAME_NUM];
	unsigned char **read_data_r = new unsigned char*[FRAME_NUM];
```
(2)-1 파일 데이터를 가져와 메모리 할당
```c
  //프레임 개수만큼 반복
  for (int i = 0; i < FRAME_NUM; i++) {
    //프레임 갯수 만큼의 동적 메모리 할당
    read_data_o[i] = new unsigned char[resolution_size * 3 / 2];
		read_data_r[i] = new unsigned char[resolution_size * 3 / 2];
      
    //yuv 파일을 읽어 read_data 배열에 복사
    n_size = fread(read_data_o[i], sizeof(unsigned char), resolution_size * 3 / 2, pfile);
    fread(read_data_r[i], sizeof(unsigned char), resolution_size * 3 / 2, cfile);
      ...
      (2)-2
      (2)-3
      ...
  }
			
```
(2)-2 y값만 가져와 MSE 연산, y를 제외한 값의 연산은 모두 break 처리
```c
double result = 0;
for (int j = 0; j < resolution_size; j++) {
  if (j >= resolution_size) break; 
  // 이차원배열의 [frame][0-(resolution-1)]:y 데이터, [frame][resolution-(resolution*3/2-1)]:u&v 데이터==yuv420 파일의 특성 때문
  result += pow(read_data_o[i][j] - read_data_r[i][j], 2);
}
 ```
(2)-3 프레임마다 y값 산출하여 MSE, RMSE(=Root(MSE)) 계산
```c
  mse_s += (result / resolution_size);
  rmse_s += sqrt(result / resolution_size);
```
(3) YUV값에서 Y값을 정말 잘! 추출한것인지?를 확인하기 
- Y값에 다 0을 할당해보고(1프레임만) 다시 yuv 파일쓰기를 통해 영상이 명암 값이 존재하지 않음을 확인하면 됩니다.<br>(사진 추가 첨부 하겠음)
```c
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
```
***

**끝나면** 꼭 메모리 
반납 해주세욘 !
```c
for(int i=0;i<1280*720;i++){
 delete[] read_data_o[i];
 delete[] read_data_r[i];
}
delete[] read_data_d;
delete[] read_data_r;
```
