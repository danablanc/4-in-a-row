/*
 * SPMainAux.c
 *
 *  Created on: Jun 6, 2017
 *      Author: Ariel
 */

#include "SPMainAux.h"

int SPFiarInit(){
	char input[SP_MAX_LINE_LENGTH]; // stands for getting input from user
	int maxDepth = 0; // stands for difficulty level
	while(true){
		printf("Please enter the difficulty level between [1-7]:\n");
		fgets(input,SP_MAX_LINE_LENGTH,stdin); // gets input from user
		int i = strlen(input)-1;
		if(input[i] == '\n') // puts null character in end of input
			input[i] = '\0';
		if(spParserPraseLine(input).cmd==SP_QUIT){ //command was to quit game
			printf("Exiting...\n");
			exit(0);
			return -1;
		}
		else if(spParserIsInt(input)){ // input is integer
			maxDepth = atoi(input);
			if(maxDepth>=1 && maxDepth<=7) //valid difficulty level
				return maxDepth;
		}
		printf("Error: invalid level (should be between 1 to 7)\n");
	}
}


SPCommand NewCommand(SPFiarGame *game, bool check){
	assert(game!=NULL);
	SPCommand command;
	char input[SP_MAX_LINE_LENGTH];
	if(check){ // checks if needs to print move message and board
		spFiarGamePrintBoard(game);//prints game board
		printf("Please make the next move:\n");
	}
	while(true){ //loop and reading from stdin until gets a valid command
		fgets(input,SP_MAX_LINE_LENGTH,stdin); //gets input from user
		int i = strlen(input)-1;
		if(input[i] == '\n') // puts null character
			input[i] = '\0';
		command = spParserPraseLine(input);  //parsing the read string into command
		if(command.cmd != SP_INVALID_LINE) // command is valid
			return command;
		printf("Error: invalid command\n");

	}
}


bool MakeMove(SPFiarGame *game,int maxDepth, SPCommand command){
	assert(game!=NULL);
	if(command.cmd == SP_ADD_DISC){ //if the command is add disc
		if(command.validArg)
			return AddDisc(game,command.arg); //return true if move succeeded, false otherwise
		else{
			printf("Error: column number must be in range 1-7\n"); //invalid col
			return false; //return false because move was not successful
		}
	}
	if(command.cmd == SP_SUGGEST_MOVE){//if the command suggest move
		int res = spMinimaxSuggestMove(game,maxDepth); // calling minimax algo for getting the best move
		if(res == -1) //problem with allocation
			Fail(game,true);
		else{
			printf("Suggested move: drop a disc to column %d\n",res+1); //suggests new move
			return true;
		}
	}
	return false;
}


bool AddDisc(SPFiarGame *game,int col){
	assert(game!=NULL);
	SP_FIAR_GAME_MESSAGE message = spFiarGameSetMove(game,col-1); //trying to make the asked move
	if(message == SP_FIAR_GAME_INVALID_ARGUMENT){ //invalid arg was given
		printf("Error: column number must be in range 1-7\n");
		return false; //return false because move was not successful
	}
	if(message == SP_FIAR_GAME_INVALID_MOVE){ //asked col is full
		printf("Error: column %d is full\n",col);
		return false; //return false because move was not successful
	}
	return true; //move was successful
}


bool UndoMove(SPFiarGame *game){
	assert(game!=NULL);
	int col=0;
	SP_FIAR_GAME_MESSAGE message;
	for(int i=0;i<2;i++){ //undo two last moves
		col = spArrayListGetLast(game->undo_hist); //getting the last add disc move has been done
		message = spFiarGameUndoPrevMove(game);
		if(message == SP_FIAR_GAME_NO_HISTORY){ //no history of moves is available
			printf("Error: cannot undo previous move!\n");
			return false; //return false because move was not successful
		}
		else if(message == SP_FIAR_GAME_INVALID_ARGUMENT) //allocation problem
			Fail(game,true);
		if(i%2 == 0)
			printf("Remove disc: remove computer's disc at column %d\n",col+1); //computer's move
		else
			printf("Remove disc: remove user's disc at column %d\n",col+1); //disc's move
	}
	return true; // move was successful
}


bool WinnerCheckPrint(SPFiarGame *game){
	assert(game!=NULL);
	char winner = spFiarCheckWinner(game); //checks if there is a winner already
	if(winner == SP_FIAR_GAME_PLAYER_1_SYMBOL){ //the user won
		spFiarGamePrintBoard(game);
		printf("Game over: you win\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
		return true; //return true because there is a winner already
	}
	else if(winner == SP_FIAR_GAME_PLAYER_2_SYMBOL){ //the computer won
		spFiarGamePrintBoard(game);
		printf("Game over: computer wins\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
		return true; //return true because there is a winner already
	}
	else if(winner == SP_FIAR_GAME_TIE_SYMBOL){ //it's a tie
		spFiarGamePrintBoard(game);
		printf("Game over: it's a tie\nPlease enter 'quit' to exit or 'restart' to start a new game!\n");
		return true;
	}
	else
		return false; // there is no winner yet
}


bool ComputerMove(SPFiarGame *game,int maxDepth){
	assert(game!=NULL);
	int col = spMinimaxSuggestMove(game,maxDepth); //make's computer move
	if(col == -1) //allocation problem
		Fail(game,true);
	AddDisc(game,col+1); //making the computer move
	printf("Computer move: add disc to column %d\n",col+1);
	return true; // move was successful
}


void Quit(SPFiarGame *game){
	assert(game!=NULL);
	spFiarGameDestroy(game); //destroys the game
	printf("Exiting...\n");
	exit(0);
}


int Restart(SPFiarGame *game){
	assert(game!=NULL);
	printf("Game restarted!\n");
	spFiarGameDestroy(game); //destroys the game
	int maxDepth = SPFiarInit(); //creates new one
	return maxDepth;
}


void Fail(SPFiarGame *game,bool destroy){
	printf("Error: malloc has failed"); //allocation error
	if(destroy)// need to destroy
		spFiarGameDestroy(game); //destroys the game
	exit(0);
}
