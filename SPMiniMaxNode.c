/*
 * SPMiniMaxNode.c
 *
 *  Created on: Jun 1, 2017
 *      Author: Ariel
 */
#include "SPMiniMaxNode.h"



int SPFiarMinMax(SPFiarGame* currentGame, unsigned int maxDepth){
	assert(currentGame!=NULL); //asserts allocation
	Node* node = spFiarNodeCreate(currentGame); //creates new node
	if(!node)
		return -1;
	SPFiarMinMaxRec(node, 0, maxDepth, spFiarGameGetCurrentPlayer(currentGame)
			==SP_FIAR_GAME_PLAYER_1_SYMBOL); //invokes the recursion of minimax
	int col=node->best_col; //returns the best move
	if(col==-1){ // all moves will lead to lose so col was not changed during recursion
		for(int i=0;i<SP_FIAR_GAME_N_COLUMNS;i++){ //checks for the min index of not full col
			if(spFiarGameIsValidMove(currentGame,i)){
				col=i;
				break;
			}
		}
	}
	if(col == -2) // there was allocation error while recursion
		col = -1;
	spFiarNodeDestroy(node); //destroying struct
	return col;
}


int SPFiarMinMaxRec(Node* src, int level, int maxDepth, bool maximize){
	assert(src!=NULL); //asserts allocation
	int best_val=0;
	int check = SPFiarCallerWin(src); // checks for winner
	if(check != 0) //halting cond - there is a winner
		return check; //return max or min value - according to the player
	if(level == maxDepth){ //halting cond - max depth
		spFiarSpan(src->game_copy);
		return SPFiarVector(src->game_copy->score_func); //return the value computed
	}
	if(maximize) //computes the starting comparison value according to the plated
		best_val = INT_MIN;
	else
		best_val = INT_MAX;
	for(int i=0; i<SP_FIAR_GAME_N_COLUMNS; i++){ //for every column in the game board
		if(spFiarGameIsValidMove(src->game_copy,i)){ //checks the validation of the move
			Node* child = spFiarNodeCopy(src); //creates new node to represent the child
			if(child == NULL){ // allocation error
				src->best_col = -2;  // flag for allocation error
				return 0;
			}
			spFiarGameSetMove(child->game_copy, i); //sets the move according to the column
			int val = SPFiarMinMaxRec(child, level+1, maxDepth, !maximize); //calls the recursion
			if(src->best_col == -2){ // there was allocation error while recursion
				spFiarNodeDestroy(child); // destroying node copy
				return 0;
			}
			if(level == 0){ //it we got to the root - compute the value
				int temp = best_val;
				best_val = SPFiarValue(best_val,val,maximize); //computes the new val
				if(best_val != temp) //checks if val was modified, is so, need to update col
					src->best_col = i;
			}
			else
				best_val = SPFiarValue(best_val,val,maximize); //computes the new val
			spFiarNodeDestroy(child); // free memory
		}
	}
	return best_val;
}


Node* spFiarNodeCreate(SPFiarGame* src){
	if(!src) //checks allocation
		return NULL;
	Node* node = (Node*) malloc(sizeof(Node)); //allocates space for the SPFiarGame
	if(!node) //checks allocation
		return NULL;
	node->game_copy = spFiarGameCopy(src);
	if(!(node->game_copy)){ //checks allocation
		free(node);
		return NULL;
	}
	node->best_col=-1;
	node->player= src->currentPlayer;
	return node;
}


void spFiarNodeDestroy(Node* src){
	if(src){ //destroying struct if needed
		spFiarGameDestroy(src->game_copy);
		free(src);
	}
}


Node* spFiarNodeCopy(Node* src){
	if(!src)
		return NULL;
	Node* res = (Node*) spFiarNodeCreate(src->game_copy);
	if(!res)
		return NULL;
	res->best_col = src->best_col;
	res->player = src->player;
	return res;
}


int SPFiarVector(int arr []){
	assert(arr!=NULL);
	int vec[(SP_FIAR_GAME_SPAN*2)-2]= VEC; //create array that represent the weight vector
	if(arr[0]>=1) //if player 2 won
		return INT_MIN;
	if(arr[8]>=1) //if player 1 won
		return INT_MAX;
	int sum = 0;
	for(int i=0; i<6; i++){ //computes the scoring func according to the weight vector & the span array
		if(i<3){
			sum += arr[i+1]*vec[i];
		}
		else{
			sum += arr[i+2]*vec[i];
		}
	}
	return sum;
}

int SPFiarCallerWin(Node* src){
	assert(src!=NULL);
	if(spFiarCheckWinner(src->game_copy) == SP_FIAR_GAME_PLAYER_1_SYMBOL) //checks if the player 1 won
		return INT_MAX;
	else if((spFiarCheckWinner(src->game_copy) != SP_FIAR_GAME_TIE_SYMBOL) && (spFiarCheckWinner(src->game_copy) != '\0'))
		//checks if the other player won
		return INT_MIN;
	return 0;
}

int SPFiarValue(int a, int b, bool maximize){
	if(maximize) //checks if need to maximize or minimize
		return max(a,b);
	else
		return min(a,b);
}
