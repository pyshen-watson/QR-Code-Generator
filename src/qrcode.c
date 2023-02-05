#include "qrcode.h"
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 21
#define CHARACTER 35
#define MAXLENGTH 25
#define MAXLEN 208
#define FORCED 2

int ECC[8][15]={{0,1,1,0,1,0,1,0,1,0,1,1,1,1,1},
				{1,1,1,0,0,1,0,1,1,1,1,0,0,1,1},
				{1,1,1,1,1,0,1,1,0,1,0,1,0,1,0},
				{1,1,1,1,0,0,0,1,0,0,1,1,1,0,1},
				{1,1,0,0,1,1,0,0,0,1,0,1,1,1,1},
				{1,1,0,0,0,1,1,0,0,0,1,1,0,0,0},
				{1,1,0,1,1,0,0,0,1,0,0,0,0,0,1},
				{1,1,0,1,0,0,1,0,1,1,1,0,1,1,0}};

int datacode[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void setcolor(unsigned short ForeColor,unsigned short BackGroundColor){
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle,ForeColor+BackGroundColor*0x10);
}

void setpos(int x,int y){
    COORD pos;
	HANDLE handle;
    pos.X=x;
    pos.Y=y;
    handle=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle,pos);
}

void errorcode(char str[],int a[]){
	int b[256]={1};
	int pre=1;

	for (int i=1;i<256;i++){

		if(pre*2>255)
			b[i] = (pre*2)^285;
		else
			b[i] = pre*2;

		pre=b[i];
	}

	int datacode_a[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	for(int i=0,count=0;count<19;i+=8,count++){

		for(int j=i;j<i+8;j++)
			datacode[count] += (a[j]<<(7-j%8));

		for(int j=0;j<256;j++){
			if(b[j]==datacode[count]){
				datacode_a[count]=j;
				break;}
			}
	}

	int error_cor[8]={0,87,229,146,149,238,102,21};

	for(int i=0;i<19;i++)
	{
		int plus=datacode_a[i];
		int error_corA[8],error_corB[8];
		for(int j=0;j<8;j++){
			error_corA[j]=(error_cor[j]+plus)%255;
			error_corB[j]=b[error_corA[j]];
			}
		for(int k=i;k<i+8;k++){
			datacode[k]=(datacode[k]^error_corB[k-i]);
			for(int n=0;n<256;n++){
				if(datacode[k]==b[n]){
					datacode_a[k]=n;
					break;}
				}
			}
	}
	return ;
}

void make_list(char str[],int a[]){
	int len = strlen(str);
	for(int i=11;i>3;i--)
		a[i] = ( (len>>(11-i)) & 1 );

	for(int i=12;i<(12+len*8);i++){
		char c = str[(i-12)/8];
		a[i] = ((c & (1<<(7-(i-4)%8)))!=0);
	}

	for(int i=12+len*8;i<16+len*8;i++)
		a[i]=0;

	int error[] = {1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1};

	for(int i=16+len*8,counter=0;i<208;i++,counter++)
		a[i] = error[counter%16];

	errorcode(str,a);

	for(int i=152;i<208;i++){
		a[i] = ( (datacode[((i-152)/8+19)] & (1<<(7-i%8))) != 0? 1:0);
	}

	return;
}

void put_table(int a[],int table[][TABLE_SIZE]){
	int dx[3]={0,-1,1};
	int dy[3]={-1,1,1};
	int x = TABLE_SIZE-1;
	int y = TABLE_SIZE-1;

	for(int i=0;i<24;i++){
		table[x][y] = a[i];
		x += dx[i%2];
		y += dy[i%2];}

		x++;y-=2;
	for(int i=24;i<48;i++){
		table[x][y] = a[i];
		x += dx[(i%2)*2];
		y += dy[(i%2)*2];}

		x--;y-=2;
	for(int i=48;i<72;i++){
		table[x][y] = a[i];
		x += dx[i%2];
		y += dy[i%2];}

		x++;y-=2;
	for(int i=72;i<96;i++){
		table[x][y] = a[i];
		x += dx[(i%2)*2];
		y += dy[(i%2)*2];}

		x--;y-=2;
	for(int i=96;i<124;i++){
		table[x][y] = a[i];
		x += dx[i%2];
		y += dy[i%2];}

	x--;
	for(int i=124;i<136;i++){
		table[x][y] = a[i];
		x += dx[i%2];
		y += dy[i%2];}

		x++;y-=2;
	for(int i=136;i<148;i++){
		table[x][y] = a[i];
		x += dx[(i%2)*2];
		y += dy[(i%2)*2];}

	x++;
	for(int i=148;i<176;i++){
		table[x][y] = a[i];
		x += dx[(i%2)*2];
		y += dy[(i%2)*2];}

	x=12;y=8;
	for(int i=176;i<184;i++){
		table[x][y] = a[i];
		x += dx[i%2];
		y += dy[i%2];}

	x=9;y=5;
	for(int i=184;i<192;i++){
		table[x][y] = a[i];
		x += dx[(i%2)*2];
		y += dy[(i%2)*2];}

	x=12;y=3;
	for(int i=192;i<200;i++){
		table[x][y] = a[i];
		x += dx[i%2];
		y += dy[i%2];}

	x=9;y=1;
	for(int i=200;i<208;i++){
		table[x][y] = a[i];
		x += dx[(i%2)*2];
		y += dy[(i%2)*2];}

	return;
}

void draw_square(int table[][TABLE_SIZE],int x,int y,int side,int color){
	for(int i=0;i<side;i++)
		for(int j=0;j<side;j++)
			table[x+i][y+j] = color;
	return;
}

void layer(int table[][TABLE_SIZE],int type){

	//location code
	draw_square(table,0,0,8,0);
	draw_square(table,0,0,7,1);
	draw_square(table,1,1,5,0);
	draw_square(table,2,2,3,1);

	draw_square(table,0,13,8,0);
	draw_square(table,0,14,7,1);
	draw_square(table,1,15,5,0);
	draw_square(table,2,16,3,1);

	draw_square(table,13,0,8,0);
	draw_square(table,14,0,7,1);
	draw_square(table,15,1,5,0);
	draw_square(table,16,2,3,1);

	//time line
	table[6][8]=table[6][10]=table[6][12]=table[8][6]=table[10][6]=table[12][6]=table[13][8]=1;
	table[6][9]=table[6][11]=table[9][6]=table[11][6]=0;

	//ECC
	int order[]={0,1,2,3,4,5,7,8};
	for(int i=0;i<15;i++){
		if(i<7) table[8][order[i]] = table[20-i][8] = ECC[type][i];
		else table[order[14-i]][8] = table[8][6+i] = ECC[type][i];
	}
}

void mask(int table[][TABLE_SIZE],int type){
	int mask[8][TABLE_SIZE][TABLE_SIZE];
	for(int i=0;i<21;i++){
		for(int j=0;j<21;j++){
			mask[0][i][j]=((i+j)%2==0);
			mask[1][i][j]=(i%2==0);
			mask[2][i][j]=(j%3==0);
			mask[3][i][j]=((i+j)%3==0);
			mask[4][i][j]=((i/2+j/3)%2==0);
			mask[5][i][j]=(((i*j)%2+(i*j)%3)==0);
			mask[6][i][j]=(((i*j)%3+i*j)%2==0);
			mask[7][i][j]=(((i*j)%3+i+j)%2==0);
		}
	}

	//Xor
	for(int i=0;i<21;i++){
		for(int j=0;j<21;j++){
			if(table[i][j] == mask[type][i][j])
				table[i][j] = 0;
			else
				table[i][j] = 1;}
	}
	return;
}

void print_qrcode(char str[]){
	int table[TABLE_SIZE][TABLE_SIZE];
	int a[MAXLEN] = {0,1,0,0};

	make_list(str,a);
	put_table(a,table);

	srand(time(0));
	int mask_type = rand()%8;

	#ifdef FORCED
	mask_type = FORCED;
	#endif

	mask(table,mask_type);
	layer(table,mask_type);

	for(int i=0;i<TABLE_SIZE;i++){
		for(int j=0;j<TABLE_SIZE;j++){
			setpos(37+j*2,6+i);
			setcolor(7,table[i][j]==0? 7:0);
			printf("  ");
		}
	}
	return;
}
