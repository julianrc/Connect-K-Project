#include "AIShell.h"
#include <iostream>
#include <cmath>

AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
	this->deadline = 0;
	this->numRows = numRows;
	this->numCols = numCols;
	this->gravityOn = gravityOn;
	this->gameState = gameState;
	this->lastMove = lastMove;
}

AIShell::~AIShell()
{

	//delete the gameState variable.
	for (int i = 0; i<numCols; i++) 
	{
		delete[] gameState[i];
	}
	delete[] gameState;

}

Move AIShell::makeMove()
{
	//this part should be filled in by the student to implement the AI
	//Example of a move could be: Move move(1, 2); //this will make a move at col 1, row 2
	

	if (gravityOn)										//gravity on
		return gravityGame();

	else if (!gravityOn)										//gravity off
		return noGravityGame();

	Move m(numCols/2, numRows/2);
	return m;
}

void AIShell::swSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &sw, bool &swblank)
{
	bool sawAHuman = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col - 1, searchRow = row - 1;				//southwest search

	while (searchCol >= 0 && searchRow >= 0 && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			sawAHuman = true;

			if ((sw || count == 0) && !addABonus)
			{
				consecutive++;
				sw = true;
			}
			else
				interruptsGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			if (count == 1)
				sw = false;
			break;
		}
		else
		{
			if (sawAHuman)
				addABonus = true;
			else
				swblank = true;

			possible++;
		}
		searchCol--;
		searchRow--;
		count++;
	}
	if (addABonus)
		consecutive += .1;
}

void AIShell::neSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &sw, bool &swblank, bool &ne, bool &neblank, double &maxInterrupts, double &totalInterruptsGreaterThanOne)
{
	bool sawAHuman = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col + 1, searchRow = row + 1;				//northeast search

	while (searchCol < numCols && searchRow < numRows && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			sawAHuman = true;

			if ((ne || count == 0) && !addABonus)
			{
				consecutive++;
				ne = true;
				if (swblank && count == 0)
					consecutive += .1;
			}
			else
				interruptsGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			if (count == 1)
				ne = false;
			break;
		}
		else
		{
			if (sawAHuman)
				addABonus = true;
			else
				neblank = true;

			possible++;
		}
		searchCol++;
		searchRow++;
		count++;
	}
	if (addABonus)
		consecutive += .1;
	if (sw && neblank)
		consecutive += .1;
	
	if (consecutive + possible + interruptsGreaterThanOne >= k && consecutive > maxInterrupts)
		maxInterrupts = consecutive;
	totalInterruptsGreaterThanOne += interruptsGreaterThanOne;
}

void AIShell::swAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &swAI, bool &swAIblank)
{
	bool sawAnAI = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col - 1, searchRow = row - 1;				//southwest search AI
	
	while (searchCol >= 0 && searchRow >= 0 && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			sawAnAI = true;

			if ((swAI || count == 0) && !addABonus)
			{
				consecutiveAI++;
				swAI = true;
			}
			else
				AIGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			if (count == 1)
				swAI = false;

			break;
		}
		else
		{
			if (sawAnAI)
				addABonus = true;
			else
				swAIblank = true;
			possibleAI++;
		}
		searchCol--;
		searchRow--;
		count++;
	}
	if (addABonus)
		consecutiveAI += .1;
}

void AIShell::neAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &swAI, bool &swAIblank, bool &neAI, bool &neAIblank, double &maxAI, double &totalAIGreaterThanOne)
{
	bool sawAnAI = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col + 1, searchRow = row + 1;				//northeast search AI

	while (searchCol < numCols && searchRow < numRows && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			sawAnAI = true;

			if ((neAI || count == 0) && !addABonus)
			{
				consecutiveAI++;
				neAI = true;
				if (swAIblank && count == 0)
					consecutiveAI += .1;
			}
			else
				AIGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			if (count == 1)
				neAI = false;
			break;
		}
		else
		{
			if (sawAnAI)
				addABonus = true;
			else
				neAIblank = true;
			possibleAI++;
		}
		searchCol++;
		searchRow++;
		count++;
	}
	if (addABonus)
		consecutiveAI += .1;
	if (swAI && neAIblank)
		consecutiveAI += .1;
	if (consecutiveAI + possibleAI + AIGreaterThanOne >= k && consecutiveAI > maxAI)
		maxAI = consecutiveAI;
	totalAIGreaterThanOne += AIGreaterThanOne;
}

void AIShell::wSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &w, bool &wblank)
{
	bool sawAHuman = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col - 1, searchRow = row;				//west search
	
	while (searchCol >= 0 && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			sawAHuman = true;

			if ((w || count == 0) && !addABonus)
			{
				consecutive++;
				w = true;
			}
			else
				interruptsGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			if (count == 1)
				w = false;

			break;
		}
		else
		{
			if (sawAHuman)
				addABonus = true;
			else
				wblank = true;

			possible++;
		}
		searchCol--;
		count++;
	}
	if (addABonus)
		consecutive += .1;
}

void AIShell::eSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &w, bool &wblank, bool &e, bool &eblank, double &maxInterrupts, double &totalInterruptsGreaterThanOne)
{
	bool sawAHuman = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col + 1, searchRow = row;				//east search

	while (searchCol < numCols && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			sawAHuman = true;

			if ((e || count == 0) && !addABonus)
			{
				consecutive++;
				e = true;
				if (wblank && count == 0)
					consecutive += .1;
			}
			else
				interruptsGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			if (count == 1)
				e = false;
			break;
		}
		else
		{
			if (sawAHuman)
				addABonus = true;
			else
				eblank = true;

			possible++;
		}
		searchCol++;
		count++;
	}
	if (addABonus)
		consecutive += .1;
	if (w && eblank)
		consecutive += .1;
	if (consecutive + possible + interruptsGreaterThanOne >= k && consecutive > maxInterrupts)
		maxInterrupts = consecutive;
	totalInterruptsGreaterThanOne += interruptsGreaterThanOne;
}

void AIShell::wAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &wAI, bool &wAIblank)
{
	bool sawAnAI = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col - 1, searchRow = row;				//west search AI

	while (searchCol >= 0 && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			sawAnAI = true;

			if ((wAI || count == 0) && !addABonus)
			{
				consecutiveAI++;
				wAI = true;
			}
			else
				AIGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			if (count == 1)
				wAI = false;
			break;
		}
		else
		{
			if (sawAnAI)
				addABonus = true;
			else
				wAIblank = true;

			possibleAI++;
		}
		searchCol--;
		count++;
	}
	if (addABonus)
		consecutiveAI += .1;
}

void AIShell::eAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &wAI, bool &wAIblank, bool &eAI, bool &eAIblank, double &maxAI, double &totalAIGreaterThanOne)
{
	bool sawAnAI = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col + 1, searchRow = row;				//east search AI

	while (searchCol < numCols && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			sawAnAI = true;

			if ((eAI || count == 0) && !addABonus)
			{
				consecutiveAI++;
				eAI = true;
				if (wAIblank && count == 0)
					consecutiveAI += .1;
			}
			else
				AIGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			if (count == 1)
				eAI = false;
			break;
		}
		else
		{
			if (sawAnAI)
				addABonus = true;
			else
				eAIblank = true;
			possibleAI++;
		}
		searchCol++;
		count++;
	}
	if (addABonus)
		consecutiveAI += .1;
	if (wAI && eAIblank)
		consecutiveAI += .1;
	if (consecutiveAI + possibleAI + AIGreaterThanOne >= k && consecutiveAI > maxAI)
		maxAI = consecutiveAI;
	totalAIGreaterThanOne += AIGreaterThanOne;
}

void AIShell::nwSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &nw, bool &nwblank)
{
	bool sawAHuman = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col - 1, searchRow = row + 1;				//northwest search
	
	while (searchCol >= 0 && searchRow < numRows && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			sawAHuman = true;

			if ((nw || count == 0) && !addABonus)
			{
				consecutive++;
				nw = true;
			}
			else
				interruptsGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			if (count == 1)
				nw = false;
			break;
		}
		else
		{
			if (sawAHuman)
				addABonus = true;
			else
				nwblank = true;

			possible++;
		}
		searchCol--;
		searchRow++;
		count++;
	}
	if (addABonus)
		consecutive += .1;
}

void AIShell::seSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &nw, bool &nwblank, bool &se, bool &seblank, double &maxInterrupts, double &totalInterruptsGreaterThanOne)
{
	bool sawAHuman = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col + 1, searchRow = row - 1;				//southeast search
	
	while (searchCol < numCols && searchRow >= 0 && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			sawAHuman = true;

			if ((se || count == 0) && !addABonus)
			{
				consecutive++;
				se = true;
				if (nwblank && count == 0)
					consecutive += .1;
			}
			else
				interruptsGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			if (count == 1)
				se = false;
			break;
		}
		else
		{
			if (sawAHuman)
				addABonus = true;
			else
				seblank = true;

			possible++;
		}
		searchCol++;
		searchRow--;
		count++;
	}
	if (addABonus)
		consecutive += .1;
	if (nw && seblank)
		consecutive += .1;
	if (consecutive + possible + interruptsGreaterThanOne >= k && consecutive > maxInterrupts)
		maxInterrupts = consecutive;
	totalInterruptsGreaterThanOne += interruptsGreaterThanOne;
}

void AIShell::nwAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &nwAI, bool &nwAIblank)
{
	bool sawAnAI = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col - 1, searchRow = row + 1;				//northwest search AI
	
	while (searchCol >= 0 && searchRow < numRows && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			sawAnAI = true;

			if ((nwAI || count == 0) && !addABonus)
			{
				consecutiveAI++;
				nwAI = true;
			}
			else
				AIGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			if (count == 1)
				nwAI = false;
			break;
		}
		else
		{
			if (sawAnAI)
				addABonus = true;
			else
				nwAIblank = true;
			possibleAI++;
		}
		searchCol--;
		searchRow++;
		count++;
	}
	if (addABonus)
		consecutiveAI += .1;
}

void AIShell::seAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &nwAI, bool &nwAIblank, bool &seAI, bool &seAIblank, double &maxAI, double &totalAIGreaterThanOne)
{
	bool sawAnAI = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col + 1, searchRow = row - 1;				//southeast search AI
	
	while (searchCol < numCols && searchRow >= 0 && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			sawAnAI = true;

			if ((seAI || count == 0) && !addABonus)
			{
				consecutiveAI++;
				seAI = true;
				if (nwAIblank && count == 0)
					consecutiveAI += .1;
			}
			else
				AIGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			if (count == 1)
				seAI = false;
			break;
		}
		else
		{
			if (sawAnAI)
				addABonus = true;
			else
				seAIblank = true;
			possibleAI++;
		}
		searchCol++;
		searchRow--;
		count++;
	}
	if (addABonus)
		consecutiveAI += .1;
	if (nwAI && seAIblank)
		consecutiveAI += .1;
	if (consecutiveAI + possibleAI + AIGreaterThanOne >= k && consecutiveAI > maxAI)
		maxAI = consecutiveAI;
	totalAIGreaterThanOne += AIGreaterThanOne;
}

void AIShell::sSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &s, bool &sblank)
{
	bool sawAHuman = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col, searchRow = row - 1;				//south search
	
	while (searchRow >= 0 && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			sawAHuman = true;

			if ((s || count == 0) && !addABonus)
			{
				consecutive++;
				s = true;
			}
			else
				interruptsGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			if (count == 1)
				s = false;
			break;
		}
		else
		{
			if (sawAHuman)
				addABonus = true;
			else
				sblank = true;

			possible++;
		}
		searchRow--;
		count++;
	}
	if (addABonus)
		consecutive += .1;
}

void AIShell::nSearch(int col, int row, double &consecutive, double &interruptsGreaterThanOne, double &possible, bool &s, bool &sblank, bool &n, bool &nblank, double &maxInterrupts, double &totalInterruptsGreaterThanOne)
{
	bool sawAHuman = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col, searchRow = row + 1;				//north search
	
	while (searchRow < numRows && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			sawAHuman = true;

			if ((n || count == 0) && !addABonus)
			{
				consecutive++;
				n = true;
				if (sblank && count == 0)
					consecutive += .1;
			}
			else
				interruptsGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			if (count == 1)
				n = false;
			break;
		}
		else
		{
			if (sawAHuman)
				addABonus = true;
			else
				nblank = true;
			possible++;
		}
		searchRow++;
		count++;
	}
	if (addABonus)
		consecutive += .1;
	if (s && nblank)
		consecutive += .1;
	if (consecutive + possible + interruptsGreaterThanOne >= k && consecutive > maxInterrupts)
		maxInterrupts = consecutive;
	totalInterruptsGreaterThanOne += interruptsGreaterThanOne;
}

void AIShell::sAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &sAI, bool &sAIblank)
{
	bool sawAnAI = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col, searchRow = row - 1;				//south search AI
	
	while (searchRow >= 0 && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			sawAnAI = true;

			if ((sAI || count == 0) && !addABonus)
			{
				consecutiveAI++;
				sAI = true;
			}
			else
				AIGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			if (count == 1)
				sAI = false;
			break;
		}
		else
		{
			if (sawAnAI)
				addABonus = true;
			else
				sAIblank = true;
			possibleAI++;
		}
		searchRow--;
		count++;
	}
	if (addABonus)
		consecutiveAI += .1;
}

void AIShell::nAISearch(int col, int row, double &consecutiveAI, double &AIGreaterThanOne, double &possibleAI, bool &sAI, bool &sAIblank, bool &nAI, bool &nAIblank, double &maxAI, double &totalAIGreaterThanOne)
{
	bool sawAnAI = false;
	bool addABonus = false;
	int count = 0;
	int searchCol = col, searchRow = row + 1;				//north search AI
	
	while (searchRow < numRows && count < k - 1)
	{
		if (gameState[searchCol][searchRow] == AI_PIECE)
		{
			sawAnAI = true;

			if ((nAI || count == 0) && !addABonus)
			{
				consecutiveAI++;
				nAI = true;
				if (sAIblank && count == 0)
					consecutiveAI += .1;
			}
			else
				AIGreaterThanOne++;
		}
		else if (gameState[searchCol][searchRow] == HUMAN_PIECE)
		{
			if (count == 1)
				nAI = false;
			break;
		}
		else
		{
			if (sawAnAI)
				addABonus = true;
			else
				nAIblank = true;
			possibleAI++;
		}
		searchRow++;
		count++;
	}
	if (addABonus)
		consecutiveAI += .1;
	if (sAI && nAIblank)
		consecutiveAI += .1;
	if (consecutiveAI + possibleAI + AIGreaterThanOne >= k && consecutiveAI > maxAI)
		maxAI = consecutiveAI;
	totalAIGreaterThanOne += AIGreaterThanOne;
}

void AIShell::chooseFinalMove(int col, int row, Move &finalMove, Move &offence, bool &useOffence, double &maxInterrupts, double &maxAI, double &mostInterrupts, double &mostAI, double &mostInterruptsGreaterThanOne, 
	double &mostAIGreaterThanOne, double &totalInterruptsGreaterThanOne, double &totalAIGreaterThanOne, int &mostSurrounding, int &mostSurroundingAI, int &surrounding, int &surroundingAI)
{
	Move m(col, row);

	if (maxAI >= k - 2 && maxInterrupts < k-1)//Take the winning chance
	{
		useOffence = true;
		offence = m;
	}
	else if (maxInterrupts >= k - 1)
	{
		useOffence = false;
	}

	if (maxInterrupts > mostInterrupts)	//UNLIMITED POWER
	{
		finalMove = m;
		mostInterrupts = maxInterrupts;
		goto comp2;
	}
	else if (maxInterrupts == mostInterrupts)
	{
	comp2:
		if (totalInterruptsGreaterThanOne + surrounding > mostInterruptsGreaterThanOne + mostSurrounding)
		{
			finalMove = m;
			mostInterruptsGreaterThanOne = totalInterruptsGreaterThanOne;
			mostSurrounding = surrounding;
			goto comp3;
		}
		else if (totalInterruptsGreaterThanOne + surrounding == mostInterruptsGreaterThanOne + mostSurrounding)
		{
		comp3:
			if (totalInterruptsGreaterThanOne > mostInterruptsGreaterThanOne)
			{
				finalMove = m;
				goto comp4;
			}
			else if (totalInterruptsGreaterThanOne == mostInterruptsGreaterThanOne)
			{
			comp4:
				if (maxAI > mostAI)
				{
					finalMove = m;
					mostAI = maxAI;
					goto comp5;
				}
				else if (maxAI == mostAI)
				{
				comp5:
					if (totalAIGreaterThanOne + surroundingAI > mostAIGreaterThanOne + mostSurroundingAI)
					{
						finalMove = m;
						mostAIGreaterThanOne = totalAIGreaterThanOne;
						mostSurroundingAI = surroundingAI;
						goto comp6;
					}
					else if (totalAIGreaterThanOne + surroundingAI == mostAIGreaterThanOne + mostSurroundingAI)
					{
					comp6:
						if (totalAIGreaterThanOne > mostAIGreaterThanOne)
						{
							finalMove = m;
						}
					}
				}
			}
		}
	}
}

bool AIShell::gravityFinalMove(Move &m, int col, int row, bool &aboveScan, bool &goToReturnFinalMove, Move &finalMove, double &maxInterrupts, double &maxAI, double &mostInterrupts, double &mostAI, 
	double &mostInterruptsGreaterThanOne, double &mostAIGreaterThanOne, double &totalInterruptsGreaterThanOne, double &totalAIGreaterThanOne, int &mostSurrounding, int &mostSurroundingAI, 
	int &surrounding, int &surroundingAI)
{
	if (maxAI >= k - 1 && !aboveScan)	//Take the winning chance
	{
		finalMove = m;
		goToReturnFinalMove = true;
		return false;
	}

	bool goToMoveWasChosen = false;

	if (maxInterrupts > mostInterrupts)	//UNLIMITED POWER
	{
		goToMoveWasChosen = true;
		mostInterrupts = maxInterrupts;
		goto gcomp2;
	}
	else if (maxInterrupts == mostInterrupts)
	{
	gcomp2:
		if (totalInterruptsGreaterThanOne + surrounding > mostInterruptsGreaterThanOne + mostSurrounding)
		{
			goToMoveWasChosen = true;
			mostInterruptsGreaterThanOne = totalInterruptsGreaterThanOne;
			mostSurrounding = surrounding;
			goto gcomp3;
		}
		else if (totalInterruptsGreaterThanOne + surrounding == mostInterruptsGreaterThanOne + mostSurrounding)
		{
		gcomp3:
			if (totalInterruptsGreaterThanOne > mostInterruptsGreaterThanOne)
			{
				goToMoveWasChosen = true;
				goto gcomp4;
			}
			else if (totalInterruptsGreaterThanOne == mostInterruptsGreaterThanOne)
			{
			gcomp4:
				if (maxAI > mostAI)
				{
					goToMoveWasChosen = true;
					mostAI = maxAI;
					goto gcomp5;
				}
				else if (maxAI == mostAI)
				{
				gcomp5:
					if (totalAIGreaterThanOne + surroundingAI > mostAIGreaterThanOne + mostSurroundingAI)
					{
						goToMoveWasChosen = true;
						mostAIGreaterThanOne = totalAIGreaterThanOne;
						mostSurroundingAI = surroundingAI;
						goto gcomp6;
					}
					else if (totalAIGreaterThanOne + surroundingAI == mostAIGreaterThanOne + mostSurroundingAI)
					{
					gcomp6:
						if (totalAIGreaterThanOne > mostAIGreaterThanOne)
						{
							goToMoveWasChosen = true;
						}
					}
				}
			}
		}
	}
	return goToMoveWasChosen;
}

bool AIShell::gravityMoveWasChosen(bool &aboveScan, int row, Move &finalMove, double &topMostInterrupts, double &mostInterrupts, double &topMostInterruptsGreaterThanOne, double &mostInterruptsGreaterThanOne, 
	int &topMostSurrounding, int &mostSurrounding, double &topMostAI, double &mostAI, double &topMostAIGreaterThanOne, double &mostAIGreaterThanOne, int &topMostSurroundingAI, int &mostSurroundingAI, 
	int &goodMoveCount, Move &m, std::vector<badMove> &badMoveVector, Move &previousFinalMove, double &previousMostInterrupts, double &previousMostInterruptsGreaterThanOne, 
	int &previousMostSurrounding, double &previousMostAI, double &previousMostAIGreaterThanOne, int &previousMostSurroundingAI)
{
	if (!aboveScan)		//first time move was chosen
	{
		if (row + 1 < numRows)	//if above is scannable
		{
			finalMove = m;//save the values of top move, but they may get changed back to previous later
			topMostInterrupts = mostInterrupts;
			topMostInterruptsGreaterThanOne = mostInterruptsGreaterThanOne;
			topMostSurrounding = mostSurrounding;
			topMostAI = mostAI;
			topMostAIGreaterThanOne = mostAIGreaterThanOne;
			topMostSurroundingAI = mostSurroundingAI;
			goodMoveCount++;

			row++;
			aboveScan = true;
			return true;
		}
		else                    //above is not scannable, so final move is this one
		{
			finalMove = m;
			goodMoveCount++;
			return false;
		}
	}
	else				//second time move was chosen (above is riskier than top)
	{
		//add to badMoveList
		badMove bm;
		bm.move = finalMove;
		bm.diffInterrupts = mostInterrupts - topMostInterrupts;
		bm.diffInterruptsGreaterThanOne = mostInterruptsGreaterThanOne - topMostInterruptsGreaterThanOne;
		bm.diffSurrounding = mostSurrounding - topMostSurrounding;
		bm.diffAI = mostAI - topMostAI;
		bm.diffAIGreaterThanOne = mostAIGreaterThanOne - topMostAIGreaterThanOne;
		bm.diffSurroundingAI = mostSurroundingAI - topMostSurroundingAI;
		badMoveVector.push_back(bm);

		finalMove = previousFinalMove;
		mostInterrupts = previousMostInterrupts;
		mostInterruptsGreaterThanOne = previousMostInterruptsGreaterThanOne;
		mostSurrounding = previousMostSurrounding;
		mostAI = previousMostAI;
		mostAIGreaterThanOne = previousMostAIGreaterThanOne;
		mostSurroundingAI = previousMostSurroundingAI;
		goodMoveCount--;
	}
	return false;
}

void AIShell::ifNoGoodMoves(std::vector<badMove> &badMoveVector, Move &finalMove)
{
	double maxDiffInterrupts = INFINITY, maxDiffInterruptsGreaterThanOne = INFINITY, maxDiffSurrounding = INFINITY;
	double maxDiffAI = INFINITY, maxDiffAIGreaterThanOne = INFINITY, maxDiffSurroundingAI = INFINITY;
	badMove searcher;

	for (unsigned int i = 0; i < badMoveVector.size(); i++)
	{
		if (badMoveVector[i].diffInterrupts < maxDiffInterrupts)
		{
			searcher = badMoveVector[i];
			maxDiffInterrupts = badMoveVector[i].diffInterrupts;
			goto crit2;
		}
		else if (badMoveVector[i].diffInterrupts == maxDiffInterrupts)
		{
		crit2:
			if (badMoveVector[i].diffInterruptsGreaterThanOne < maxDiffInterruptsGreaterThanOne)
			{
				searcher = badMoveVector[i];
				maxDiffInterruptsGreaterThanOne = badMoveVector[i].diffInterruptsGreaterThanOne;
				goto crit3;
			}
			else if (badMoveVector[i].diffInterruptsGreaterThanOne == maxDiffInterruptsGreaterThanOne)
			{
			crit3:
				if (badMoveVector[i].diffSurrounding < maxDiffSurrounding)
				{
					searcher = badMoveVector[i];
					maxDiffSurrounding = badMoveVector[i].diffSurrounding;
					goto crit4;
				}
				else if (badMoveVector[i].diffSurrounding == maxDiffSurrounding)
				{
				crit4:
					if (badMoveVector[i].diffAI < maxDiffAI)
					{
						searcher = badMoveVector[i];
						maxDiffAI = badMoveVector[i].diffAI;
						goto crit5;
					}
					else if (badMoveVector[i].diffAI == maxDiffAI)
					{
					crit5:
						if (badMoveVector[i].diffAIGreaterThanOne < maxDiffAIGreaterThanOne)
						{
							searcher = badMoveVector[i];
							maxDiffAIGreaterThanOne = badMoveVector[i].diffAIGreaterThanOne;
							goto crit6;
						}
						else if (badMoveVector[i].diffAIGreaterThanOne == maxDiffAIGreaterThanOne)
						{
						crit6:
							if (badMoveVector[i].diffSurroundingAI < maxDiffSurroundingAI)
							{
								searcher = badMoveVector[i];
								maxDiffSurroundingAI = badMoveVector[i].diffSurroundingAI;
							}
						}
					}
				}
			}
		}
	}

	finalMove = searcher.move;
}

Move AIShell::noGravityGame()
{
	double mostInterrupts = -1, mostInterruptsGreaterThanOne = -1;
	double mostAI = -1, mostAIGreaterThanOne = -1;
	int mostSurrounding = -1, mostSurroundingAI = -1;
	Move finalMove(numCols/2, numRows/2);
	Move offence(numCols/2, numRows/2);
	bool useOffence = false;

	for (int col = 0; col<numCols; col++)			//scan horizontally
	{
		for (int row = 0; row < numRows; row++)		//scan vertically
		{
			if (gameState[col][row] == NO_PIECE)	//top of the column, totals maxInterrupts
			{
				bool sw = false, w = false, nw = false, s = false, n = false, se = false, e = false, ne = false;
				bool swAI = false, wAI = false, nwAI = false, sAI = false, nAI = false, seAI = false, eAI = false, neAI = false;
				bool swblank = false, wblank = false, nwblank = false, sblank = false, nblank = false, seblank = false, eblank = false, neblank = false;
				bool swAIblank = false, wAIblank = false, nwAIblank = false, sAIblank = false, nAIblank = false, seAIblank = false, eAIblank = false, neAIblank = false;

				double maxInterrupts = 0, totalInterruptsGreaterThanOne = 0;//spot specific
				double maxAI = 0, totalAIGreaterThanOne = 0;				 //
				int surrounding = 0, surroundingAI = 0;

/////////////////////////////

				double consecutive = 0, interruptsGreaterThanOne = 0, possible = 1, consecutiveAI = 0, AIGreaterThanOne = 0, possibleAI = 1;

				swSearch(col, row, consecutive, interruptsGreaterThanOne, possible, sw, swblank);
				neSearch(col, row, consecutive, interruptsGreaterThanOne, possible, sw, swblank, ne, neblank, maxInterrupts, totalInterruptsGreaterThanOne);
				swAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, swAI, swAIblank);
				neAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, swAI, swAIblank, neAI, neAIblank, maxAI, totalAIGreaterThanOne);

/////////////////////////////

				consecutive = 0, interruptsGreaterThanOne = 0, possible = 1, consecutiveAI = 0, AIGreaterThanOne = 0, possibleAI = 1;

				wSearch(col, row, consecutive, interruptsGreaterThanOne, possible, w, wblank);
				eSearch(col, row, consecutive, interruptsGreaterThanOne, possible, w, wblank, e, eblank, maxInterrupts, totalInterruptsGreaterThanOne);
				wAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, wAI, wAIblank);
				eAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, wAI, wAIblank, eAI, eAIblank, maxAI, totalAIGreaterThanOne);

/////////////////////////////

				consecutive = 0, interruptsGreaterThanOne = 0, possible = 1, consecutiveAI = 0, AIGreaterThanOne = 0, possibleAI = 1;

				nwSearch(col, row, consecutive, interruptsGreaterThanOne, possible, nw, nwblank);
				seSearch(col, row, consecutive, interruptsGreaterThanOne, possible, nw, nwblank, se, seblank, maxInterrupts, totalInterruptsGreaterThanOne);
				nwAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, nwAI, nwAIblank);
				seAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, nwAI, nwAIblank, seAI, seAIblank, maxAI, totalAIGreaterThanOne);

/////////////////////////////

				consecutive = 0, interruptsGreaterThanOne = 0, possible = 1, consecutiveAI = 0, AIGreaterThanOne = 0, possibleAI = 1;

				sSearch(col, row, consecutive, interruptsGreaterThanOne, possible, s, sblank);
				nSearch(col, row, consecutive, interruptsGreaterThanOne, possible, s, sblank, n, nblank, maxInterrupts, totalInterruptsGreaterThanOne);
				sAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, sAI, sAIblank);
				nAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, sAI, sAIblank, nAI, nAIblank, maxAI, totalAIGreaterThanOne);

/////////////////////////////

				surrounding = sw + w + nw + s + n + se + e + ne;
				surroundingAI = swAI + wAI + nwAI + sAI + nAI + seAI + eAI + neAI;

				chooseFinalMove(col, row, finalMove, offence, useOffence, maxInterrupts, maxAI, mostInterrupts, mostAI, mostInterruptsGreaterThanOne, mostAIGreaterThanOne, totalInterruptsGreaterThanOne, totalAIGreaterThanOne, mostSurrounding, mostSurroundingAI, surrounding, surroundingAI);
			}
		}
	}//scan horizontally
	if (useOffence)
		return offence;

	return finalMove;
}

Move AIShell::gravityGame()
{
	double mostInterrupts = -1, mostInterruptsGreaterThanOne = -1;
	double mostAI = -1, mostAIGreaterThanOne = -1;
	int mostSurrounding = -1, mostSurroundingAI = -1;
	Move finalMove(numCols/2, numRows/2);

	double previousMostInterrupts = -1, previousMostInterruptsGreaterThanOne = -1;
	double previousMostAI = -1, previousMostAIGreaterThanOne = -1;
	int previousMostSurrounding = -1, previousMostSurroundingAI = -1;
	Move previousFinalMove(numCols / 2, numRows / 2);

	int goodMoveCount = 0;

	std::vector<badMove> badMoveVector;


	for (int col = 0; col<numCols; col++)			//scan horizontally
	{
		bool aboveScan = false;
		double topMostInterrupts = 0, topMostInterruptsGreaterThanOne = 0;
		double topMostAI = 0, topMostAIGreaterThanOne = 0;
		int topMostSurrounding = 0, topMostSurroundingAI = 0;

		for (int row = 0; row < numRows; row++)		//scan vertically
		{
			if (gameState[col][row] == NO_PIECE)	//top of the column, totals maxInterrupts
			{
			noPiece:

				bool sw = false, w = false, nw = false, s = false, n = false, se = false, e = false, ne = false;
				bool swAI = false, wAI = false, nwAI = false, sAI = false, nAI = false, seAI = false, eAI = false, neAI = false;
				bool swblank = false, wblank = false, nwblank = false, sblank = false, nblank = false, seblank = false, eblank = false, neblank = false;
				bool swAIblank = false, wAIblank = false, nwAIblank = false, sAIblank = false, nAIblank = false, seAIblank = false, eAIblank = false, neAIblank = false;

				double maxInterrupts = 0, totalInterruptsGreaterThanOne = 0;//spot specific
				double maxAI = 0, totalAIGreaterThanOne = 0;				 //
				int surrounding = 0, surroundingAI = 0;

				/////////////////////////////

				double consecutive = 0, interruptsGreaterThanOne = 0, possible = 1, consecutiveAI = 0, AIGreaterThanOne = 0, possibleAI = 1;

				swSearch(col, row, consecutive, interruptsGreaterThanOne, possible, sw, swblank);
				neSearch(col, row, consecutive, interruptsGreaterThanOne, possible, sw, swblank, ne, neblank, maxInterrupts, totalInterruptsGreaterThanOne);
				swAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, swAI, swAIblank);
				neAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, swAI, swAIblank, neAI, neAIblank, maxAI, totalAIGreaterThanOne);

				/////////////////////////////

				consecutive = 0, interruptsGreaterThanOne = 0, possible = 1, consecutiveAI = 0, AIGreaterThanOne = 0, possibleAI = 1;

				wSearch(col, row, consecutive, interruptsGreaterThanOne, possible, w, wblank);
				eSearch(col, row, consecutive, interruptsGreaterThanOne, possible, w, wblank, e, eblank, maxInterrupts, totalInterruptsGreaterThanOne);
				wAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, wAI, wAIblank);
				eAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, wAI, wAIblank, eAI, eAIblank, maxAI, totalAIGreaterThanOne);

				/////////////////////////////

				consecutive = 0, interruptsGreaterThanOne = 0, possible = 1, consecutiveAI = 0, AIGreaterThanOne = 0, possibleAI = 1;

				nwSearch(col, row, consecutive, interruptsGreaterThanOne, possible, nw, nwblank);
				seSearch(col, row, consecutive, interruptsGreaterThanOne, possible, nw, nwblank, se, seblank, maxInterrupts, totalInterruptsGreaterThanOne);
				nwAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, nwAI, nwAIblank);
				seAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, nwAI, nwAIblank, seAI, seAIblank, maxAI, totalAIGreaterThanOne);

				/////////////////////////////

				consecutive = 0, interruptsGreaterThanOne = 0, possible = 1, consecutiveAI = 0, AIGreaterThanOne = 0, possibleAI = 1;

				sSearch(col, row, consecutive, interruptsGreaterThanOne, possible, s, sblank);
				nSearch(col, row, consecutive, interruptsGreaterThanOne, possible, s, sblank, n, nblank, maxInterrupts, totalInterruptsGreaterThanOne);
				sAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, sAI, sAIblank);
				nAISearch(col, row, consecutiveAI, AIGreaterThanOne, possibleAI, sAI, sAIblank, nAI, nAIblank, maxAI, totalAIGreaterThanOne);

				/////////////////////////////

				surrounding = sw + w + nw + s + n + se + e + ne;
				surroundingAI = swAI + wAI + nwAI + sAI + nAI + seAI + eAI + neAI;

				Move m(col, row);
				bool goToReturnFinalMove = false;

				if (gravityFinalMove(m, col, row, aboveScan, goToReturnFinalMove, finalMove, maxInterrupts, maxAI, mostInterrupts, mostAI, mostInterruptsGreaterThanOne,
					mostAIGreaterThanOne, totalInterruptsGreaterThanOne, totalAIGreaterThanOne, mostSurrounding, mostSurroundingAI, surrounding, surroundingAI))
					goto moveWasChosen;

				if (goToReturnFinalMove)
					goto returnFinalMove;

				break;/////////////////////////////////////////////////////////////////////////////////////

			moveWasChosen:
				if (gravityMoveWasChosen(aboveScan, row, finalMove, topMostInterrupts, mostInterrupts, topMostInterruptsGreaterThanOne, mostInterruptsGreaterThanOne, topMostSurrounding, mostSurrounding, 
					topMostAI, mostAI, topMostAIGreaterThanOne, mostAIGreaterThanOne, topMostSurroundingAI, mostSurroundingAI, goodMoveCount, m, badMoveVector, previousFinalMove, 
					previousMostInterrupts, previousMostInterruptsGreaterThanOne, previousMostSurrounding, previousMostAI, previousMostAIGreaterThanOne, previousMostSurroundingAI))
					goto noPiece;

				break;////////////////////////////////////////////////////////////////////////////////////

			}//if no piece
		}//scan vertically	BREAK LEADS TO HERE

		previousFinalMove = finalMove;
		previousMostInterrupts = mostInterrupts;
		previousMostInterruptsGreaterThanOne = mostInterruptsGreaterThanOne;
		previousMostSurrounding = mostSurrounding;
		previousMostAI = mostAI;
		previousMostAIGreaterThanOne = mostAIGreaterThanOne;
		previousMostSurroundingAI = mostSurroundingAI;
	}//scan horizontally

	if (goodMoveCount == 0)
		ifNoGoodMoves(badMoveVector, finalMove);

returnFinalMove:
	return finalMove;
}
