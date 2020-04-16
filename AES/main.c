
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef unsigned char uint8;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int m=8;
int p=27;
int mask;//=pow(2,8)-1;
int bt[2];
int pt[2];
int mt[256][256];
uint8 *s_box;
uint8 *Iv_s_box;
uint8 state[4][4]; 
int a0=2, a1=1, a2=1, a3=3;
int s0, s1, s2, s3, s4, s5, s6, s7;
int b0, b1, b2, b3;   
int w0, w1;
int r0, r1, r2, r3; 
int m0, m1, m2, m3, m4, m5, m6, m7, m8;
int t, t2;

//-- using lookup table------------------
int GFM(int , int );

//----using Horner rule -------------------
//int GFM(int , int );

int GFM1(int , int );
void mtable();
int inverse(int );
void S_B();
void SubByte();
void Shiftrow();
void M16A12();
void M10A17();
void M5A18();
void MixColumns();
void exkey(uint8* , unsigned int* );
void AddRoundKey(int , unsigned int* );
void show();
void AES_Encrypt(uint8* );

void InvShiftRows();
void Iv_S_B();
void InvSubBytes();
void Iv_M16A12();
void Iv_M10A17();
void  Iv_M5A18();
void InvMixColumns();
void AES_decrypt(uint8* );



int main(int argc, char** argv){
	mask=pow(2,8)-1;
	srand(time(NULL));
	clock_t Start, End;
    mtable();  // multiplier table---------------		

	S_B();
	Iv_S_B();	
	//-------exkey-----------
	
    uint8 key[16]={0x2b, 0x7e, 0x15, 0x16,
                   0x28, 0xae, 0xd2, 0xa6,
                   0xab, 0xf7, 0x15, 0x88,
	               0x09, 0xcf, 0x4f, 0x3c};

	uint8 message[4][4]={0x32, 0x88, 0x31, 0xe0,
					  	 0x43, 0x5a, 0x31, 0x37,
					  	 0xf6, 0x30, 0x98, 0x07,
					  	 0xa8, 0x8d, 0xa2, 0x34};
	
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++)
			state[i][j] = message[i][j];
	}
		
	printf("---To show-input data---\n"); 
	show();

	AES_Encrypt(key);
	printf("---Encrypt---\n");
	show();
	
	AES_decrypt(key);
	printf("---Decrypt---\n");
	show();	
	
	return 0;	
}

//-- using lookup table------------------
int GFM(int a, int b){
	int c;
	c=mt[a][b];
	return c;	
}


//----using Horner rule -------------------
/*int GFM(int a, int b){

	int c=0; bt[0]=0; bt[1]=b; pt[0]=0; pt[1]=27, mask=0xFF;
	for(int i=m-1;i>=1;i--){
	 	c=c^(bt[(a>>i)&0x01]);
	 	c=((c<<1)&mask)^pt[c>>(m-1)];
	} 
	c=c^bt[(a&0x01)]; 	
	return c;	
}*/

int GFM1(int a, int b){
	
	int c=0; bt[0]=0; bt[1]=b; pt[0]=0; pt[1]=27, mask=0xFF;
	for(int i=m-1;i>=1;i--){
	 	c=c^(bt[(a>>i)&0x01]);
	 	c=((c<<1)&mask)^pt[c>>(m-1)];
	} 
	c=c^bt[(a&0x01)]; 	
	
	return c;	
}

void mtable(){
 for(int i=0;i<256;i++)	
	for(int j=0;j<256;j++)
	    mt[i][j]=GFM1(i,j);
}

int inverse(int a){
    int a2,c1;
	a2=GFM(a,a);
	c1=1;
    for(int i=1;i<m;i++){
	    c1=GFM(c1,a2);	
	    a2=GFM(a2,a2);  	    
    }      
 return c1;
} 

void S_B(){
	
	int b,s;
	int b0,b1,b2,b3,b4,b5,b6,b7;
	int s0,s1,s2,s3,s4,s5,s6,s7;
	
	s_box = calloc(256, sizeof(uint8));

    for(int i=0;i<256;i++){
		b=inverse(i);
		b0=b&0x01;
		b1=(b>>1)&0x01;
		b2=(b>>2)&0x01;
		b3=(b>>3)&0x01;
		b4=(b>>4)&0x01;
		b5=(b>>5)&0x01;
		b6=(b>>6)&0x01;
		b7=(b>>7)&0x01; 	
		s0=b0^b4^b5^b6^b7^1;
		s1=b0^b1^b5^b6^b7^1;
		s2=b0^b1^b2^b6^b7;
		s3=b0^b1^b2^b3^b7;
		s4=b0^b1^b2^b3^b4;
		s5=b1^b2^b3^b4^b5^1;
		s6=b2^b3^b4^b5^b6^1; 
		s7=b3^b4^b5^b6^b7;
		s=s0^(s1<<1)^(s2<<2)^(s3<<3)^(s4<<4)^(s5<<5)^(s6<<6)^(s7<<7);
		s_box[i]=s;
	}
//	for(int i=0;i<256;i++){
//		printf("%.2X ", s_box[i]); 
//		if((i+1)%16==0)
//			printf("\n"); 	
//	}
}

void Iv_S_B(){
	Iv_s_box = calloc(256, sizeof(uint8));
	for(int i=0;i<256;i++){
		Iv_s_box[s_box[i]] = i;	
	}
//	printf("\n"); 	
//	for(int i=0; i<256; i++){
//		printf("%.2X ", Iv_s_box[i]); 
//		if((i+1)%16==0)
//			printf("\n"); 			
//	}
} 

void SubByte()
{
//------DATA CONVERT BY S-BOX for subbytes 
 for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
       	state[i][j]=s_box[state[i][j]];
}

void Shiftrow(){
    int temp;
    temp=state[1][0];  
    state[1][0]=state[1][1];  
    state[1][1]=state[1][2];  
    state[1][2]=state[1][3];  
    state[1][3]=temp;  

    temp=state[2][0];  
    state[2][0]=state[2][2];  
    state[2][2]=temp;  
    temp=state[2][1];  
    state[2][1]=state[2][3];
    state[2][3]=temp;

    temp=state[3][3];  
    state[3][3]=state[3][2];  
    state[3][2]=state[3][1];  
    state[3][1]=state[3][0];  
    state[3][0]=temp;  	
}

void M16A12(){
   a0=2; a1=3; a2=1; a3=1;
   int b0, b1, b2, b3;	
   for(int i=0;i<4;i++){
		b0=state[0][i];
		b1=state[1][i];       	
		b2=state[2][i];       	
		b3=state[3][i];       	       	  
		state[0][i]=GFM(a0,b0) ^ GFM(a1,b1) ^ GFM(a2,b2) ^ GFM(a3,b3);
		state[1][i]=GFM(a3,b0) ^ GFM(a0,b1) ^ GFM(a1,b2) ^ GFM(a2,b3);
		state[2][i]=GFM(a2,b0) ^ GFM(a3,b1) ^ GFM(a0,b2) ^ GFM(a1,b3);
		state[3][i]=GFM(a1,b0) ^ GFM(a2,b1) ^ GFM(a3,b2) ^ GFM(a0,b3);
	}
}

void M10A17(){
	a0=2; a1=1; a2=1; a3=3;
	for(int i=0;i<4;i++){
	 	b0=state[0][i];
	 	b1=state[1][i];       	
	 	b2=state[2][i];       	
	 	b3=state[3][i];       	       	
	 	s0=b0^b2; s1=b1^b3; s2=a0^a2; s3=a1^a3;
	 	w0=GFM(a0,s0)^GFM(a3,s1); w1=GFM(a1,s0)^GFM(a0,s1);
	 	s4=s2^s3;s5=GFM(s2,b2^b3);s6=GFM(s2,b0^b1);
	 	r0=w0^s5;r1=w1^s5;r2=w0^s6;r3=w1^s6;
	 	state[0][i]=r0^GFM(s4,b3);
	 	state[1][i]=r1^GFM(s4,b2);
	 	state[2][i]=r2^GFM(s4,b1);
	 	state[3][i]=r3^GFM(s4,b0);
	}
}

void M5A18(){
	a0=02; a1=03; a2=01; a3=01;
	for(int i=0;i<4;i++){
		b0=state[0][i];
		b1=state[1][i];       	
		b2=state[2][i];       	
		b3=state[3][i];     
		// matrix left  	       	
		s0=b0^b2; s1=b1^b3; 
		m0=GFM(a0,s0^s1);
		s2=m0^GFM(s1, a0^a1);  
		s3=m0^GFM(s0, a3^a0);
		t=a1^a3;
		// matrix right	 
		m1=GFM(t, b2^b3);
		m2=GFM(t, b0^b1);
		// --- combin matrix
		state[0][i]=s2^m1^b2;  
		state[1][i]=s3^m1^b3;  
		state[2][i]=s2^m2^b0;  
		state[3][i]=s3^m2^b1;  
	}
} 

void MixColumns(){
//	M16A12();
//    M10A17();
    M5A18();
}

void exkey(uint8 *key , unsigned int *W){
	int Rcon[10];			                 	

 	for(int i=0; i<4; i++)
 		W[i] = (key[i*4]<<24) | (key[i*4+1]<<16) | (key[i*4+2]<<8) | key[i*4+3];

    //----rotword()
	int one=1;
	for(int i = 0; i < 10; i++){
	    Rcon[i]=one<<24;
		one= ((one<<1)&0x0ff)^((one>>7)&0x01)*27;  	
	}
   
	for(int i=0; i<44; i+=4)
	{
		W[4+i]=(W[3+i] << 8) | ((W[3+i] >> 24)&0XFF);
		
		W[4+i]=((s_box[(W[4+i]>>24)&0XFF]) << 24 | 
		       (s_box[(W[4+i]>>16)&0XFF]) << 16 | 
			   (s_box[(W[4+i]>> 8)&0XFF]) << 8 | 
			   (s_box[W[4+i]&0XFF]))^W[0+i]^Rcon[i/4];

		W[5+i]=W[4+i]^W[1+i];
		W[6+i]=W[5+i]^W[2+i];
		W[7+i]=W[6+i]^W[3+i];
	}	
}

void AddRoundKey(int c, unsigned int *W){	

	for(int i=0; i<4; i++){
		state[0][i]=state[0][i]^((W[c]>>24)& 0xff);	
		state[1][i]=state[1][i]^((W[c]>>16)& 0xff);	
		state[2][i]=state[2][i]^((W[c]>>8) & 0xff);	
		state[3][i]=state[3][i]^(W[c] & 0xff);	
		c++;		
	}
}

void show(){
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++)
           printf("%.2X ",state[i][j]);      
        printf("\n");   	
	} 
	printf("\n");   	
}

void AES_Encrypt(uint8* ckey){
	unsigned int  W[44];

	exkey(ckey,W);
	AddRoundKey(0, W);	
	
	for(int i=1; i<10; i++){
	    SubByte();
	    Shiftrow(); 
		MixColumns();		
		AddRoundKey(i*4, W);
	}	
	SubByte();
	Shiftrow();
	AddRoundKey(40, W);
}

void AES_decrypt(uint8* key){
	unsigned int  W[44];	
	exkey(key,W);

	AddRoundKey(40, W);	
	for(int i=9; i>0; i--){
		InvShiftRows();
//		show();
		InvSubBytes();
//		show();		
		AddRoundKey(i*4, W);
//		show();	
		InvMixColumns();		
	}	
	
	// Final round
	InvShiftRows();	
	InvSubBytes();
	AddRoundKey(0, W);
} 

void InvShiftRows(){
    int temp,temp1;
    temp=state[1][0];  
    state[1][0]=state[1][3];  
    state[1][3]=state[1][2];  
    state[1][2]=state[1][1];  
    state[1][1]=temp;  

    temp=state[2][0];  
    state[2][0]=state[2][2];  
    state[2][2]=temp;  
    temp=state[2][1];  
    state[2][1]=state[2][3];
    state[2][3]=temp;

    temp=state[3][3];  
    state[3][3]=state[3][0];  
    state[3][0]=state[3][1];  
    state[3][1]=state[3][2];  
    state[3][2]=temp;  		
}

void InvSubBytes(){
	for(int i = 0; i < 4; i++)
    	for(int j = 0; j < 4; j++)
       		state[i][j]=Iv_s_box[state[i][j]];	
}

void Iv_M16A12(){
    a0=0x0e; a1=0x0B; a2=0x0d; a3=0x09;  //Using the coefficent of matrix	
	for(int i=0;i<4;i++){
		b0=state[0][i];
		b1=state[1][i];       	
		b2=state[2][i];       	
		b3=state[3][i];       	       	  
		state[0][i]=GFM(a0,b0) ^ GFM(a1,b1) ^ GFM(a2,b2) ^ GFM(a3,b3);
		state[1][i]=GFM(a3,b0) ^ GFM(a0,b1) ^ GFM(a1,b2) ^ GFM(a2,b3);
		state[2][i]=GFM(a2,b0) ^ GFM(a3,b1) ^ GFM(a0,b2) ^ GFM(a1,b3);
		state[3][i]=GFM(a1,b0) ^ GFM(a2,b1) ^ GFM(a3,b2) ^ GFM(a0,b3);
	}    
} 

void Iv_M10A17(){
    a0=0x0e; a1=0x09; a2=0x0d; a3=0x0B; 
	for(int i=0;i<4;i++){
		b0=state[0][i];
		b1=state[1][i];       	
		b2=state[2][i];       	
		b3=state[3][i];       	       	
		s0=b0^b2; s1=b1^b3; s2=a0^a2; s3=a1^a3;
		w0=GFM(a0,s0)^GFM(a3,s1); w1=GFM(a1,s0)^GFM(a0,s1);
		s4=s2^s3;s5=GFM(s2,b2^b3);s6=GFM(s2,b0^b1);
		r0=w0^s5;r1=w1^s5;r2=w0^s6;r3=w1^s6;
		state[0][i]=r0^GFM(s4,b3);
		state[1][i]=r1^GFM(s4,b2);
		state[2][i]=r2^GFM(s4,b1);
		state[3][i]=r3^GFM(s4,b0);
	}		
} 

void  Iv_M5A18(){
    a0=0x0e; a1=0x0B; a2=0x0d; a3=0x09;
	for(int i=0;i<4;i++){
		b0=state[0][i];
		b1=state[1][i];       	
		b2=state[2][i];       	
		b3=state[3][i];     
		// matrix left  	       	
		s0=b0^b2; s1=b1^b3; 
		m0=GFM(a0,s0^s1);
		s2=m0^GFM(s1, a0^a1);  
		s3=m0^GFM(s0, a3^a0);
		t=a1^a3;
		// matrix right	 
	    m1=GFM(t, b2^b3);
		m2=GFM(t, b0^b1);
		// --- combin matrix
		state[0][i]=s2^m1^b2;  
		state[1][i]=s3^m1^b3;  
		state[2][i]=s2^m2^b0;  
		state[3][i]=s3^m2^b1;  
	}		
}
 
void InvMixColumns(){
//	Iv_M16A12();	
//	Iv_M10A17();
	Iv_M5A18();
}
