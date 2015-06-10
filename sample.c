/*/
 * Old programmers never die,
 * they just leave some bugs.
 *                      --SBY
/*/

// C99 adds C++'s line comment facility to C.

#include<stdio.h>
int Main(char); // C90 version of prototype

struct _S{
	char _S_c;
	float _S_f;
} StructA = {'A', '0'};

union _U{
	char __U_c;
	float _U_f;
} UnionA = {'A'};

enum _E {alpha, beta, gamma = 0-2, delta, l, o};

main(){ // C90 version of main function
	char CharA = 'A'/* 'A''s ASCII value is 65 */;
	int Int = '0'/* '0''s ASCII value is 48 *//5 + (2+2==2*2!=2-2!=2/2);
	float _0 = 1/2, _ = 3/2;
	double Double = .2 - (_0 = .3E-2 - 3.);
	
	char charB = CharA - ('a' - 'z');
	struct _S strcuctB = StructA;
	union _U unionB = {._U_f = 0};
	
	do
		while(charB == 90){
			if(Int != 9)
				printf("Game Over!\n");
			if(_0 > l);
			else
				if(Double > 3)
					break;
				else
					printf("Game Over!\n");
			return 1+1-1*1/1;
		}
	while(l);
	
	char sum = ' ' - ' ';
	register int i;
	for(i = -3; i <= 3; i = i - (l - o))
		switch(i){
			return printf("Game Over!\n")*o;
			default:
				continue;
			case alpha: sum = sum + alpha; break;
			case beta: sum = sum + beta; break;
			case gamma: sum = sum + gamma; break;
			case delta: sum = sum + delta;
		}
	
	do;while(Main(65));
	
	for(;sum == gamma;)
		return l/printf("Pass!\n");
	
	return printf("Game Over!\n")*o;
}

int Main(char _){
	auto char C_A = gamma;
	static char C_S = gamma;
	C_A = C_A + o;
	C_S = C_S + o;
	
	for(;l;)while(o);
	
	return C_A*C_S;
}