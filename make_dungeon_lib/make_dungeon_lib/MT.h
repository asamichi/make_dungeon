

void ini_rand(void);//プログラムの初めに一回読み込む。シード値の設定
int get_rand(int n, int m);
/*n~mの範囲でランダムに値を返す。０以下の数時は返さない
負の値が欲しいなら
例えば-1~1の範囲なら、1 2 3の範囲で1 3を手に入れて真ん中の 2 を引けばいい

int tmp;
tmp = 2;
while(tmp == 2){
get_rand(1,3);
}
tmp = tmp - 2;

こんな感じ
*/





/*
unsigned long genrand_int32(void);
void init_genrand(unsigned long s);

*/