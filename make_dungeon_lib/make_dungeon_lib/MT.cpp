#include <stdio.h>
#include "time.h"
#include "MTT.h"
#include "MT.h"
//#include "conio.h"




void ini_rand(void){
	init_genrand((unsigned)time(NULL));//SEED’l‚ğŠÔ‚É‚µ‚Ä‚éA—”‚Ì‰Šú‰»
}

int get_rand(int n, int m){
	int lib_check_rand;

	//‘å¬ŠÖŒW‚ª‚¨‚©‚µ‚¢
	if (n>m){
		printf("error(n>m) %d,%d\n", n, m);
		return(m);
	}
	//genrand_int32()‚Å‚O`2^32-1‚Ì”ÍˆÍ‚Å—”

	do{
		lib_check_rand = (genrand_int32() % m) + 1;

		//a‚Í0~n-1‚Ì”ÍˆÍ
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
