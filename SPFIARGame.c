/*
 * SPFIARGame.c
 *
 *  Created on: May 28, 2017
 *      Author: Ariel
 */
#include "SPFIARGame.h"


SPFiarGame* spFiarGameCreate(int historySize){
    if(historySize <= 0)
    	return NULL;
    SPFiarGame* new = (SPFiarGame*)malloc(sizeof(SPFiarGame));
    if(!new)
    	return NULL; // allocation failed
    new->undo_hist = spArrayListCreate(historySize);
    if(!(new->undo_hist)){
        free(new);
        return NULL;
    }
    for(int m=0;m<((SP_FIAR_GAME_SPAN*2)+1);m++) //initialize the scoring function
    	new->score_func[m]=0;
    for(int i=0; i < SP_FIAR_GAME_N_ROWS;i++) //initialize gameBoard
        for(int j=0;j<SP_FIAR_GAME_N_COLUMNS;j++)
            new->gameBoard[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
    for(int t=0;t<SP_FIAR_GAME_N_COLUMNS;t++) //initialize tops
        new->tops[t]=0;
    new->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL; //player 1 starts
    return new;
}


SPFiarGame* spFiarGameCopy(SPFiarGame* src){
    if(!src)
    	return NULL;
    SPFiarGame* gamecopy = (SPFiarGame*)malloc(sizeof(SPFiarGame));
    if (!gamecopy)
    	return NULL;
    gamecopy->undo_hist = spArrayListCopy(src->undo_hist);
    if(gamecopy->undo_hist == NULL){
        free(gamecopy);
        return NULL;
    }
    for (int m=0;m<((SP_FIAR_GAME_SPAN*2)+1);m++) //copies score func
    	gamecopy->score_func[m]=src->score_func[m];
    for(int i=0;i<SP_FIAR_GAME_N_ROWS;i++) //copies the gameBoard
        for(int j=0;j<SP_FIAR_GAME_N_COLUMNS;j++)
            (gamecopy->gameBoard)[i][j] = (src->gameBoard)[i][j];
    for(int t=0; t<SP_FIAR_GAME_N_COLUMNS;t++) //copies tops
        (gamecopy->tops)[t] = (src->tops)[t];
    gamecopy->currentPlayer = src->currentPlayer;
    return gamecopy;
}


void spFiarGameDestroy(SPFiarGame* src){
	if(src){	// destroying struct if needed
		spArrayListDestroy(src->undo_hist);
		free(src);
	}
}


SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	if((!src) || (col<0) || (col> SP_FIAR_GAME_N_COLUMNS-1)) //checks validity of the col given
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if(src->tops[col] == SP_FIAR_GAME_N_ROWS) //checks validity of the move
		return SP_FIAR_GAME_INVALID_MOVE;
	spFiarDoMove(src,col); //does the move
	return SP_FIAR_GAME_SUCCESS;
}


bool spFiarGameIsValidMove(SPFiarGame* src, int col){
	if((!src) || (col<0) || (col> SP_FIAR_GAME_N_COLUMNS-1)) //checks validity of the col given
		return false;
	if(src->tops[col] == SP_FIAR_GAME_N_ROWS) //checks validity of the move
		return false;
	return true;
}


SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src){
	if(!src)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if(spArrayListIsEmpty(src->undo_hist)) //checks if the move can be done
		return SP_FIAR_GAME_NO_HISTORY;
	int column = src->undo_hist->elements[(src->undo_hist->actualSize)-1]; //last added disc col
	int row = (src->tops[column])-1; //last added disc row
	src->gameBoard[row][column]= SP_FIAR_GAME_EMPTY_ENTRY; //empties the board in the given entry of the last move
	src->tops[column] = src->tops[column]-1;
	spArrayListRemoveLast(src->undo_hist);
	return SP_FIAR_GAME_SUCCESS;
}


SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if (!src)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	for(int i=SP_FIAR_GAME_N_ROWS-1; i >= 0; i--){
		printf("| ");
		for(int j=0; j<SP_FIAR_GAME_N_COLUMNS;j++){
			printf("%c ",src->gameBoard[i][j]); //prints the gameBoard
		}
		printf("|\n");
	}
	for(int i=0; i<17; i++) //prints the sign "-"
		printf("-");
	printf("\n");
	printf("  ");
	for(int i=1; i<=SP_FIAR_GAME_N_COLUMNS; i++) //prints the number of columns
		printf("%d ",i);
	printf(" ");
	printf("\n");
	return SP_FIAR_GAME_SUCCESS;
}


char spFiarGameGetCurrentPlayer(SPFiarGame* src){
	if(!src)
		return SP_FIAR_GAME_EMPTY_ENTRY;
	else
		return src->currentPlayer;
}


char spFiarCheckWinner(SPFiarGame* src){
	if(!src)
		return '\0';
	spFiarSpan(src); //updates the scoring function
	if(src->score_func[0]!=0) //checks if player 2 has a 4
		return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	if(src->score_func[SP_FIAR_GAME_SPAN*2]!=0)//checks if player 1 has a 4
		return SP_FIAR_GAME_PLAYER_1_SYMBOL;
	else{
		for(int i=0; i<SP_FIAR_GAME_N_COLUMNS; i++){ //checks if game is not over
			if(src->tops[i]<SP_FIAR_GAME_N_ROWS)
				return '\0';
		}
		return SP_FIAR_GAME_TIE_SYMBOL; //there is a tie
	}
}


void spFiarSpan(SPFiarGame* src){
	assert(src!=NULL);
	for(int i=0; i<((SP_FIAR_GAME_SPAN*2)+1); i++) //initialize the scoring function
			src->score_func[i]=0;
	spFiarSpanColumns(src); //help func for column spans computation
	spFiarSpanRows(src); //help func for rows spans computation
	spFiarSpanDiagonals(src); //help func for diagonal spans computation
}


void spFiarSpanColumns(SPFiarGame* src){
	assert(src!=NULL);
	for(int i=0; i<SP_FIAR_GAME_N_ROWS-3; i++){ //computes the spans of the columns
		for(int j=0; j<SP_FIAR_GAME_N_COLUMNS; j++){
			int sum = 0;
			for(int k=0; k<SP_FIAR_GAME_SPAN; k++){
				if(src->gameBoard[i+k][j] == SP_FIAR_GAME_PLAYER_1_SYMBOL)
					sum++;
				if(src->gameBoard[i+k][j] == SP_FIAR_GAME_PLAYER_2_SYMBOL)
					sum--;
			}
			if (sum!=0)
				src->score_func[sum+SP_FIAR_GAME_SPAN]++;
		}
	}
}



void spFiarSpanRows(SPFiarGame* src){
	assert(src!=NULL);
	for(int i=0; i<SP_FIAR_GAME_N_ROWS; i++){ //computes the spans of the rows
		for(int j=0; j<SP_FIAR_GAME_N_COLUMNS-3; j++){
			int sum = 0;
			for(int k=0; k<SP_FIAR_GAME_SPAN; k++){
				if(src->gameBoard[i][j+k] == SP_FIAR_GAME_PLAYER_1_SYMBOL)
					sum++;
				if(src->gameBoard[i][j+k] == SP_FIAR_GAME_PLAYER_2_SYMBOL)
					sum--;
			}
			if (sum!=0)
				src->score_func[sum+SP_FIAR_GAME_SPAN]++;
		}
	}
}


void spFiarSpanDiagonals(SPFiarGame* src){
	assert(src!=NULL);
	for(int i=0; i<SP_FIAR_GAME_N_ROWS-3; i++){ //computes the spans of the diagonal (to the right)
			for(int j=0; j<SP_FIAR_GAME_N_COLUMNS-3; j++){
				int sum = 0;
				for(int k=0; k<SP_FIAR_GAME_SPAN; k++){
					if(src->gameBoard[i+k][j+k] == SP_FIAR_GAME_PLAYER_1_SYMBOL)
						sum++;
					if(src->gameBoard[i+k][j+k] == SP_FIAR_GAME_PLAYER_2_SYMBOL)
						sum--;
				}
				if (sum!=0)
					src->score_func[sum+SP_FIAR_GAME_SPAN]++;
			}
	}
	for(int i=0; i<SP_FIAR_GAME_N_ROWS-3; i++){ //computes the spans of the diagonal (to the left)
		for(int j=SP_FIAR_GAME_N_COLUMNS-1; j>SP_FIAR_GAME_N_COLUMNS-5; j--){
			int sum = 0;
			for(int k=0; k<SP_FIAR_GAME_SPAN; k++){
				if(src->gameBoard[i+k][j-k] == SP_FIAR_GAME_PLAYER_1_SYMBOL)
					sum++;
				if(src->gameBoard[i+k][j-k] == SP_FIAR_GAME_PLAYER_2_SYMBOL)
					sum--;
			}
			if (sum!=0)
			src->score_func[sum+SP_FIAR_GAME_SPAN]++;
		}
	}
}



void spFiarDoMove(SPFiarGame* src , int col){
	assert(src!=NULL);
	src->gameBoard[src->tops[col]][col] = src->currentPlayer; //puts the player's symbol in the desired place
	src->tops[col]++; //updates the column info
	spFiarSwitchPlayer(src); //switch player
	if(spArrayListIsFull(src->undo_hist)) //updates the history array list which contains previous moves
		spArrayListRemoveFirst(src->undo_hist);
	spArrayListAddLast(src->undo_hist, col); //updates the history array list which contains previous moves
}



void spFiarSwitchPlayer(SPFiarGame* src){
	assert(src!=NULL); //checks that the game src is a valid arg
	if((src->currentPlayer) == SP_FIAR_GAME_PLAYER_1_SYMBOL)
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	else
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
}



