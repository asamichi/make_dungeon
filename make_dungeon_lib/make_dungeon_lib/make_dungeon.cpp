#include <stdio.h>
#include <conio.h>
#include "MT.h"


#define Dungeon_Len_X 34 //56�����̉��̑傫���A�啔�����ɓ�����͈�
#define Dungeon_Wall_X 1 //5���̕s���ǂ̑傫�� 
#define Dungeon_Len_Y 28 //34�����̂��Ă̑傫��
#define Dungeon_Wall_Y 1 //4�c�̕s���ǂ̑傫��
#define Dungeon_Room_min_X 6
#define Dungeon_Room_min_Y 6  //5 ���������̍ŏ��̃u���b�N�̑傫���A�������{�����炢�K�v
#define Dungeon_Room_Num_MAX 12 //8 �����̍ő吔
#define Dungeon_Block_min_X 8 //������
#define Dungeon_Block_min_Y 8








enum panel{ p_fix_wall, p_wall, p_split, p_room, p_root, p_entrance };

//�_���W�����{��
enum panel dungeon[Dungeon_Len_X][Dungeon_Len_Y];

//�_���W�������u���b�N�������鎞�̋��̍\����
typedef struct{
	int ux, uy; //����̍��W
	int dx, dy; //�E���̍��W
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
	int compass;//�c�Ȃ�P�A���Ȃ�Q
	int flag;
}split_block_t;

split_block_t split_block[Dungeon_Room_Num_MAX];
split_block_t ini_split_block = { 0, 0, 0, 0, 0 };

room_t room[Dungeon_Room_Num_MAX];
room_t ini_room = { 0, 0, 0, 0, { 0 }, };

split_t ini_block = { 0, 0, Dungeon_Len_X - 1, Dungeon_Len_Y - 1, 0 };//�u���b�N�̈�߂̏������p
//split_t ini_block={0,0,15,15,0};
split_t block[Dungeon_Room_Num_MAX];

void make_dungeon_ini(void){
	int i, j;//for�p

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


// n�ȍ~��block�����ɂ��炷
void move_block(int n){
	int i;
	for (i = Dungeon_Room_Num_MAX - 1; i>n; i--){
		block[i] = block[i - 1];
	}
	/*
	for(i=0;i<Dungeon_Room_Num_MAX;i++){
	if(block[i].flag==1){
	printf("�t���O�����Ă�%d\n",i);
	}
	}
	*/
}


/*
�Ԓl�\
-1 : �u���b�N�ő吔�܂ō���Ă�
-2 : ��������Ȃ�
-3 : �c������Ȃ�
-4 : ���݂��Ȃ��u���b�N
-5 : �����ŕ������Ȃ�����
0  : ����ɕ���
*/
int split_dungeon(int n){
	int i, j;
	int split_way;//�c�ɕ��������ɕ��������߂�
	//�c�Ɂ��P�@���Ɂ��Q

	int split_line;//�������郉�C��

	//�������K��ʂ܂ō쐬����Ă���
	if (block[Dungeon_Room_Num_MAX - 1].flag == 1){
		return(-1);
	}

	//�t���O�����ĂȂ��u���b�N�͑��݂��Ȃ��̂ŃA�E�g
	if (block[n].flag != 1){
		return(-4);
	}


	//�����Ń����_���ŕ��������Ȃ�
	if (get_rand(1, 100) <= 10 + (10 * get_room_num())){
		return(-5);
	}


	if (block[n].dx - block[n].ux >= block[n].dy - block[n].uy){
		//���̕�������
		split_way = 1;
		//	printf("�c�ɐ�������(n=%d)\n",n);
	}
	else{
		//�c�̕�������
		//	printf("���ɐ�������(n=%d)\n",n);
		split_way = 2;
	}




	if (split_way == 1){
		//�u���b�N������������e��
		//	printf("dx=%d,ux=%d\n",block[n].dx,block[n].ux);
		//	printf("%d \n",(block[n].dx-block[n].ux) * 2);
		if (block[n].dx - block[n].ux + 1 <= (Dungeon_Block_min_X * 2) + 1){
			//	printf("���������\n");
			return(-2);
		}
		//printf("150(%d,%d)",block[n].ux + Dungeon_Block_min_X - 1 , block[n].dx - Dungeon_Block_min_X + 1);
		//if(block[n].ux + Dungeon_Block_min_X  > block[n].dx - Dungeon_Block_min_X )while(1);
		split_line = get_rand(block[n].ux + Dungeon_Block_min_X, block[n].dx - Dungeon_Block_min_X);
		//�f�o�b�N�p�ɐ����ē����
		for (i = block[n].uy; i <= block[n].dy; i++){
			for (j = block[n].ux; j <= block[n].dx; j++){
				if (j == split_line)
					dungeon[j][i] = p_split;
			}
		}

		//����o�^����
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




		//�u���b�N�����ɂ��炵�Ă���
		move_block(n);
		//�u���b�N�𕪊�����
		block[n].dx = split_line - 1;
		block[n + 1].ux = split_line + 1;


	}

	if (split_way == 2){
		//�u���b�N������������e��
		if (block[n].dy - block[n].uy + 1 <= (Dungeon_Block_min_Y * 2) + 1){
			//	printf("�c�������\n");
			return(-3);
		}
		//	printf("190 (n=%d,uy=%d,dy=%d\n",n,block[n].uy,block[n].dy);
		//	printf("190 (%d,%d)\n",block[n].uy + Dungeon_Block_min_Y - 1 , block[n].dy - Dungeon_Block_min_Y + 1);
		//if(block[n].uy + Dungeon_Block_min_Y  > block[n].dy - Dungeon_Block_min_Y )while(1);
		split_line = get_rand(block[n].uy + Dungeon_Block_min_Y, block[n].dy - Dungeon_Block_min_Y);

		//�f�o�b�N�p�ɐ����ē����
		for (i = block[n].uy; i <= block[n].dy; i++){
			for (j = block[n].ux; j <= block[n].dx; j++){
				if (i == split_line)
					dungeon[j][i] = p_split;
			}
		}
		//����o�^����
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


		//�u���b�N�����ɂ��炵�Ă���
		move_block(n);
		//�u���b�N�𕪊�����
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

		//�����ɂȂ�ׂ��ł͂Ȃ��u���b�N��e��
		//if(block[i].dx - block[i].ux > Dungeon_Room_min_X && block[i].dy - block[i].uy > Dungeon_Room_min_Y&& 
		if (block[i].flag == 1){
			//	printf("i=%d\n",i);
			//	printf("ux %d,dx %d,uy %d,dy %d\n",block[i].ux,block[i].dx,block[i].uy,block[i].dy);
			//�����̍���ƉE���m��
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
0 :���͍��Ȃ�
1 :�������
compass
2 :��
4 : ��
6 : ��
8 : ��
i
���Ԗڂ̃u���b�N�ɑ΂��čs���̂�

�Ԓl
���̕��p�Ƀu���b�N���������@�P
�@�Ȃ������@�O
 */
int make_root_dungeon(int root_switch, int compass, int i){
	int j, k, m;
	int entrance_num;
	int flag;
	int find = 0;

	if (compass == 2){
		//�������ɒT��

		flag = 0;
		//�u���b�N���ׂĂ̂����W���牺�ɒT������
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
		//��

		flag = 0;
		//�u���b�N���ׂĂ̂����W�����ɒT������
		for (j = room[i].ux; j <= room[i].dx; j++){
			//�����W�Ɋւ��Ē[�����܂ő{��
			for (k = room[i].uy; k != 0; k--){
				//�����������������
				if (dungeon[j][k] == p_split){
					find = 1;//�u���b�N��������`
					//�����܂����������ĂȂ��ē����\��Ȃ�
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
		//��

		flag = 0;
		//�u���b�N���ׂĂ�y���W����E�ɒT������
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
		//��

		flag = 0;
		//�u���b�N���ׂĂ�y���W���獶�ɒT������
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
		//���C�������݂����
		if (split_block[i].flag == 1){
			//		printf("s=%d,e=%d,p=%d\n",split_block[i].start,split_block[i].end,split_block[i].point);
			//�c���Ȃ�
			if (split_block[i].compass == 1){

				//���ɑ΂��Ă����������Ă邩������
				for (j = split_block[i].start; j <= split_block[i].end; j++){
					//��
					if (dungeon[split_block[i].point + 1][j] == p_root){
						root_cnt++;
					}
					//��
					if (dungeon[split_block[i].point - 1][j] == p_root){
						root_cnt++;
					}
				}
				//���������Ȃ����Ă���
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
				//�������
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
			//����
			if (split_block[i].compass == 2){
				//���ɑ΂��Ă����������Ă邩������
				for (j = split_block[i].start; j <= split_block[i].end; j++){
					//��
					if (dungeon[j][split_block[i].point + 1] == p_root){
						root_cnt++;
					}
					//��
					if (dungeon[j][split_block[i].point - 1] == p_root){
						root_cnt++;
					}
				}
				//���������Ȃ����Ă���
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
				//�������
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

	//��̕s����
	for (j = 0; j<Dungeon_Wall_Y; j++){
		for (i = 0; i<Dungeon_Len_X + (2 * Dungeon_Wall_X); i++){
			printf("��");
		}
		printf("\n");
	}

	//����
	for (i = 0; i<Dungeon_Len_Y; i++){
		//���̕s����
		for (j = 0; j<Dungeon_Wall_X; j++){
			printf("��");
		}
		for (j = 0; j<Dungeon_Len_X; j++){
			switch (dungeon[j][i]){
			case p_fix_wall:  	printf("��");
			case p_wall:   		printf("��"); break;
			case p_room:   		printf("��"); break;
			case p_split: 		printf("��"); break;
			case p_entrance:	printf("��"); break;
			case p_root:		printf("��"); break;
			default: printf("error ij = %d/n", dungeon[j][i]); break;
			}
		}
		//���̕s����
		for (j = 0; j<Dungeon_Wall_X; j++){
			printf("��");
		}
		printf("\n");
	}

	//���̕s����
	for (j = 0; j<Dungeon_Wall_Y; j++){
		for (i = 0; i<Dungeon_Len_X + (2 * Dungeon_Wall_X); i++){
			printf("��");
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
				//			case 0:printf("����ɕ���\n");break;
				//			case -1 :printf("�u���b�N���ő吔�ł��Ă�\n");break;
				//			case -2 :printf("��������Ȃ�\n");break;
				//			case -3 :printf("�c������Ȃ�\n");break;
				//			case -4 :printf("���݂��Ȃ��u���b�N\n");break;
				//			case -5 :printf("�����ŕ������~\n");break;
				//			}

				if (debug != 0){
					//	printf("debug=%d\n",debug);
					break;
				}
			}
		}


		//		printf("����\n");
		make_room_dungeon();
		make_root_dungeon_normal();
		root_connect();
		print_dungeon();
		//printf("�����̐���%d�ł�\n",get_room_num());
			_getch();


	}
	return(0);
}
