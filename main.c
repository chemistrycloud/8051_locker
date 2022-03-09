#include <REGX52.H>
#include "Delay.h"
#include "IR.h"
#include "Delay.c"
#include "IR.c"
#include"timer0.h"
#include"timer0.c"
#include"Int0.h"
#include"Int0.c"
#include"MatrixKey.c"
#include"MatrixKey.h"
#define passlen 8  //���볤��

sbit machine=P3^7;
sbit led = P2^0;   //��״ָ̬ʾ��
sbit info=P2^1;	   //����������״ָ̬ʾ��
sbit error=P2^2;   //�������ָʾ��
sbit KeyPress=P2^3;   //����ָʾ��
unsigned char Num;
unsigned char Address;
unsigned char Command;

unsigned char KeyNum;
int length=0;
int Time;
KeyValue[16] = {1,2,3,10,4,5,6,11,7,8,9,12,15,0,16,13};		 //������̵İ�����Ӧ��ʶֵ
unsigned char password[16]={1,9,1,8,0,2,0,8,-1,-1,-1,-1,-1,-1,-1,1};	//��ʼ����
//unsigned char input[passlen];	 	//���������������飨����д��������Խ�縲���ڴ�BUG��											 0
unsigned char input[16];			 //����������������

void Delay1000us()		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}


void Delay2000us()		//@12.000MHz
{
	unsigned char i, j;

	i = 4;
	j = 225;
	do
	{
		while (--j);
	} while (--i);
}




/*����ʱ�����ļ�����뺯��*/
int CheckPassword(){   
	int flag=1,i;
	for(i=0;i<16;i++)
	{
		if(input[i]!=password[i])
		{
			if(i<passlen-1)
				{
					flag=0;	  
				}
			else{
					1==1;	  //ȷ��ʱ�临�Ӷ�һ��
			 	}
		}
	}
	if(length!=passlen)
		{
			return 0;
		}
		else
		{
			return flag;
		}
}

/*����*/
void LockON(){
		unsigned char i;
		for(i=0;i<10;i++)
		{
			machine=1;
			Delay2000us();
			//Delay(2);
			machine=0;
		}
		led=1;	
}

/*����*/
void LockOFF(){
		unsigned char i;
		for(i=0;i<10;i++)
		{
			machine=1;
			Delay1000us();
			//Delay(1.5);
			machine=0;
		}
		led=0; 
}

/*���뿪��*/
void PasswordOpen(){
   		KeyNum=MatrixKey();	
		if(KeyNum){	
			KeyPress=1;
			Delay(100);																											  
			KeyPress=0;	
			if(KeyValue[KeyNum-1]>=0&&KeyValue[KeyNum-1]<=9&&length<16)						
			{

				input[length]=KeyValue[KeyNum-1];
				length++;
			}
			if(KeyNum==15)	 //����#��ȷ��
			{
				if(CheckPassword())
				{
					info = 0;
					LockON();
					Delay(3000);
					LockOFF();
					info=1;
				}
						
				else
				{
				info=0;
				error = 1;
				Delay(2000);
				error = 0;
				info=1;
				}				
				length=0;				
			}
			if(KeyNum==13&&length>0)  //����*������
			{
				length--;

			}
		}
}




void IrOpen(){

	if(IR_GetDataFlag() || IR_GetRepeatFlag())	
		{
			info = 0;
			Address=IR_GetAddress();		
			Command=IR_GetCommand();						
			if(Command==0x16)		
			{
				LockON();
				Command=0;					
			}
			if(Command==0x0d)			
			{
				LockOFF();	
				Command=0;					

			}
			//Delay(2000);
			info = 1;
		}
		
}

void main()
{	
	
	led=0;
	info=0;
	error=0;
	KeyPress=0;
	machine=0;
	IR_Init();
	LockOFF();
	info = 1; //�����������ָʾ����
	while(1)
	{
		PasswordOpen();		 //���뿪��
		IrOpen();			 //���⿪��
	}
}

