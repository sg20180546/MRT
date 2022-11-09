#include "common.h"



bool CheckToMove(struct Block* check_block){

	int i;
	int j;


	for(i=0;i<BLOCK_HEIGHT;i++){
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[check_block->shape][check_block->rotate][i][j]==1){
				
				if(check_block->y+i>=HEIGHT ){
					return false;
				}

				if(field[check_block->y+i][check_block->x+j]==1){
					return false;	
				}
				if(check_block->x+j<0 || check_block->x+j>=WIDTH){
					return false;
				}

			}
		}
	}
	return true;
}