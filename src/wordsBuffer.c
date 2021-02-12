////////////////////////////////////////////////////////////////////////////////
/*!
 * @file wordsBuffer.c
 * @date 1-2014
 * @author Angel Lareo
 * 
 * @brief Implementation of WordsBuffer structure functions
*/
////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "wordsBuffer.h"

/**
 * WordsBuffer constructor
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb       pointer to WordsBuffer
 * @param[in]       length   word length in bits    
 * @param[in]       maxWords max number of words to store
*/
int wbInit(WordsBuffer* wb, int length, int maxWords){
    wb->insert=wb->words;
    wb->init=wb->words;
    wb->check = wb->words;
    wb->bb.wordLength=length;
    wb->numWords=0;
    wb->maxWords=maxWords;
    wb->bb.init = wb->bb.bits;
    wb->bb.insert = wb->bb.bits;
	wb->overlap = -1;
    return OK;
}

int wbInit_overlap(WordsBuffer* wb, int length, int maxWords, int overlap){
    if (overlap>=length)
        return ERR;

    wb->insert=wb->words;
    wb->init=wb->words;
    wb->check = wb->words;
    wb->bb.wordLength=length;
    wb->numWords=0;
    wb->maxWords=maxWords; //Calculated from the GUI
    wb->bb.init = wb->bb.bits;
    wb->bb.insert = wb->bb.bits;
	wb->overlap = overlap;
    return OK;
}

void wbCreateHistogram (WordsBuffer* wb, int* results){
	int i;
	
	for (i=0; i<pow(2,wb->bb.wordLength); i++){
		results[i]=0;
	}
	
	for (i=0; i<wb->numWords; i++){
		results[wb->words[i]]++;
	}
}

/**
 * Insert new bit in WordsBuffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb      pointer to WordsBuffer
 * @param[in]       bit     bit a introducir
*/
int wbBitInsert(WordsBuffer* wb, char bit){  //Inserts bit on BitBuffer
    *(wb->bb.insert)=bit;
    bbAdvancePtr(&(wb->bb), &(wb->bb.insert));
    wb->bb.numBits++;
    return OK;
}

/**
 * Forward insert pointer, circular buffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in]   wb      pointer to WordsBuffer
 * @param[in,out]       ptr     insert pointer to advance
*/
void bbAdvancePtr (BitsBuffer* bb, char** ptr){
    if (*ptr==&(bb->bits[bb->wordLength-1])) *ptr = bb->bits;
    else (*ptr)++;
}

/**
 * Inserts word in WordsBuffer structure
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in,out]   wb      pointer to WordsBuffer
 * @param[in]       word    word value
*/
int wbWordInsert (WordsBuffer* wb, int word){ //Inserts word on WordsBuffer
                                                 //Called by StoreWord
    *wb->insert = word; //copy word value
	wb->check=wb->insert;
    wb->numWords++; 

    if (wb->insert == wb->words+wb->maxWords-1){
        wb->insert = wb->words;
        if (wb->init == wb->words+wb->maxWords-1) wb->init = wb->words;
        else wb->init++;
    } else wb->insert++;

    return OK;
}

/**
 * Converts to decimal value the code word in BitsBuffer and stores it in WordsBuffer
 * It performs bits to integer transformation calling wbBits2Int(wb)
 * ERR if there is no word in the bits buffer
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @see wbWordInsert
 * 
 * @param[in,out]   wb      pointer to WordsBuffer
 * 
 * @return word value or ERR
*/
int wbStoreWord (WordsBuffer *wb){ 
	int wordResult;
    int i;

    if (wb->bb.numBits<wb->bb.wordLength){
		return ERR;
    }
    
    wordResult = wbBits2Int(wb);
	
    //Advance bit init
    if (wb->overlap==-1)
        bbAdvancePtr(&(wb->bb),&(wb->bb.init));
    else{
        for (i=0; i<=(wb->bb.wordLength - wb->overlap); ++i)
            bbAdvancePtr(&(wb->bb),&(wb->bb.init));
    }

    //Insert word
    wbWordInsert(wb, wordResult);

    return wordResult;
}

int wbBits2Int(WordsBuffer *wb){ 
	char *auxPtr;
	int i, exp;
	int wordResult = 0;
	
	exp=wb->bb.wordLength - 1;
	
	auxPtr=wb->bb.init;
    for (i=0; i<wb->bb.wordLength; ++i){
        if ((int)*auxPtr){
            switch (exp){
                case 0: wordResult += 1; break;
                case 1: wordResult += 2; break;
                default: wordResult += pow(2,exp); break;
            }
        }
        exp--;
        bbAdvancePtr(&(wb->bb), &auxPtr);
    }
    
    return wordResult;
}

int Bits2Int(char* bb, int length){ 
	char *auxPtr;
	int i, exp;
	int wordResult = 0;
	
	exp=length - 1;
	
	auxPtr=bb;
    for (i=0; i<length; ++i){
        if ((int)*auxPtr){
            switch (exp){
                case 0: wordResult += 1; break;
                case 1: wordResult += 2; break;
                default: wordResult += pow(2,exp); break;
            }
        }
        exp--;
        auxPtr++;
    }
    
    return wordResult;
}

/**
 * Comprueba si la palabra binaria word coincide con la que se encuentra en la estructura de bits
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in]   wb      puntero a la estructura donde se encuentran los bits de una palabra a comparar
 * @param[in]   word    array de bits con la otra palabra a comparar
*/
int wbCheckWordMatch(WordsBuffer *wb, char *word){
	return (*(wb->check)==Bits2Int(word, wb->bb.wordLength));
}


int wbRestartBitBuff(WordsBuffer *wb){
    wb->bb.numBits = 0;
}

int wbGetWordInt(WordsBuffer *wb){
    return wbBits2Int(wb);
}

/**
 * Check if the bits in BitsBuffer matched with the one received as an argument
 * 
 * @author Ángel Lareo
 * @date 1/2013
 * 
 * @param[in]   wb      pointer to WordsBuffer
 * @param[in]   word    binary code word to check matching
*/
void wbGetWordChar(char* bbOut, WordsBuffer *wb){
    char *auxPtr;
	int i, exp;
	
	exp=wb->bb.wordLength - 1;
	
	auxPtr=wb->bb.init;
    for (i=0; i<wb->bb.wordLength; ++i){
        bbOut[i] = *auxPtr;
        auxPtr++;
    }
}
