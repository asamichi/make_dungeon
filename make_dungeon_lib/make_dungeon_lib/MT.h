

void ini_rand(void);//�v���O�����̏��߂Ɉ��ǂݍ��ށB�V�[�h�l�̐ݒ�
int get_rand(int n, int m);
/*n~m�͈̔͂Ń����_���ɒl��Ԃ��B�O�ȉ��̐����͕Ԃ��Ȃ�
���̒l���~�����Ȃ�
�Ⴆ��-1~1�͈̔͂Ȃ�A1 2 3�͈̔͂�1 3����ɓ���Đ^�񒆂� 2 �������΂���

int tmp;
tmp = 2;
while(tmp == 2){
get_rand(1,3);
}
tmp = tmp - 2;

����Ȋ���
*/





/*
unsigned long genrand_int32(void);
void init_genrand(unsigned long s);

*/