#ifndef AISHELL_H
#define AISHELL_H

#pragma once
#include "Move.h"
#include <vector>


// A new AIShell will be created for every move request.
class AIShell 
{
	struct badMove
	{
		Move move;
		double diffInterrupts;
		double diffInterruptsGreaterThanOne;
		int diffSurrounding;

		double diffAI;
		double diffAIGreaterThanOne;
		int diffSurroundingAI;
	};


public:
	//these represent the values for each piece type.
	static const int AI_PIECE = 1;
	static const int HUMAN_PIECE = -1;
	static const int NO_PIECE = 0;


private:
	//Do not alter the values of numRows or numcols.
	//they are used for deallocating the gameState variable.
	int numRows; //the total number of rows in the game state.
	int numCols; //the total number of columns in the game state.
	int **gameState; //a pointer to a two-dimensional array representing the game state.
	bool gravityOn; //this will be true if gravity is turned on. It will be false if gravity is turned off.
	Move lastMove; //this is the move made last by your opponent. If your opponent has not made a move yet (you move first) then this move will hold the value (-1, -1) instead.


public:
	int deadline; //this is how many milliseconds the AI has to make move.
	int k;        // k is the number of pieces a player must get in a row/column/diagonal to win the game. IE in connect 4, this variable would be 4

	AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove);
	~AIShell();
	Move makeMove();
	Move gravityGame();
	Move noGravityGame();

	void swSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &sw, bool &swblank);
	void neSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &sw, bool &swblank, bool &ne, bool &neblank, double &maxInterrupts, double &totalInterruptsGreaterThanOne);
	void swAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &swAI, bool &swAIblank);
	void neAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &swAI, bool &swAIblank, bool &neAI, bool &neAIblank, double &maxAI, double &totalAIGreaterThanOne);

	void wSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &w, bool &wblank);
	void eSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &w, bool &wblank, bool &e, bool &eblank, double &maxInterrupts, double &totalInterruptsGreaterThanOne);
	void wAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &wAI, bool &wAIblank);
	void eAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &wAI, bool &wAIblank, bool &eAI, bool &eAIblank, double &maxAI, double &totalAIGreaterThanOne);

	void nwSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &nw, bool &nwblank);
	void seSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &nw, bool &nwblank, bool &se, bool &seblank, double &maxInterrupts, double &totalInterruptsGreaterThanOne);
	void nwAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &nwAI, bool &nwAIblank);
	void seAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &nwAI, bool &nwAIblank, bool &seAI, bool &seAIblank, double &maxAI, double &totalAIGreaterThanOne);
	
	void sSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &s, bool &sblank);
	void nSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &s, bool &sblank, bool &n, bool &nblank, double &maxInterrupts, double &totalInterruptsGreaterThanOne);
	void sAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &sAI, bool &sAIblank);
	void nAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &sAI, bool &sAIblank, bool &nAI, bool &nAIblank, double &maxAI, double &totalAIGreaterThanOne);
	
	void chooseFinalMove(int col, int row, Move &finalMove, Move &offence, bool &useOffence, double &maxInterrupts, double &maxAI, double &mostInterrupts, double &mostAI, double &mostInterruptsGreaterThanOne, 
		double &mostAIGreaterThanOne, double &totalInterruptsGreaterThanOne, double &totalAIGreaterThanOne, int &mostSurrounding, int &mostSurroundingAI, int &surrounding, int &surroundingAI);
	
	bool gravityFinalMove(Move &m, int col, int row, bool &aboveScan, bool &goToReturnFinalMove, Move &finalMove, double &maxInterrupts, double &maxAI, double &mostInterrupts, double &mostAI, double &mostInterruptsGreaterThanOne,
		double &mostAIGreaterThanOne, double &totalInterruptsGreaterThanOne, double &totalAIGreaterThanOne, int &mostSurrounding, int &mostSurroundingAI, int &surrounding, int &surroundingAI);
	bool gravityMoveWasChosen(bool &aboveScan, int row, Move &finalMove, double &topMostInterrupts, double &mostInterrupts, double &topMostInterruptsGreaterThanOne, double &mostInterruptsGreaterThanOne,
		int &topMostSurrounding, int &mostSurrounding, double &topMostAI, double &mostAI, double &topMostAIGreaterThanOne, double &mostAIGreaterThanOne, int &topMostSurroundingAI, int &mostSurroundingAI,
		int &goodMoveCount, Move &m, std::vector<badMove> &badMoveVector, Move &previousFinalMove, double &previousMostInterrupts, double &previousMostInterruptsGreaterThanOne,
		int &previousMostSurrounding, double &previousMostAI, double &previousMostAIGreaterThanOne, int &previousMostSurroundingAI);
	void ifNoGoodMoves(std::vector<badMove> &badMoveVector, Move &finalMove);
};

#endif //AISHELL_H#pragma once
