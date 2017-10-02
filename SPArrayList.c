/*
 * SPArrayList.c
 *
 *  Created on: May 28, 2017
 *      Author: Ariel
 */
#include "SPArrayList.h"


SPArrayList* spArrayListCreate(int maxSize){
	if (maxSize <= 0) //checks if the size is valid
		return NULL;
	SPArrayList* arr = (SPArrayList*) malloc(sizeof(SPArrayList)); //allocates space for the arraylise
	if(!arr) //checks if allocation is fine
		return NULL;
	int* elem = (int*) malloc(maxSize*(sizeof(int)));
	if(!elem){ //checks allocation
		free(arr);
		return NULL;
	}
	arr->maxSize = maxSize;
	arr->elements = elem;
	arr->actualSize = 0;
	return arr;
}


SPArrayList* spArrayListCopy(SPArrayList* src){
	if(src == NULL) //checks allocation
		return NULL;
	SPArrayList* arr = spArrayListCreate(src->maxSize); //creates new arraylist
	if(!arr) //checks allocation
		return NULL;
	arr->actualSize = src->actualSize;
	for (int i=0;i<src->actualSize;i++) //copies elements
		arr->elements[i]=src->elements[i];
	return arr;
}


void spArrayListDestroy(SPArrayList* src){
	if(src){  // destroying struct if needed
		free(src->elements);
		free(src);
	}
}


SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if(!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	src->actualSize=0; //changes the actual size of the array - lazy clean
	return SP_ARRAY_LIST_SUCCESS;
}


SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index){ //gets index between zero to actualSize-1
	if(!src || (index<0 || index>(src->maxSize)-1) || index>(src->actualSize)) //checks validity of the index
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(src->actualSize == src->maxSize) //checks if the array is full
		return SP_ARRAY_LIST_FULL;
    for (int i = src->actualSize; i > index ; --i) { //copies the elements
        (src->elements)[i] = (src->elements)[i-1];
    }
	src->elements[index] = elem; //inserts the element
	src->actualSize = src->actualSize + 1;
	return SP_ARRAY_LIST_SUCCESS;
}


SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem){
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	return spArrayListAddAt(src,elem,0);
}


SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem){
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(spArrayListIsFull(src))
			return SP_ARRAY_LIST_FULL;
	return spArrayListAddAt(src,elem,src->actualSize);
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){ //gets index between zero to actualSize-1
	if(!src || (index<0 || index>(src->actualSize)-1)) //checks validity of the index
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(src->actualSize == 0) //if the array is empty
		return SP_ARRAY_LIST_EMPTY;
    for (int i = index; i < src->actualSize-1; ++i) {//copies elements
        (src->elements)[i] = (src->elements)[i+1];
    }
	src->actualSize = src->actualSize - 1;
	return SP_ARRAY_LIST_SUCCESS;
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(spArrayListIsEmpty(src))
		return SP_ARRAY_LIST_EMPTY;
	return spArrayListRemoveAt(src,0);
}


SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if (!src)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if(spArrayListIsEmpty(src))
		return SP_ARRAY_LIST_EMPTY;
	return spArrayListRemoveAt(src,(src->actualSize)-1);
}


int spArrayListGetAt(SPArrayList* src, int index){
	if (index<0 || index>src->actualSize-1 || (!src)) //checks validity of the input
		return -1;
	else
		return src->elements[index];
}


int spArrayListGetFirst(SPArrayList* src){
	if (!src)
		return -1;
	return spArrayListGetAt(src,0);
}


int spArrayListGetLast(SPArrayList* src){
	if (!src)
		return -1;
	return spArrayListGetAt(src,(src->actualSize)-1);
}


int spArrayListMaxCapacity(SPArrayList* src){
	if (!src)
		return -1;
	return src->maxSize;
}


int spArrayListSize(SPArrayList* src){
	if (!src)
		return -1;
	return src->actualSize;
}


bool spArrayListIsFull(SPArrayList* src){
	if ((!src) || ((src->actualSize)<(src->maxSize)))
		return false;
	return true;
}


bool spArrayListIsEmpty(SPArrayList* src){
	if ((!src) || ((src->actualSize)!=0))
		return false;
	return true;
}
