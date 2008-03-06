#include <../lib/types.h>
#include <../detectors/ascii/esc_statemachine.h>
#include <../detectors/multibyte/unicode_statemachine.h>

int main(){
	int i=0, j=0;
	u32 data[2] = {
		PCK4BITS(0,1,2,3,4,5,6,7),
		PCK4BITS(8,9,10,11,12,13,14,15)
	};
	PackInt pi = {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, data };
	for( i =0; i< 16; i++){
		j = GETFROMPCK( i, pi );
		printf("Expecting i=%d, get pkg j=%d\n", i, j ); 
	}

	PackInt utf8_pi = {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, UTF8_cls };
	for( i=0; i<256; i++ ){
		if(!( i % 8) ) printf("\n");
		printf("%d,", GETFROMPCK(i,utf8_pi) ); 
	}
	PackInt iso2022cn_pi = {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, ISO2022CN_st };
	for( i=0; i<64; i++ ){
		if(!( i % 8) ) printf("\n");
		printf("%d,", GETFROMPCK(i,iso2022cn_pi) ); 
	}
	return 0;
}
