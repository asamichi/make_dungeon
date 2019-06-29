#include <stdio.h>
#include <conio.h>
#include "MT.h"


#define Dungeon_Len_X 34 //56部屋の横の大きさ、大部屋時に動ける範囲
#define Dungeon_Wall_X 1 //5横の不懐壁の大きさ 
#define Dungeon_Len_Y 28 //34部屋のたての大きさ
#define Dungeon_Wall_Y 1 //4縦の不懐壁の大きさ
#define Dungeon_Room_min_X 6
#define Dungeon_Room_min_Y 6  //5 部屋分割の最小のブロックの大きさ、部屋数＋αくらい必要
#define Dungeon_Room_Num_MAX 12 //8 部屋の最大数
#define Dungeon_Block_min_X 8 //偶数で
#define Dungeon_Block_min_Y 8








enum panel{ p_fix_wall, p_wall, p_split, p_room, p_root, p_entrance };

//ダンジョン本体
enum panel dungeon[Dungeon_Len_X][Dungeon_Len_Y];

//ダンジョンをブロック分けする時の区画の構造体
typedef struct{
	int ux, uy; //左上の座標
	int dx, dy; //右下の座標
	int flag;
}split_t;

typedef struct{
	int ux, uy;
	int dx, dy;
	int entrance[Dungeon_Room_Num_MAX];
}room_t;

typedef struct{
	int start;
	int end;
	int point;
	int compass;//縦なら１、横なら２
	int flag;
}split_block_t;

split_block_t split_block[Dungeon_Room_Num_MAX];
split_block_t ini_split_block = { 0, 0, 0, 0, 0 };

room_t room[Dungeon_Room_Num_MAX];
room_t ini_room = { 0, 0, 0, 0, { 0 }, };

split_t ini_block = { 0, 0, Dungeon_Len_X - 1, Dungeon_Len_Y - 1, 0 };//ブロックの一個めの初期化用
//split_t ini_block={0,0,15,15,0};
split_t block[Dungeon_Room_Num_MAX];

void make_dungeon_ini(void){
	int i, j;//for用

	for (i = 0; i<Dungeon_Len_X; i++){
		for (j = 0; j<Dungeon_Len_Y; j++){
			dungeon[i][j] = p_wall;
		}
	}
	for (i = 0; i<Dungeon_Room_Num_MAX; i++){
		block[i] = ini_block;
		room[i] = ini_room;
		split_block[i] = ini_split_block;
	}
	block[0].flag = 1;
}


int get_room_num(void){
	int i;
	int room_count = 0;
	for (i = 0; i<Dungeon_Room_Num_MAX; i++){
		if (block[i].flag == 1){
			room_count++;
		}
	}
	return(room_count);
}


// n以降のblockを後ろにずらす
void move_block(int n){
	int i;
	for (i = Dungeon_Room_Num_MAX - 1; i>n; i--){
		block[i] = block[i - 1];
	}
	/*
	for(i=0;i<Dungeon_Room_Num_MAX;i++){
	if(block[i].flag==1){
	printf("フラグ立ってる%d\n",i);
	}
	}
	*/
}


/*
返値表
-1 : ブロック最大数まで作られてる
-2 : 横幅足りない
-3 : 縦幅足りない
-4 : 存在しないブロック
-5 : 乱数で分割しなかった
0  : 正常に分割
*/
int split_dungeon(int n){
	int i, j;
	int split_way;//縦に分割か横に分割か決める
	//縦に＝１　横に＝２

	int split_line;//分割するライン

	//部屋が規定量まで作成されてたら
	if (block[Dungeon_Room_Num_MAX - 1].flag == 1){
		return(-1);
	}

	//フラグ立ってないブロックは存在しないのでアウト
	if (block[n].flag != 1){
		return(-4);
	}


	//乱数でランダムで分割させない
	if (get_rand(1, 100) <= 10 + (10 * get_room_num())){
		return(-5);
	}


	if (block[n].dx - block[n].ux >= block[n].dy - block[n].uy){
		//横の方が長い
		split_way = 1;
		//	printf("縦に線を引く(n=%d)\n",n);
	}
	else{
		//縦の方が長い
		//	printf("横に線を引く(n=%d)\n",n);
		split_way = 2;
	}




	if (split_way == 1){
		//ブロック小さすぎたら弾く
		//	printf("dx=%d,ux=%d\n",block[n].dx,block[n].ux);
		//	printf("%d \n",(block[n].dx-block[n].ux) * 2);
		if (block[n].dx - block[n].ux + 1 <= (Dungeon_Block_min_X * 2) + 1){
			//	printf("横幅足りん\n");
			return(-2);
		}
		//printf("150(%d,%d)",block[n].ux + Dungeon_Block_min_X - 1 , block[n].dx - Dungeon_Block_min_X + 1);
		//if(block[n].ux + Dungeon_Block_min_X  > block[n].dx - Dungeon_Block_min_X )while(1);
		split_line = get_rand(block[n].ux + Dungeon_Block_min_X, block[n].dx - Dungeon_Block_min_X);
		//デバック用に線って入れる
		for (i = block[n].uy; i <= block[n].dy; i++){
			for (j = block[n].ux; j <= block[n].dx; j++){
				if (j == split_line)
					dungeon[j][i] = p_split;
			}
		}

		//線を登録する
		for (i = 0; i<Dungeon_Room_Num_MAX; i++){
			if (split_block[i].flag == 0){
				split_block[i].start = block[n].uy;
				split_block[i].end = block[n].dy;
				split_block[i].compass = 1;
				split_block[i].point = split_line;
				split_block[i].flag = 1;
				break;
			}
		}




		//ブロックを後ろにずらしてから
		move_block(n);
		//ブロックを分割する
		block[n].dx = split_line - 1;
		block[n + 1].ux = split_line + 1;


	}

	if (split_way == 2){
		//ブロック小さすぎたら弾く
		if (block[n].dy - block[n].uy + 1 <= (Dungeon_Block_min_Y * 2) + 1){
			//	printf("縦幅足りん\n");
			return(-3);
		}
		//	printf("190 (n=%d,uy=%d,dy=%d\n",n,block[n].uy,block[n].dy);
		//	printf("190 (%d,%d)\n",block[n].uy + Dungeon_Block_min_Y - 1 , block[n].dy - Dungeon_Block_min_Y + 1);
		//if(block[n].uy + Dungeon_Block_min_Y  > block[n].dy - Dungeon_Block_min_Y )while(1);
		split_line = get_rand(block[n].uy + Dungeon_Block_min_Y, block[n].dy - Dungeon_Block_min_Y);

		//デバック用に線って入れる
		for (i = block[n].uy; i <= block[n].dy; i++){
			for (j = block[n].ux; j <= block[n].dx; j++){
				if (i == split_line)
					dungeon[j][i] = p_split;
			}
		}
		//線を登録する
		for (i = 0; i<Dungeon_Room_Num_MAX; i++){
			if (split_block[i].flag == 0){
				split_block[i].start = block[n].ux;
				split_block[i].end = block[n].dx;
				split_block[i].compass = 2;
				split_block[i].point = split_line;
				split_block[i].flag = 1;
				break;
			}
		}


		//ブロックを後ろにずらしてから
		move_block(n);
		//ブロックを分割する
		block[n].dy = split_line - 1;
		block[n + 1].uy = split_line + 1;
	}

	return(0);
}

void split_2_dungeon(void){
	int i;
	block[0].dx = (Dungeon_Len_X / 2) - 2;
	block[0].dy = Dungeon_Len_Y - 1;
	block[1].ux = (Dungeon_Len_X / 2) + 2;
	block[1].uy = 0;
	block[1].dx = Dungeon_Len_X - 1;
	block[1].dy = Dungeon_Len_Y - 1;

	for (i = 0; i<2; i++)
		block[i].flag = 1;
}











int make_room_dungeon(void){
	int i, j, k;
	//	int rux, ruy, rdx, rdy;
	int room_num;
	room_num = get_room_num();




	for (i = 0; i<Dungeon_Room_Num_MAX; i++){

		//部屋になるべきではないブロックを弾く
		//if(block[i].dx - block[i].ux > Dungeon_Room_min_X && block[i].dy - block[i].uy > Dungeon_Room_min_Y&& 
		if (block[i].flag == 1){
			//	printf("i=%d\n",i);
			//	printf("ux %d,dx %d,uy %d,dy %d\n",block[i].ux,block[i].dx,block[i].uy,block[i].dy);
			//部屋の左上と右下確定
			//	printf("B %d,%d\n",block[i].ux + 1 ,block[i].dx - Dungeon_Room_min_X-1);
			room[i].ux = get_rand(block[i].ux + 1, block[i].dx - Dungeon_Room_min_X);

			//	printf("A %d,%d\n",room[i].ux + Dungeon_Room_min_X ,block[i].dx - 1);
			room[i].dx = get_rand(room[i].ux + Dungeon_Room_min_X - 1, block[i].dx - 1);

			room[i].uy = get_rand(block[i].uy + 1, block[i].dy - Dungeon_Room_min_Y);
			room[i].dy = get_rand(room[i].uy + Dungeon_Room_min_Y - 1, block[i].dy - 1);
			//	printf("ux %d,dx %d,uy %d,dy %d\n",room[i].ux,room[i].dx,room[i].uy,room[i].dy);
			for (j = room[i].ux; j <= room[i].dx; j++){
				for (k = room[i].uy; k <= room[i].dy; k++){
					dungeon[j][k] = p_room;
				}
			}
		}
	}
	return(0);

}


/*
int make_root_dungeon(int root_switch, int compass ,int i);
root_switch
0 :道は作らない
1 :道も作る
compass
2 :↓
4 : ←
6 : →
8 : ↑
i
何番目のブロックに対して行うのか

返値
その方角にブロックがあった　１
　なかった　０
 */
int make_root_dungeon(int root_switch, int compass, int i){
	int j, k, m;
	int entrance_num;
	int flag;
	int find = 0;

	if (compass == 2){
		//↓方向に探索

		flag = 0;
		//ブロックすべてのｘ座標から下に探索する
		for (j = room[i].ux; j <= room[i].dx; j++){
			for (k = room[i].dy; k<Dungeon_Len_Y; k++){
				if (dungeon[j][k] == p_split){
					find = 1;
					if (flag == 0 && root_switch == 1){
						entrance_num = get_rand(room[i].ux, room[i].dx);
						dungeon[entrance_num][room[i].dy] = p_entrance;
						for (m = room[i].dy + 1; dungeon[entrance_num][m] != p_split; m++){
							dungeon[entrance_num][m] = p_root;
						}
						flag = 1;
					}
				}
			}
		}
	}



	if (compass == 8){
		//↑

		flag = 0;
		//ブロックすべてのｘ座標から上に探索する
		for (j = room[i].ux; j <= room[i].dx; j++){
			//ｙ座標に関して端っこまで捜査
			for (k = room[i].uy; k != 0; k--){
				//もし分割線があれば
				if (dungeon[j][k] == p_split){
					find = 1;//ブロックあったよ～
					//もしまだ入り口作ってなくて道作る予定なら
					if (flag == 0 && root_switch == 1){
						entrance_num = get_rand(room[i].ux, room[i].dx);
						dungeon[entrance_num][room[i].uy] = p_entrance;
						for (m = room[i].uy - 1; dungeon[entrance_num][m] != p_split; m--){
							dungeon[entrance_num][m] = p_root;
						}
						flag = 1;
					}
				}
			}
		}
	}


	if (compass == 6){
		//→

		flag = 0;
		//ブロックすべてのy座標から右に探索する
		for (j = room[i].uy; j <= room[i].dy; j++){
			for (k = room[i].dx; k != Dungeon_Len_X; k++){
				if (dungeon[k][j] == p_split){
					find = 1;
					if (flag == 0 && root_switch == 1){
						entrance_num = get_rand(room[i].uy, room[i].dy);
						dungeon[room[i].dx][entrance_num] = p_entrance;
						for (m = room[i].dx + 1; dungeon[m][entrance_num] != p_split; m++){
							dungeon[m][entrance_num] = p_root;
						}
						flag = 1;
					}
				}
			}
		}
	}


	if (compass == 4){
		//←

		flag = 0;
		//ブロックすべてのy座標から左に探索する
		for (j = room[i].uy; j <= room[i].dy; j++){
			for (k = room[i].ux; k != 0; k--){
				if (dungeon[k][j] == p_split){
					find = 1;
					if (flag == 0 && root_switch == 1){
						entrance_num = get_rand(room[i].uy, room[i].dy);
						dungeon[room[i].ux][entrance_num] = p_entrance;
						for (m = room[i].ux - 1; dungeon[m][entrance_num] != p_split; m--){
							dungeon[m][entrance_num] = p_root;
						}
						flag = 1;
					}
				}
			}
		}

	}
	return(find);
}

void make_root_dungeon_normal(void){
	int i, j;
	for (i = 0; i<get_room_num(); i++){
		for (j = 2; j <= 8; j += 2){
			make_root_dungeon(1, j, i);
		}
	}
}

void root_connect(void){
	int i, j;
	int root_cnt;
	int root_flag;
	int tmp;

	for (i = 0; i<Dungeon_Room_Num_MAX; i++){
		root_cnt = 0;
		root_flag = -1;
		//ラインが存在すれば
		if (split_block[i].flag == 1){
			//		printf("s=%d,e=%d,p=%d\n",split_block[i].start,split_block[i].end,split_block[i].point);
			//縦線なら
			if (split_block[i].compass == 1){

				//線に対していくつ道が来てるか数える
				for (j = split_block[i].start; j <= split_block[i].end; j++){
					//→
					if (dungeon[split_block[i].point + 1][j] == p_root){
						root_cnt++;
					}
					//←
					if (dungeon[split_block[i].point - 1][j] == p_root){
						root_cnt++;
					}
				}
				//道が偶数個つながってたら
				if (root_cnt % 2 == 0){
					for (j = split_block[i].start; j <= split_block[i].end; j++){
						if (root_flag == 1){
							dungeon[split_block[i].point][j] = p_root;
						}
						if (dungeon[split_block[i].point + 1][j] == p_root){
							root_flag = root_flag * -1;
						}
						if (root_flag == 1){
							dungeon[split_block[i].point][j] = p_root;
						}
						if (dungeon[split_block[i].point - 1][j] == p_root){
							root_flag = root_flag * -1;
						}
						if (root_flag == 1){
							dungeon[split_block[i].point][j] = p_root;
						}
					}
				}
				//道が奇数個
				else if (root_cnt == 3){
					root_flag = 0;
					for (j = split_block[i].start; j <= split_block[i].end; j++){
						if (root_flag >0 && root_flag != 3){
							dungeon[split_block[i].point][j] = p_root;
						}
						if (dungeon[split_block[i].point + 1][j] == p_root){
							root_flag++;
						}
						if (root_flag >0 && root_flag != 3){
							dungeon[split_block[i].point][j] = p_root;
						}
						if (dungeon[split_block[i].point - 1][j] == p_root){
							root_flag++;
						}
						if (root_flag >0 && root_flag != 3){
							dungeon[split_block[i].point][j] = p_root;
						}
					}
				}
				else {
					root_flag = 0;
					tmp = 2;
					while (tmp == 2){
						tmp = get_rand(1, 3);
					}
					tmp = tmp - 2;
					//	printf("tmp=%d\n",tmp);
					//	scanf("%d",&i);
					for (j = split_block[i].start; j <= split_block[i].end; j++){
						if (root_flag >0 && root_flag != (root_cnt - tmp) / 2 && root_flag <root_cnt){
							dungeon[split_block[i].point][j] = p_root;
						}
						if (dungeon[split_block[i].point + 1][j] == p_root){
							root_flag++;
						}
						if (root_flag >0 && root_flag != (root_cnt - tmp) / 2 && root_flag <root_cnt){
							dungeon[split_block[i].point][j] = p_root;
						}
						if (dungeon[split_block[i].point - 1][j] == p_root){
							root_flag++;
						}
						if (root_flag >0 && root_flag != (root_cnt - tmp) / 2 && root_flag <root_cnt){
							dungeon[split_block[i].point][j] = p_root;
						}
					}
				}
			}
			//横線
			if (split_block[i].compass == 2){
				//線に対していくつ道が来てるか数える
				for (j = split_block[i].start; j <= split_block[i].end; j++){
					//→
					if (dungeon[j][split_block[i].point + 1] == p_root){
						root_cnt++;
					}
					//←
					if (dungeon[j][split_block[i].point - 1] == p_root){
						root_cnt++;
					}
				}
				//道が偶数個つながってたら
				if (root_cnt % 2 == 0){
					for (j = split_block[i].start; j <= split_block[i].end; j++){
						if (root_flag == 1){
							dungeon[j][split_block[i].point] = p_root;
						}
						if (dungeon[j][split_block[i].point + 1] == p_root){
							root_flag = root_flag * -1;
						}
						if (root_flag == 1){
							dungeon[j][split_block[i].point] = p_root;
						}
						if (dungeon[j][split_block[i].point - 1] == p_root){
							root_flag = root_flag * -1;
						}
						if (root_flag == 1){
							dungeon[j][split_block[i].point] = p_root;
						}
					}
				}
				//道が奇数個
				else if (root_cnt == 3){
					root_flag = 0;
					for (j = split_block[i].start; j <= split_block[i].end; j++){
						if (root_flag >0 && root_flag != 3){
							dungeon[j][split_block[i].point] = p_root;
						}
						if (dungeon[j][split_block[i].point + 1] == p_root){
							root_flag++;
						}
						if (root_flag >0 && root_flag != 3){
							dungeon[j][split_block[i].point] = p_root;
						}
						if (dungeon[j][split_block[i].point - 1] == p_root){
							root_flag++;
						}
						if (root_flag >0 && root_flag != 3){
							dungeon[j][split_block[i].point] = p_root;
						}
					}
				}
				else {
					root_flag = 0;
					tmp = 2;
					while (tmp == 2){
						tmp = get_rand(1, 3);
					}
					tmp = tmp - 2;
					//	printf("tmp=%d\n",tmp);
					//	scanf("%d",&i);
					for (j = split_block[i].start; j <= split_block[i].end; j++){
						if (root_flag >0 && root_flag != (root_cnt - tmp) / 2 && root_flag < root_cnt){
							dungeon[j][split_block[i].point] = p_root;
						}
						if (dungeon[j][split_block[i].point + 1] == p_root){
							root_flag++;
						}
						if (root_flag >0 && root_flag != (root_cnt - tmp) / 2 && root_flag < root_cnt){
							dungeon[j][split_block[i].point] = p_root;
						}
						if (dungeon[j][split_block[i].point - 1] == p_root){
							root_flag++;
						}
						if (root_flag >0 && root_flag != (root_cnt - tmp) / 2 && root_flag < root_cnt){
							dungeon[j][split_block[i].point] = p_root;
						}
					}
				}
			}
		}
	}
}



void print_dungeon(void){
	int i, j;

	//上の不懐壁
	for (j = 0; j<Dungeon_Wall_Y; j++){
		for (i = 0; i<Dungeon_Len_X + (2 * Dungeon_Wall_X); i++){
			printf("岩");
		}
		printf("\n");
	}

	//部屋
	for (i = 0; i<Dungeon_Len_Y; i++){
		//横の不懐壁
		for (j = 0; j<Dungeon_Wall_X; j++){
			printf("岩");
		}
		for (j = 0; j<Dungeon_Len_X; j++){
			switch (dungeon[j][i]){
			case p_fix_wall:  	printf("岩");
			case p_wall:   		printf("■"); break;
			case p_room:   		printf("□"); break;
			case p_split: 		printf("■"); break;
			case p_entrance:	printf("□"); break;
			case p_root:		printf("◇"); break;
			default: printf("error ij = %d/n", dungeon[j][i]); break;
			}
		}
		//横の不懐壁
		for (j = 0; j<Dungeon_Wall_X; j++){
			printf("岩");
		}
		printf("\n");
	}

	//下の不懐壁
	for (j = 0; j<Dungeon_Wall_Y; j++){
		for (i = 0; i<Dungeon_Len_X + (2 * Dungeon_Wall_X); i++){
			printf("岩");
		}
		printf("\n");
	}

}



int make_dungeon(void){
	int a = 0;
	int i, j;
	int debug;
	int split_cnt;
	int n;


	ini_rand();
	while (1){

		//printf("%d\n",get_rand(0,10));

		make_dungeon_ini();
		//split_2_dungeon();

		for (i = 0; i<Dungeon_Room_Num_MAX; i++){
			//	split_cnt = 1;
			//	while( (debug = split_dungeon(i)) == 0){
			while (1){
				debug = split_dungeon(i);

				//			switch(debug){
				//			case 0:printf("正常に分割\n");break;
				//			case -1 :printf("ブロックが最大数できてる\n");break;
				//			case -2 :printf("横幅足りない\n");break;
				//			case -3 :printf("縦幅足りない\n");break;
				//			case -4 :printf("存在しないブロック\n");break;
				//			case -5 :printf("乱数で分割中止\n");break;
				//			}

				if (debug != 0){
					//	printf("debug=%d\n",debug);
					break;
				}
			}
		}


		//		printf("完成\n");
		make_room_dungeon();
		make_root_dungeon_normal();
		root_connect();
		print_dungeon();
		//printf("部屋の数は%dです\n",get_room_num());
			_getch();


	}
	return(0);
}
