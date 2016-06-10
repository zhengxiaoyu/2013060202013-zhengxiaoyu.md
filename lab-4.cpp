#include<stdio.h>
#include<stdlib.h>
#define SIZE_B 4096 //块大小
#define SIZE_M 4   //块记录大小

struct Indeo{
	int MAX_SIZE;
	int addr[13];
};

struct Add{
	int fadd;
	int *add;
};
struct Add *p_add;
struct Indeo indeo;
int num_add;

void init(){
	int i, j, k;
	int num_m = SIZE_B / SIZE_M;
	int MAX_B = num_m + num_m*num_m + 10 + num_m*num_m*num_m;
	num_add = 3 + num_m + num_m + num_m*num_m;
	p_add = (struct Add *)malloc(sizeof(struct Add)*num_add);
	for (i = 0; i<num_add; i++){
		p_add[i].add = (int *)malloc(sizeof(int)*num_m);
	}
	indeo.MAX_SIZE = SIZE_B*MAX_B;
	for (i = 0; i<10; i++){
		indeo.addr[i] = i;
	}
	indeo.addr[10] = MAX_B;
	indeo.addr[11] = MAX_B + 1;
	indeo.addr[12] = MAX_B + num_m + 1;
	p_add[0].fadd = indeo.addr[10];
	p_add[1].fadd = indeo.addr[11];
	for (i = 0; i<num_m; i++){
		p_add[0].add[i] = 10 + i;
		p_add[1].add[i] = indeo.addr[11] + i + 1;
		p_add[i + 2].fadd = p_add[1].add[i];
		for (j = 0; j<num_m; j++){
			p_add[i + 2].add[j] = (10 + num_m) + i*num_m + j;
		}
	}
	p_add[num_m + 2].fadd = indeo.addr[12];
	for (i = 0; i<num_m; i++){
		p_add[num_m + 3 + i].add[i] = indeo.addr[12] + i + 1;
		for (j = 0; j<num_m; j++){
			p_add[num_m + 3 + j + i*num_m].fadd = p_add[num_m + 3 + i].add[i];
			for (k = 0; k<num_m; k++){
				p_add[num_m + 3 + j + i*num_m].add[k] = 9 + k + (num_m + j + i*num_m)*num_m;
			}
		}
	}
}

int main(){
	init();
	struct inode node;
    initInode(node);
    printInode(node);


	printf("请输入逻辑地址： ");
	scanf("%b",&indeo.addr);
 
    int ret = convertToPhysicalAddr(logicAddr, node, num_m);

    if (ret == 0)
    {
		printf("物理盘块编号: ");
		scanf("%b",&num_m);
		
		printf("偏移: ");
		scanf("%b",&offset);
    }

    return 0;
}
















