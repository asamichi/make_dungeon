#include <stdio.h>
#include "time.h"
#include "MTT.h"
#include "MT.h"
//#include "conio.h"




void ini_rand(void){
	init_genrand((unsigned)time(NULL));//SEED値を時間にしてる、乱数の初期化
}

int get_rand(int n, int m){
	int lib_check_rand;

	//大小関係がおかしい
	if (n>m){
		printf("error(n>m) %d,%d\n", n, m);
		return(m);
	}
	//genrand_int32()で０〜2^32-1の範囲で乱数

	do{
		lib_check_rand = (genrand_int32() % m) + 1;

		//aは0~n-1の範囲
	} while (lib_check_rand < n || lib_check_rand == -12345);

	return(lib_check_rand);
}
/*
int main(void){
int i;
ini_rand();
while(1){
printf("%d\n",get_rand(10,10));


getch();
}


return(0);
}
*/
