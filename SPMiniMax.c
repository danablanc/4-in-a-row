/*
 * SPMiniMax.c
 *
 *  Created on: Jun 1, 2017
 *      Author: Ariel
 */
#include "SPMiniMax.h"


int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth){
	if(!currentGame || maxDepth <= 0) //checks validity of input
		return -1;
	return SPFiarMinMax(currentGame,maxDepth); //returns the suggested move
}

