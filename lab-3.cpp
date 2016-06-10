// Page Storage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct pageTable 
{
	int logic_num;
	int phy_num;
}pageTable;

int main(int argc, char* argv[])
{
	int page_record_size=4,page_size=4,page_table_step=3;
	int page_size_bit = 0;//页内地址所占位数
	int page_num_bit = 0;//页号所占位数
	char logic_addr[16]={0X0,0X0,0X0,0X0,0X0,0X0,0X0,0X0,0X0,0X1,0X1,0X1,0Xa,0Xa,0X0,0Xa};
	int c_logic_addr[64]={0};//存储64位2进制逻辑地址
	int all_page = 0;//总页数，存放的是2的次方
	int page_dev = 0;//页内偏移
	int page_num = 0;
	int page_table_num;//最高级页表项数目(均为2的次方数)
	int record_one_page;//每页中页记录数
	/*******
	64位系统
	页记录大小（如 4Byte）
	页大小（如 4KB）本程序为方便以下计算取值为2的整数次方
	页表级数（如，2表示2级页表，n表示n级页表）
	逻辑地址（十六进制）
	*******/
	printf("please input message\n");

	printf("页表项大小page_record_size(Byte):");
	scanf("%d",&page_record_size);

	printf("页大小page_size(KB):");
	scanf("%d",&page_size);

	printf("页表级数page_table_step:");
	scanf("%d",&page_table_step);

	//printf("逻辑地址logic_addr(16进制):");
	//scanf("%s",&logic_addr);
	//写入物理内存
	FILE *fp = fopen("phy.txt","w");
	srand(time(0));
	for(unsigned int i=0; i<pow(2,16); ++i)
	{
		fprintf(fp,"%d %d\n",i,rand()%(64/page_size));
	}
	fclose(fp);

/**/

	for(int i=0; 1; ++i)//求出总页数,页表项数,每页页表项数
	{
		if(pow(2,i) == page_size)
		{
			page_size_bit = i + 10;//页内地址所占位数
			page_num_bit = 64 - page_size_bit;//页号地址所占位数
			all_page = 64 - 10 - i;//总页数(均为2的次方数)
			page_table_num = all_page - 10 -i + 2;//最高级页表项数目(均为2的次方数)，这里默认页记录大小为4Byte
			record_one_page = i + 8;//记录每页页记录数（记录的是2的次方数，即页表页内偏移所占位数）
			break;
		}
		if(pow(2,i) > page_size)
		{
			printf("请输入2的整数次方为页大小\n");
			printf("page_size(KB):");
			scanf("%d",&page_size);
			i = -1;
		}
	}
	
	int k = 0;
	for(int j=0; j!=16; ++j)//将16进制逻辑地址转换为64位2进制,并存入c_logic_addr[]中
	{   
		int n = logic_addr[j];
		int a[4];
		for (i = 0; i != 4; ++i)
		{
			a[4 - 1 - i] = n % 2;
			n /= 2;
		}
		for (i = 0; i != 4; ++i)
		{
			
			c_logic_addr[k] = a[i];
			printf("%d",c_logic_addr[k]);
			++k;
		}
		printf("  ");
	}
	//pageTable page_table[]
	pageTable page_table[100];//存放第一级页表（从page_table.txt文件中读取）

	pageTable phy[65536];//存放物理内存（从phy.txt文件中读取）
	FILE *table = fopen("page_table.txt","r");
	for(j=0; j<20; ++j)//这里页表项数取一个小值20
	{

			fscanf(table,"%d",&page_table[j].logic_num);//!=EOF
			fscanf(table,"%d",&page_table[j].phy_num);
	}
	fclose(table);
	FILE *phyTxt = fopen("phy.txt","r");
	for(j=0; j<65536; ++j)
	{
		fscanf(phyTxt,"%d",&phy[j].logic_num);//!=EOF
		fscanf(phyTxt,"%d",&phy[j].phy_num);
	}
	fclose(phyTxt);

	if(page_table_step == 1)
	{
		for(i = page_num_bit-1; i>=0; --i) 
			page_num += c_logic_addr[i] * (int)pow(2,page_num_bit-1 - i); //计算逻辑页号
		for(i = 0; i<page_size_bit; ++i)
		{
			page_dev += c_logic_addr[63 - i] * (int)pow(2,i);//计算页内偏移
		}
		printf("\n物理页号:%d\n",page_table[page_num].phy_num);//输出逻辑页号对应的物理页号
		printf("页内偏移:%d\n",page_dev);
		//printf("%d    %d",page_size_bit,page_num_bit);
	}
	/**
	if(page_table_step == 2)
	{
		int page1_num=0,page2_num=0;//计算一级页表,二级页表页内偏移
		for(i = 0; i<page_size_bit; ++i)
		{
			page_dev += c_logic_addr[63 - i] * (int)pow(2,i);//计算页内偏移
		}
		//page_num += c_logic_addr[i] * (int)pow(2,i-page_num_bit+1);//计算页号
		
		for(i = page_num_bit-record_one_page -1; i>=0; --i)
		{
				page1_num += c_logic_addr[i] * (int)pow(2,page_num_bit-record_one_page-1 - i); //计算一级页表对应的编号，page_num_bit -  record_one_page一级页表所占位数
		}
			//得到page_table[page1_num].phy_num存放二级页表的页框号
		for(i = page_num_bit - record_one_page; i<64 - page_size_bit; ++i)
			page2_num += c_logic_addr[i] * (int)pow(2,(64-page_size_bit) - i -1);//计算二级页表页内偏移
		int addr = page_record_size*page2_num + page_table[page1_num].phy_num * (int)pow(2,page_size_bit);//二级页表页内偏移*页表项大小+2级对应页框首址
		printf("\n一级页表偏移：%d\n",page1_num);
		printf("二级页表地址：%d\n",addr);
		printf("二级页表页框号：%d\n",page_table[page1_num].phy_num);
		printf("二级页表偏移：%d\n",page2_num);
		printf("物理页号：%d\n",phy[addr].phy_num);
		printf("页内偏移：%d\n",page_dev);
	}
	**/
	if(page_table_step >= 2)
	{
		int page1_num=0,page2_num=0;//计算一级页表,二级页表页内偏移
		int addr = 0;
		for(i = 0; i<page_size_bit; ++i)
		{
			page_dev += c_logic_addr[63 - i] * (int)pow(2,i);//计算页内偏移
		}
		//page_num += c_logic_addr[i] * (int)pow(2,i-page_num_bit+1);//计算页号
		

		for(i = page_num_bit - (page_table_step-1)*record_one_page -1; i>=0; --i)//(page_size_step-1)*record_one_page
		{
			page1_num += c_logic_addr[i] * (int)pow(2,page_num_bit-(page_table_step-1)*record_one_page-1 - i); //计算一级页表对应的编号，page_num_bit -  record_one_page一级页表所占位数
		}
				//得到page_table[page1_num].phy_num存放二级页表的页框号
		for(i = page_num_bit - (page_table_step-1)*record_one_page; i<64 - page_size_bit - (page_table_step-2)*record_one_page; ++i)
			page2_num += c_logic_addr[i] * (int)pow(2,(64-page_size_bit- (page_table_step-2)*record_one_page) - i -1);//计算二级页表C偏移
		addr = page_record_size*page2_num + page_table[page1_num].phy_num * (int)pow(2,page_size_bit);//二级页表页内偏移*页表项大小+2级对应页框首址
		printf("\n一级页表页内偏移：%d\n",page1_num);
		
		printf("\n2级页表页框号：%d\n",page_table[page1_num].phy_num);
		printf("2级页表页内偏移：%d\n",page2_num);
		printf("2级页表对应物理地址(页内偏移*记录表项大小后的值)：%d\n",addr);
		for(int n=page_table_step; n>2; --n)//循环查找
		{
			phy[addr].phy_num;//上级对应下级页表的页框
			printf("\n%d级页表页框号：%d\n",page_table_step+3-n,phy[addr].phy_num);
			for(i = page_num_bit - (n-2)*record_one_page; i<page_num_bit - (n-3)*record_one_page; ++i)
			{
				
				page2_num += c_logic_addr[i] * (int)pow(2,(page_num_bit - (n-3)*record_one_page) - i -1);//计算页表页内偏移
			}
			addr = page_record_size*page2_num + phy[addr].phy_num * (int)pow(2,page_size_bit);//页表页内偏移*页表项大小+对应页框首址
			
			printf("%d级页表页内偏移：%d\n",page_table_step+3-n,page2_num);
			printf("%d级页表对应物理地址(页内偏移*记录表项大小后的值)：%d\n",page_table_step+3-n,addr);
		}
		printf("物理页号：%d\n",phy[addr].phy_num);
		printf("页内偏移：%d\n",page_dev);
	}
	/************/
	return 0;
}
