#include "stdafx.h"
#include "RulesManager.h"
#include "State.h"
#include "Movements.h"
#include "Pieces.h"
#include <vector>

using namespace Common;
using namespace std;

namespace Utils
{
	// Se inicializa la variable estática responses
	RESPONSES RulesManager::response = VALID_OK;

	bool RulesManager::IsPlayerArea(int col, int row, ORIENTATION orientation) 
	{ 
		return ((orientation==RIGHT && boardAreas[col][row]<=AREA && row<7) 
			|| (orientation==LEFT && boardAreas[col][row]<=AREA && row>7));
	}

	bool RulesManager::IsLittleArea(int col, int row, ORIENTATION orientation)
	{
		return ((orientation==RIGHT && boardAreas[col][row]<=LITTLE && row<7) 
			|| (orientation==LEFT && boardAreas[col][row]<=LITTLE && row>7));	
	}

	bool RulesManager::IsSpecial(int col, int row, ORIENTATION orientation)
	{
		return ((orientation==RIGHT && (boardAreas[col][row]==CORNER || boardAreas[col][row]==SPECIAL) && row==13)
			|| (orientation==LEFT && (boardAreas[col][row]==CORNER || boardAreas[col][row]==SPECIAL) && row==1) ); 
	}

	bool RulesManager::IsValidPlayerMove(const State& state, const PlayerMovement* movement)
	{
		int level = state.GetLevel();
		const Board* board = state.GetBoard();

		/* En la primera jugada y despues de un gol, debe tomar la pelota si o si */
		if(state.FirstPly() && !IsBallAround(board, movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow())) 	
		{
			response = INVALID_GET_BALL; 
			return false;
		}
			
		/* Reglas basicas */
		if(!IsCorrectPlayerMove(board, movement))
		{
			response = INVALID_PLAYER_MOVE;
			return false;
		}
	
		// Indica que la pelota se encuentra en una casilla neutra
		bool neutralSquare = IsNeutralSquare(board, level, board->GetBall()->GetCol(), board->GetBall()->GetRow());

		/* Si esta en posesion de la pelota no puede moverse el jugador */ 
		if(IsBallAround(board, movement->GetPiece()->GetCol(), movement->GetPiece()->GetRow())
			&& neutralSquare==false)
		{
			response = INVALID_PLAYER_MOVE; 
			return false;
		}

		/* Reglas Nivel 2 y 3 */
		if(level > 1) 
		{
			/* Reglas Casilla Neutra */
			// TODO:
			if (neutralSquare)
			{
				/* Jugador no puede dejar su posicion si participa de una casilla neutra */
				if(IsBallAround(board, movement->GetPiece()->GetCol(),movement->GetPiece()->GetRow())
					&& !IsBallAround(board, movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow()))
				{
					response = INVALID_PLAYER_MOVE;
					return false;
				}

				/* Cuando un jugador puede apoderarse de una casilla neutra que esta en su campo, debe hacerlo */
				if(IsNeutralSquareInArea(board, movement))
				{
					response = INVALID_NEUTRAL_IN_AREA; 
					return false;
				}
			}
		}

		/* Al finalizar, verificamos si llego o no a la pelota */
		if(IsBallAround(board, movement->GetSquareAfter()->GetCol(), movement->GetSquareAfter()->GetRow()))
		{
							/* TODO: neutralSquare */
			if(level > 1 && neutralSquare)
			{
				/* Si habia casilla neutra y llego a la pelota, hay mayoria para el team que movio el jugador 
			      (si el jugador que se movio no participaba de la casilla neutra) */
				if(!IsBallAround(board, movement->GetPiece()->GetCol(), movement->GetPiece()->GetRow()) 
					&& IsBallAround(board, movement->GetSquareAfter()->GetCol(), movement->GetSquareAfter()->GetRow())) 
				{
					// TODO: neutralSquare=false;
					//lastPlayerNeutral=movement->GetPiece()->GetPlayerId(); //probar esto
					//lastPlayer=0;
					response = VALID_HAS_BALL;
					return true;
				}
				else if(IsBallAround(board, movement->GetPiece()->GetCol(), movement->GetPiece()->GetRow())
					&& IsBallAround(board, movement->GetSquareAfter()->GetCol(), movement->GetSquareAfter()->GetRow()))
				{
					/* Si se movio un jugador que participa de la casilla neutra, pero se movio a otra casilla 
			           donde tambien participa de la misma casilla neutra, sigue habiendo casilla neutra*/
					//lastPlayerNeutral=0;
					//lastPlayer=0;

					response = VALID_OK;
					return true;
				}			
			}
			response = VALID_HAS_BALL;
			return true;
		}
		//else /* Si no llego a la pelota, termina su turno */
			response = VALID_OK;
		return true;
	}

	bool RulesManager::IsBallAround(const Board* board, int col, int row)
	{
		const BallPiece* ball = board->GetBall();
		return ( IsAround(ball->GetCol(), ball->GetRow(), col, row) );
	}

	//Reglas basicas
	bool RulesManager::IsCorrectPlayerMove(const Board* board, const PlayerMovement* movement)
	{	
		int colBefore, rowBefore, colAfter, rowAfter;	
		int level = board->GetNPieces();
		// Si hay 5 piezas el nivel es 3
		if (level == 5)
			level = 3;
		// Si hay 3 piezas el nivel es 4
		else if (level == 3)
			level = 4;
		// else level == 2 o level == 1

		colBefore = movement->GetPiece()->GetCol();
		rowBefore = movement->GetPiece()->GetRow();
		colAfter = movement->GetSquareAfter()->GetCol();
		rowAfter = movement->GetSquareAfter()->GetRow();

		/* El jugador se puede mover hasta 2 casillas */
		if(abs(colBefore-colAfter)>2 || abs(rowBefore-rowAfter)>2) 
			return false; 

		/* El jugador no puede saltar sobre ninguna ficha */
		if(colBefore == colAfter) //si se mueve hacia la derecha o hacia la izquierda (avanza o retrocede)
		{					
			if((rowAfter>rowBefore) && (abs(rowAfter-rowBefore) != 1))	//si se mueve hacia la derecha 2 casillas
			{
				if(IsPieceInSquare(board, colBefore,rowAfter-1)) //si hay alguna pieza en esa casilla intermedia
					return false; //movimiento no valido
			}
			else if((rowBefore>rowAfter) && (abs(rowAfter-rowBefore) != 1)) //si se mueve hacia la izquierda 2 casillas
			{
				if(IsPieceInSquare(board, colBefore,rowAfter+1)) 
					return false;
			}
		}
		else if(rowBefore == rowAfter) //si se mueve hacia arriba o hacia abajo (hacia los costados)
		{
			if((colAfter > colBefore) && (abs(colAfter-colBefore)!=1)) //si se mueve hacia abajo 2 casillas
			{
				if(IsPieceInSquare(board, colAfter-1,rowBefore)) 
					return false;
			}
			else if((colBefore > colAfter) && (abs(colAfter-colBefore)!=1)) //si se mueve hacia arriba 2 casillas
			{
				if(IsPieceInSquare(board, colAfter+1,rowBefore)) 
					return false;
			}
		}	
		else //si se mueve en diagonal
		{	
			/* Movimiento en L no es valido */
			if(abs(colBefore-colAfter) != abs(rowBefore-rowAfter)) //movimiento diagonal, diferencias entre i's y j's iguales
				return false;

			if(abs(colBefore-colAfter)!=1) //si se mueve 2 casillas, no 1
			{
				if(colBefore>colAfter && rowBefore<rowAfter) //hacia arriba der
				{
					if(IsPieceInSquare(board, colAfter+1,rowAfter-1)) 
						return false;
				}
				else if(colBefore>colAfter && rowBefore>rowAfter) //hacia arriba izq
				{
					if(IsPieceInSquare(board, colAfter+1,rowAfter+1)) 
						return false;
				}
				else if(colBefore<colAfter && rowBefore<rowAfter) //hacia abajo der
				{
					if(IsPieceInSquare(board, colAfter-1,rowAfter-1)) 
						return false;
				}
				else if(colBefore<colAfter && rowBefore>rowAfter) //hacia abajo izq
				{
					if(IsPieceInSquare(board, colAfter-1,rowAfter+1)) 
						return false;
				}
			}		
		}

		/* No se puede mover a una casilla ocupada por otra ficha */
		if(IsPieceInSquare(board, colAfter,rowAfter)) 
			return false;

		/* No moverse fuera de las dimensiones del tablero (el usuario tal vez no, pero la inteligencia si) */
		if(colAfter<0 || colAfter>10 || rowAfter<1 || rowAfter>13) 
			return false;

		/* Reglas Nivel 2 y 3 */
		if(level > 1) 
			/* Jugador no puede ocupar casilla de corner propia (pero si puede apoderarse de la pelota en esa casilla) */
			if(IsCorner(colAfter,rowAfter,((PlayerPiece*)movement->GetPiece())->GetOrientation())
				&& !IsBallAround(board, colAfter,rowAfter))		
				return false;

		/* Reglas Nivel 3 */
		if(level > 2)
		{
			ORIENTATION opponent = (((PlayerPiece*)movement->GetPiece())->GetOrientation() == LEFT) ? RIGHT : LEFT; 
			ORIENTATION team = ((PlayerPiece*)movement->GetPiece())->GetOrientation();
			if(((PlayerPiece*)movement->GetPiece())->GetId()==ID_G //si se esta moviendo el arquero...
				&& IsBigArea(colAfter, rowAfter, team)) 
			{
				if ((IsPieceInSquare(board, colAfter-1,rowAfter) && !(colBefore == colAfter-1 && rowBefore == rowAfter)
					&& IsBigArea(colAfter-1, rowAfter, team) 
						&& (board->GetBall()->GetCol()!=colAfter-1 || board->GetBall()->GetRow()!=rowAfter))
					|| 
					(IsPieceInSquare(board, colAfter+1, rowAfter) && !(colBefore == colAfter+1 && rowBefore == rowAfter)
					&& IsBigArea(colAfter+1, rowAfter, team) 
					&& (board->GetBall()->GetCol()!=colAfter+1 || board->GetBall()->GetRow()!=rowAfter)))
					return false;
				/*if(colBefore==colAfter && IsPieceInSquare(board, colAfter-1,rowAfter) 
					&& (board->GetBall()->GetCol()!=colAfter-1 || board->GetBall()->GetRow()!=rowAfter))
					return false;
				else if(colBefore==colAfter && IsPieceInSquare(board, colAfter+1,rowAfter) 
					&& (board->GetBall()->GetCol()!=colAfter+1 || board->GetBall()->GetRow()!=rowAfter))
					return false;
				else if(colAfter<colBefore && IsPieceInSquare(board, colAfter-1,rowAfter) 
					&& (board->GetBall()->GetCol()!=colAfter-1 || board->GetBall()->GetRow()!=rowAfter)) 
					return false;
				else if(colAfter>colBefore && IsPieceInSquare(board, colAfter+1,rowAfter)
					&& (board->GetBall()->GetCol()!=colAfter+1 || board->GetBall()->GetRow()!=rowAfter))
					return false;*/
			}
			else
			{
				const PlayerPiece* opKeeper = board->GetPlayer(opponent, ID_G);
				const PlayerPiece* keeper = board->GetPlayer(((PlayerPiece*)movement->GetPiece())->GetOrientation(), ID_G);
				/* Ningun jugador puede colocarse en las casillas ocupadas por los brazos del arquero */
				if (IsBigArea(colAfter, rowAfter, opponent) 
					&& IsBigArea(opKeeper->GetCol(), opKeeper->GetRow(), opponent) 
					&& rowAfter == opKeeper->GetRow() 
					&& (colAfter + 1 == opKeeper->GetCol() || colAfter - 1 == opKeeper->GetCol()))
					return false;

				if (IsBigArea(colAfter, rowAfter, keeper->GetOrientation()) 
					&& IsBigArea(keeper->GetCol(), keeper->GetRow(), keeper->GetOrientation()) 
					&& rowAfter == keeper->GetRow() 
					&& (colAfter + 1 == keeper->GetCol() || colAfter - 1 == keeper->GetCol()))
					return false;
				/*if(IsGoalkeeper(board, colAfter,rowAfter,((PlayerPiece*)movement->GetPiece())->GetOrientation())!=0 
					|| IsGoalkeeper(board, colAfter,rowAfter,opponent)!=0)
					return false;*/
			}		
		}
		return true;
	}

	bool RulesManager::IsOpponentInSquare(const Board* board, int col, int row, ORIENTATION orientation)
	{
		PlayerPiece** opPieces = NULL;
		if (orientation == LEFT)
			opPieces = board->GetRightPieces();
		else
			opPieces = board->GetLeftPieces();

		int nPieces = board->GetNPieces();
		for (int i = 0; i < nPieces; i++)
		{
			if (opPieces[i]->GetCol() == col 
				&& opPieces[i]->GetRow() == row)
				return true;
		}
		return false;
	}

	bool RulesManager::IsNeutralSquare(const Board* board, int level, int colBall, int rowBall)
	{
		int teamRight=0, teamLeft=0;

		/* Si la pelota esta en una casilla contigua a los brazos del arquero.. esta pertenece si o si al arquero */
		/* Si un brazo del arquero esta contiguo a la pelota, la pelota esta en el area grande y el arquero esta en su area grande
		   no hay casilla neutra */	
		if(level>2)
		{
			if(board->GetPlayer(LEFT, ID_G)->GetRow() == rowBall 
				&& (board->GetPlayer(LEFT, ID_G)->GetCol()+1==colBall || board->GetPlayer(LEFT, ID_G)->GetCol()-1==colBall)
				&& IsBigArea(board->GetPlayer(LEFT, ID_G)->GetCol(), board->GetPlayer(LEFT, ID_G)->GetRow(), LEFT)
				&& IsBigArea(colBall, rowBall, LEFT))
				return false; 
			else if(board->GetPlayer(RIGHT, ID_G)->GetRow()==rowBall 
				&& (board->GetPlayer(RIGHT, ID_G)->GetCol()+1==colBall || board->GetPlayer(RIGHT, ID_G)->GetCol()-1==colBall)
				&& IsBigArea(board->GetPlayer(RIGHT, ID_G)->GetCol(), board->GetPlayer(RIGHT, ID_G)->GetRow(), RIGHT)
				&& IsBigArea(colBall,rowBall, RIGHT))
				return false;
		}

		PlayerPiece** players = board->GetRightPieces();
		int nPieces = board->GetNPieces();
		for (int i = 0; i < nPieces; i++)
		{
			if (IsAround(players[i]->GetCol(), players[i]->GetRow(), colBall, rowBall))
				teamRight++;
		}
		players = board->GetLeftPieces();
		for (int i = 0; i < nPieces; i++)
		{
			if (IsAround(players[i]->GetCol(), players[i]->GetRow(), colBall, rowBall))
				teamLeft++;
		}

		if(teamRight>0 && teamLeft>0 && teamRight==teamLeft)
			return true;

		return false;	

	}

	//verifica si hay casilla neutra en el area del jugador y si este puede llegar a la pelota
	//Se controla:
	// - que haya casilla neutra en el area del jugador
	// - que el jugador pueda llegar a la pelota -> IsCorrectPlayerMove para cada casilla alrededor de pelota
	// - que esa casilla valida no sea la casilla a donde se quiere ir el jugador
	bool RulesManager::IsNeutralSquareInArea(const Board* board, const PlayerMovement* movement)
	{
		bool inArea=false;
		int colBall, rowBall, colBefore, rowBefore;

		//coordenadas de la casilla a donde quiere ir el usuario
		colBefore = movement->GetPiece()->GetCol(); 
		rowBefore = movement->GetPiece()->GetRow();
	
		if(IsPlayerArea(board->GetBall()->GetCol(), board->GetBall()->GetRow(), ((PlayerPiece*)movement->GetPiece())->GetOrientation()))
		{
			inArea = true;
			colBall = board->GetBall()->GetCol();
			rowBall = board->GetBall()->GetRow();
		}
		
		//verificamos que el jugador pueda llegar con un movimiento valido
		if(inArea)
		{
			//si en la casilla, a donde se quiere mover el jugador, no se apodera de la pelota 
			//(que esta en casilla neutra), entonces buscamos casillas alrededor de la pelota....
			if(!IsBallAround(board, movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow())
				|| IsAround(board->GetBall()->GetCol(), board->GetBall()->GetRow(), movement->GetSquareBefore()->GetCol(), movement->GetSquareBefore()->GetRow()))
			{
				const PlayerPiece* piece;
				PlayerMovement *move = new PlayerMovement(*movement);
				int nPieces = board->GetNPieces();
				PlayerPiece** pieces;
				if (((PlayerPiece*)movement->GetPiece())->GetOrientation() == LEFT)
					pieces = board->GetLeftPieces();
				else
					pieces = board->GetRightPieces();

				for (int i = 0; i < nPieces; i++)
				{
					piece = pieces[i];

					// Si la ficha ya estaba alrededor de la pelota no puede apoderarse de ella
					if (IsBallAround(board, piece->GetCol(), piece->GetRow()))
						continue;

					move->SetPlayer(piece->GetOrientation(), piece->GetId());
					move->SetSquareBefore(*piece->GetPosition());
	
					move->SetSquareAfter(rowBall-1, colBall-1); //arriba, izq
					if(IsCorrectPlayerMove(board, move)) 	
					{
						delete move; move = NULL;
						return true;
					}
					
					move->SetSquareAfter(rowBall, colBall-1); //arriba, medio				
					if(IsCorrectPlayerMove(board, move))
					{
						delete move; move = NULL;
						return true;
					}

					move->SetSquareAfter(rowBall+1, colBall-1); //arriba, der
					if(IsCorrectPlayerMove(board, move))
					{
						delete move; move = NULL;
						return true;
					}

					move->SetSquareAfter(rowBall-1, colBall); //medio, izq
					if(IsCorrectPlayerMove(board, move))
					{
						delete move; move = NULL;
						return true;
					}

					move->SetSquareAfter(rowBall+1, colBall); //medio, der
					if(IsCorrectPlayerMove(board, move))
					{
						delete move; move = NULL;
						return true;
					}

					move->SetSquareAfter(rowBall-1, colBall+1); //abajo, izq
					if(IsCorrectPlayerMove(board, move))
					{
						delete move; move = NULL;
						return true;
					}

					move->SetSquareAfter(rowBall, colBall+1); //abajo, medio
					if(IsCorrectPlayerMove(board, move))
					{
						delete move; move = NULL;
						return true;
					}

					move->SetSquareAfter(rowBall+1, colBall+1); //abajo, der
					if(IsCorrectPlayerMove(board, move))
					{
						delete move; move = NULL;
						return true;
					}
				}				
				delete move; move = NULL;
			}
		}
		return false;
	}

	int RulesManager::IsGoalkeeper(const Common::Board *board, int col, int row, Common::ORIENTATION orientation)
	{
		const PlayerPiece* keeper = board->GetPlayer(orientation, ID_G);
		if(keeper->GetRow()==row) //misma fila
		{
			if(IsLittleArea(col,row,orientation))
			{
				if(keeper->GetCol()==col)
					return 1; //el cuerpo del arquero ocupa la casilla col,row y esta en el area chica
				if(keeper->GetCol()+1==col || keeper->GetCol()-1==col)
					return 2; //uno de los brazos del arquero ocupa la casilla i,j y esta en el area chica
			}
			else if(IsBigArea(col,row,orientation))
			{
				if(keeper->GetCol()==col)
					return 1; //el cuerpo del arquero ocupa la casilla i,j y esta en el area grande, fuera del area chica
				// TODO: recien agregue, probar
				if((keeper->GetCol()+1==col || keeper->GetCol()-1==col)
					&& IsBigArea(keeper->GetCol(), keeper->GetRow(), orientation))
					return 2; //uno de los brazos del arquero ocupa la casilla i,j y esta en el area grande
			}		
		}
		return 0; //ni el cuerpo ni los brazos del arquero ocupan la casilla i,j	
	}

	bool RulesManager::IsValidBallMove(const State& state, const BallMovement *movement) 
	{
		if (state.GetNKicks() >= 4)
			return false;
			
		int colBefore, rowBefore, colAfter, rowAfter;
		int level = state.GetLevel();
		const Board* board = state.GetBoard();
		colBefore = movement->GetSquareBefore()->GetCol();
		rowBefore = movement->GetSquareBefore()->GetRow();
		colAfter = movement->GetSquareAfter()->GetCol();
		rowAfter = movement->GetSquareAfter()->GetRow();

		/* Buscamos el jugador o team que esta moviendo la pelota */
		//piecePlaying = PiecePlaying(colBefore,rowBefore);	
		const PlayerPiece *piecePlaying = state.GetPieceWithBall();

		/* Si no hay ningun jugador contiguo a la pelota, esta no se puede mover */
		if (piecePlaying == NULL)
		{
			response = INVALID_FREE_BALL; 
			return false;
		}

		ORIENTATION majority = Majority(board, colAfter, rowAfter); 

		/* En la primera jugada o despues de un gol, la pelota no se puede dejar en el area del jugador */
		if(state.FirstPly()
			&& IsPlayerArea(movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow(),piecePlaying->GetOrientation())
			&& majority!=piecePlaying->GetOrientation())
		{
			response = INVALID_LEAVE_BALL;
			return false;
		}

		/* La pelota no se puede mover mas de 4 casillas */
		if(abs(colBefore-colAfter)>4 || abs(rowBefore-rowAfter)>4) 
		{
			response = INVALID_MOVE; 
			return false;
		}

		/* Movimiento en L no valido */
		if(colBefore!=colAfter && rowBefore!=rowAfter) //si no mueve en linea recta
			if(abs(colBefore-colAfter) != abs(rowBefore-rowAfter)) //movimiento diagonal, diferencias entre i's y j's iguales
			{
				response = INVALID_MOVE; 	
				return false;
			}

		/* La pelota no se puede enviar a una casilla ocupada por otra ficha */
		if(IsPieceInSquare(board,colAfter,rowAfter)) 
		{
			response = INVALID_MOVE; 
			return false;
		}

		/* No enviar la pelota fuera de las dimensiones del tablero (el usuario tal vez no, pero la inteligencia si) */
		if(colAfter<0 || colAfter>10 || rowAfter<0 || rowAfter>14) 
		{
			response = INVALID_MOVE;
			return false;
		}
		
		if((colAfter<3 || colAfter>7) && (rowAfter==0 || rowAfter==14))
		{
			response = INVALID_MOVE;
			return false;
		}

		/* No se pueden hacer autopases */
		if(IsAutopass(board, colAfter, rowAfter, piecePlaying))
		{
			response = INVALID_AUTOPASS; 
			return false;
		}
		
		// TODO: cuando es gol si puede estar al lado del contrario
		/* No se puede entregar la pelota al contrario */
		if(majority != piecePlaying->GetOrientation() && majority != NONE 
			&& !IsGoal(colAfter,rowAfter,piecePlaying->GetOrientation()))
		{
			response = INVALID_PASS_OPPONENT;  
			return false;
		}

		/* No se puede enviar la pelota al propio corner */
		if(IsCorner(movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow(),piecePlaying->GetOrientation()))
		{
			response = INVALID_BALL_CORNER;
			return false;
		}

		/* La pelota no se puede dejar en el area grande del jugador */
		if(IsBigArea(movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow(),piecePlaying->GetOrientation())
			&& majority!=piecePlaying->GetOrientation()
			&& IsGoalkeeper(state.GetBoard(), movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow(), piecePlaying->GetOrientation()) != 2)
		{
			response = INVALID_LEAVE_BALL;
			return false;
		}

		/* No puede meter gol en contra */
		if(IsAutogoal(movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow(),piecePlaying->GetOrientation()))
		{
			response = INVALID_AUTOGOAL;	
			return false;
		}

		/* Reglas Nivel 3 y 4 */
		if(level>2)
		{
			ORIENTATION opponentOrient = (piecePlaying->GetOrientation()==RIGHT) ? LEFT : RIGHT;
			//TEAM opponentTeam = (piecePlaying->GetTeam()==TEAM_GUI) ? TEAM_PC : TEAM_GUI;
			int col,row;

			/* No pasarle la pelota a los brazos del arquero, pase al contrario
			  (esto para cuando "aparenta" ser una casilla neutra, pero no es porque la pelota siempre es del arquero
			   cuando esta al lado de uno de sus brazos) */
			if(board->GetPlayer(opponentOrient, ID_G)->GetRow()==rowAfter && 
				(board->GetPlayer(opponentOrient, ID_G)->GetCol()+1==colAfter || board->GetPlayer(opponentOrient, ID_G)->GetCol()-1==colAfter)
				&& IsBigArea(board->GetPlayer(opponentOrient, ID_G)->GetCol(), board->GetPlayer(opponentOrient, ID_G)->GetRow(), opponentOrient)
				&& IsBigArea(colAfter, rowAfter, opponentOrient))
			{
				response = INVALID_PASS_OPPONENT;
				return false;
			}

			/* Reglas relacionadas con el arquero */
			if(colBefore==colAfter) //Si la pelota se mueve en linea recta hacia adelante o hacia atras...
			{				
				if(rowBefore<rowAfter) //se mueve de izquierda a derecha
				{
					for(row=rowBefore+1; row<=rowAfter-1; row++) 
					{
						/* La pelota no puede pasar por encima de los jugadores defensores en el area chica */
						if(IsOpponentInSquare(board,colAfter,row,piecePlaying->GetOrientation()) && IsLittleArea(colAfter,row,opponentOrient))
						{
							response = INVALID_LITTLE_AREA_DEFENSE;
							return false;
						}
						// TODO: area chica nomas?
						/* La pelota no puede pasar por encima de los brazos del arquero contrario en el area chica */
						if(IsGoalkeeper(board,colAfter,row,opponentOrient)!=0) 
						{
							response = INVALID_OVER_KEEPER;
							return false;
						}
						/* La pelota no puede pasar por encima del cuerpo del arquero en el area grande, ni en los pases */
						if(IsGoalkeeper(board,colAfter,row,opponentOrient)==1 || IsGoalkeeper(board,colAfter,row,piecePlaying->GetOrientation())==1)
						{
							response = INVALID_OVER_KEEPER;
							return false;
						}
					}
				}
				else //se mueve de derecha a izquierda
				{
					for(row=rowBefore-1; row>=rowAfter+1; row--)
					{
						if(IsOpponentInSquare(board,colAfter,row,piecePlaying->GetOrientation()) && IsLittleArea(colAfter,row,opponentOrient))
						{
							response = INVALID_LITTLE_AREA_DEFENSE;
							return false;
						}
						if(IsGoalkeeper(board,colAfter,row,opponentOrient)!=0) 
						{
							response = INVALID_OVER_KEEPER;
							return false;
						}
						if(IsGoalkeeper(board,colAfter,row,opponentOrient)==1 || IsGoalkeeper(board,colAfter,row,piecePlaying->GetOrientation())==1)
						{
							response = INVALID_OVER_KEEPER;
							return false;
						}
					}
				}		
			}
			else if(rowBefore==rowAfter) //Si la pelota se mueve en linea recta hacia los costados...
			{
				if(colBefore<colAfter) //se mueve de arriba hacia abajo
				{
					for(col=colBefore+1; col<=colAfter-1; col++)
					{
						if(IsOpponentInSquare(board,col,rowAfter,piecePlaying->GetOrientation()) && IsLittleArea(col,rowAfter,opponentOrient))
						{
							response = INVALID_LITTLE_AREA_DEFENSE;
							return false;
						}
						if(IsGoalkeeper(board,col,rowAfter,opponentOrient)!=0) 
						{
							response = INVALID_OVER_KEEPER;
							return false;
						}
						if(IsGoalkeeper(board,col,rowAfter,opponentOrient)==1 || IsGoalkeeper(board,col,rowAfter,piecePlaying->GetOrientation())==1)
						{
							response = INVALID_OVER_KEEPER;
							return false;
						}
					}
				}
				else //se mueve de abajo hacia arriba
				{
					for(col=colBefore-1; col>=colAfter+1; col--)
					{
						if(IsOpponentInSquare(board,col,rowAfter,piecePlaying->GetOrientation()) && IsLittleArea(col,rowAfter,opponentOrient))
						{
							response = INVALID_LITTLE_AREA_DEFENSE;
							return false;
						}
						if(IsGoalkeeper(board,col,rowAfter,opponentOrient)!=0) 
						{
							response = INVALID_OVER_KEEPER;
							return false;
						}
						if(IsGoalkeeper(board,col,rowAfter,opponentOrient)==1 || IsGoalkeeper(board,col,rowAfter,piecePlaying->GetOrientation())==1)
						{
							response = INVALID_OVER_KEEPER;
							return false;
						}
					}
				}			
			}
			else //Si la pelota se mueve en diagonal...
			{
				if(colBefore>colAfter) //hacia arriba
				{
					if(rowBefore<rowAfter) //hacia arriba derecha
					{
						//disminuye col, aumenta row
						col=colBefore-1;
						for(row=rowBefore+1; row<=rowAfter-1; row++)
						{
							if(IsOpponentInSquare(board,col,row,piecePlaying->GetOrientation()) && IsLittleArea(col,row,opponentOrient))
							{
								response = INVALID_LITTLE_AREA_DEFENSE;
								return false;
							}
							if(IsGoalkeeper(board,col,row,opponentOrient)!=0) 
							{
								response = INVALID_OVER_KEEPER;
								return false;
							}
							if(IsGoalkeeper(board,col,row,opponentOrient)==1 || IsGoalkeeper(board,col,row,piecePlaying->GetOrientation())==1)
							{
								response = INVALID_OVER_KEEPER;
								return false;
							}
							col--;
						}
					}
					else //hacia arriba izquierda
					{
						//disminuye col, disminuye row
						col=colBefore-1;
						for(row=rowBefore-1; row>=rowAfter+1; row--)
						{
							if(IsOpponentInSquare(board,col,row,piecePlaying->GetOrientation()) && IsLittleArea(col,row,opponentOrient))
							{
								response = INVALID_LITTLE_AREA_DEFENSE;
								return false;
							}
							if(IsGoalkeeper(board,col,row,opponentOrient)!=0) 
							{
								response = INVALID_OVER_KEEPER;
								return false;
							}
							if(IsGoalkeeper(board,col,row,opponentOrient)==1 || IsGoalkeeper(board,col,row,piecePlaying->GetOrientation())==1)
							{
								response = INVALID_OVER_KEEPER;
								return false;
							}
							col--;						
						}					
					}
				}
				else //hacia abajo
				{
					if(rowBefore<rowAfter) //hacia abajo derecha
					{
						//aumenta col, aumenta row
						col=colBefore+1;
						for(row=rowBefore+1; row<=rowAfter-1; row++)
						{
							if(IsOpponentInSquare(board,col,row,piecePlaying->GetOrientation()) && IsLittleArea(col,row,opponentOrient))
							{
								response = INVALID_LITTLE_AREA_DEFENSE;
								return false;
							}
							if(IsGoalkeeper(board,col,row,opponentOrient)!=0) 
							{
								response = INVALID_OVER_KEEPER;
								return false;
							}
							if(IsGoalkeeper(board,col,row,opponentOrient)==1 || IsGoalkeeper(board,col,row,piecePlaying->GetOrientation())==1)
							{
								response = INVALID_OVER_KEEPER;
								return false;
							}
							col++;						
						}	
					}
					else //hacia abajo izquierda
					{
						//aumenta col, disminuye row
						col=colBefore+1;
						for(row=rowBefore-1; row>=rowAfter+1; row--)
						{
							if(IsOpponentInSquare(board,col,row,piecePlaying->GetOrientation()) && IsLittleArea(col,row,opponentOrient))
							{
								response = INVALID_LITTLE_AREA_DEFENSE;
								return false;
							}
							if(IsGoalkeeper(board,col,row,opponentOrient)!=0) 
							{
								response = INVALID_OVER_KEEPER;
								return false;
							}
							if(IsGoalkeeper(board,col,row,opponentOrient)==1 || IsGoalkeeper(board,col,row,piecePlaying->GetOrientation())==1)
							{
								response = INVALID_OVER_KEEPER;
								return false;
							}
							col++;						
						}					
					}				
				}
			}	
		}

		bool neutralSquare = false;
		/* Reglas Nivel 2 y 3 */
		if(level > 1) 
		{			
			//Casilla Neutra antes de mover la pelota...
			/* La pelota no se puede mover si se encuentra en casilla neutra */
			// Indica que la pelota se encuntra en una casilla neutra
			neutralSquare = IsNeutralSquare(board, level, board->GetBall()->GetCol(), board->GetBall()->GetRow());
			// TODO: en este caso state.PlayerWithBall() 
			// va a ser NULL luego?
			// Como controlar que al crear un nuevo estado sucesor
			// playerWithBall sea asignado correctamente?
			// Play: conjunto de Movement? o tiene alguna otra info,
			// conjunto de estados?
			// CONTINUARÁ...
			if(neutralSquare)
			{
				response = INVALID_BALL_NEUTRAL; 
				return false;
			}

			//Casilla Neutra despues de mover la pelota...
			if(IsNeutralSquare(board, level, colAfter, rowAfter)) 
			{
				/* Un jugador no puede dejar la pelota en una casilla neutra en la que el participa */
				if(IsAround(colAfter,rowAfter,piecePlaying->GetCol(),piecePlaying->GetRow()))
				{
					response = INVALID_AUTOPASS;
					return false;
				}

				/* La pelota no se puede dejar en casilla neutra dentro del area grande del jugador */
				if(IsBigArea(movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow(),piecePlaying->GetOrientation()))
				{
					response = INVALID_LEAVE_BALL;
					return false;
				}
			}
		}

		if(level>1)
		{
			if(IsNeutralSquare(board, level, colAfter,rowAfter)) 
			{
				/* ...si se deja la pelota en casilla neutra */
				// TODO:
				//neutralSquare=true; //seteo a true casilla neutra para la siguiente jugada
				//isGoal=false; //por si en la primera jugada ya hay casilla neutra..
				//lastPlayer=0; 
			//	lastPlayerNeutral=0;
			//	response = VALID_NEUTRAL;
				response = (IsSpecialSquare(*movement->GetSquareAfter(), piecePlaying->GetOrientation(), level)) ? VALID_NEUTRAL_REPEAT : VALID_NEUTRAL;
				return true;
			}	
			else if(!IsGoal(colAfter,rowAfter,piecePlaying->GetOrientation())
				&& (majority==piecePlaying->GetOrientation()
					|| (level > 2 && IsGoalkeeper(board, colAfter, rowAfter, state.GetTurn()) == 2)))
			{
				/* ...si se hizo un pase */			
				// TODO:
				//lastPlayer = this->MapId(piecePlaying->GetTeam(),piecePlaying->GetPlayerId()); //guardar el id del jugador que hizo el pase
				//lastPlayerNeutral=piecePlaying->GetPlayerId();
				//lastPlayerNeutral=0;
				response = VALID_PASS; 
				return true;
			} 
		}


		if(level>2)
		{
			if(IsSpecial(movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow(),piecePlaying->GetOrientation())) //nivel 2a y 2b??
			{
				/* ...si se llego a casilla especial */
				// TODO:
				//lastPlayer=0; 
				//lastPlayerNeutral=0;
				response = VALID_REPEAT_TURN; //SOLO NIVEL 3
				return true;
			}
		}
		 
		if(IsGoal(movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow(),piecePlaying->GetOrientation()))
		{
			/* ...si metio gol */
			// TODO:
			//isGoal=true;
			//lastPlayer=0; 
			//lastPlayerNeutral=0;
			response = VALID_GOAL; 
			return true;
		}	
		else 
		{	 
			//isGoal=false;
			//lastPlayer=0;
			//lastPlayerNeutral=0;
			response = VALID_OK; /* Se dejo la pelota libre y termina el turno del jugador */
			return true;
		}
	
	} //IsValidBallMove()

	ORIENTATION RulesManager::Majority(const Board* board, int colBall, int rowBall)
	{
		int teamRight=0, teamLeft=0;
		int nPieces = board->GetNPieces();

		PlayerPiece** pieces = board->GetLeftPieces();
		for (int i = 0; i < nPieces; i++)
		{
			if (IsAround(pieces[i]->GetCol(), pieces[i]->GetRow(), colBall, rowBall))
				teamLeft++;
		}

		pieces = board->GetRightPieces();
		for (int i = 0; i < nPieces; i++)
		{
			if (IsAround(pieces[i]->GetCol(), pieces[i]->GetRow(), colBall, rowBall))
				teamRight++;
		}

		if (teamLeft > teamRight)
			return LEFT;
		else if (teamRight > teamLeft)
			return RIGHT;

		return NONE;
	}

	// TODO: al final no se usa
	const PlayerPiece* RulesManager::PiecePlaying(const State& state, int colBall, int rowBall) 
	{
		const Board* board = state.GetBoard();
		ORIENTATION teamPlaying = state.GetTurn();
		//ORIENTATION teamPlaying = Majority(board, colBall,rowBall);
		PlayerPiece **pieces;
		if (teamPlaying == LEFT)
			pieces = board->GetLeftPieces();
		else
			pieces = board->GetRightPieces();

		int nPieces = board->GetNPieces();
		for (int i = 0; i < nPieces; i++)
		{
				// TODO: saque lo de abajo porque se asume que piecePlaying esta alrededor del balon
			if (IsAround(colBall,rowBall,pieces[i]->GetCol(),pieces[i]->GetRow()) //esta alrededor de la pelota
				//TODO: this->MapId(piece->GetTeam(),piece->GetPlayerId()) != lastPlayer)
				&& pieces[i]->Equals(*state.PlayerWithBall()))
					return pieces[i];
		}		
		return NULL;	
	}

	/* 
		Retorna un vector de jugadores alrededor de la pelota del equipo que juega.
		Si la casilla de la pelota es neutra o gol retorna un vector vacío.
		El vector retornado no contiene al jugador que anteriomente tenía el balón.
		Se asume que alguien tiene el balón (stateBeforeBallMov.PlayerWithBall == ballMovement->PlayerWhoMoves).
		Parámetros: stateBeforeBallMov -> estado antes del movimiento del balón
					ballMovement -> movimiento de balón a realizar
	*/ 
	vector<Player> RulesManager::GetPossiblePlayersWithBall(const State& stateBeforeBallMov, const BallMovement* ballMovement)
	{
		vector<Player> playersAround;

		const Board* board = stateBeforeBallMov.GetBoard();
		int colBall = ballMovement->GetSquareAfter()->GetCol(),
			rowBall = ballMovement->GetSquareAfter()->GetRow();
		// El jugador que estaba en posesión del balón
		const Player* playerWithBall = ballMovement->GetPlayerWhoMoves();

		PlayerPiece** pieces;
		if (stateBeforeBallMov.GetTurn() == LEFT)
			pieces = board->GetLeftPieces();
		else
			pieces = board->GetRightPieces();

		int nPieces = board->GetNPieces();

		// Si la pelota está en una casilla neutra nadie puede tener el balón, por lo tanto se
		// controla que no sea neutra
		if (!IsNeutralSquare(board, stateBeforeBallMov.GetLevel(), colBall, rowBall)
			&& boardAreas[colBall][rowBall]!=GOAL)
		{
			for (int i = 0; i < nPieces; i ++)
			{
				// Si está alrededor de la pelota y no es igual a playerWithBall (mismos orientation e id)
				if (IsAround(pieces[i]->GetCol(), pieces[i]->GetRow(), colBall, rowBall)
					&& !pieces[i]->Equals(*playerWithBall))
					playersAround.push_back(*pieces[i]);
			}
		}
		return playersAround;
	}

	/* 
		Retorna un vector de jugadores alrededor de la pelota del equipo que juega.
		Si la casilla de la pelota es neutra o gol retorna un vector vacío.
		El vector retornado no contiene al jugador que anteriomente tenía el balón.
		Parámetros: state -> estado antes del movimiento del balón
		Se asume que nadie tiene el balón
	*/
	vector<Player> RulesManager::GetPossiblePlayersWithBall(const State& state)
	{
		vector<Player> playersAround;

		const Board* board = state.GetBoard();
		int colBall = board->GetBall()->GetCol(),
			rowBall = board->GetBall()->GetRow();

		PlayerPiece** pieces;
		if (state.GetTurn() == LEFT)
			pieces = board->GetLeftPieces();
		else
			pieces = board->GetRightPieces();

		int nPieces = board->GetNPieces();

		// Si la pelota está en una casilla neutra nadie puede tener el balón, por lo tanto se
		// controla que no sea neutra
		if (Majority(board, colBall, rowBall) == state.GetTurn()
			&& boardAreas[colBall][rowBall]!=GOAL)
		{
			for (int i = 0; i < nPieces; i ++)
			{
				// Si está alrededor de la pelota y no es igual a playerWithBall (mismos orientation e id)
				if (IsAround(pieces[i]->GetCol(), pieces[i]->GetRow(), colBall, rowBall))
					playersAround.push_back(*pieces[i]);
			}
		}
		return playersAround;
	}

	vector<Player> RulesManager::GetPlayersAroundSquare(int squareCol, int squareRow, const Board* board, ORIENTATION orientation)
	{
		vector<Player> playersAround;

		PlayerPiece** pieces;
		if (orientation == LEFT)
			pieces = board->GetLeftPieces();
		else
			pieces = board->GetRightPieces();

		int nPieces = board->GetNPieces();
		for (int i = 0; i < nPieces; i ++)
		{
			if (IsAround(pieces[i]->GetCol(), pieces[i]->GetRow(), squareCol, squareRow))
				playersAround.push_back(*pieces[i]);
		}

		return playersAround;
	}

	bool RulesManager::IsDrawnBall(const State& state)
	{
		const Board* board = state.GetBoard();
		const BallPiece* ball = board->GetBall();
		// Se setea la pieza que se mueve
		BallMovement movement(*ball, ball->GetRow(), ball->GetCol(), *state.PlayerWithBall());
		
		// columnas
		for (int col=-1; col<=1; col++)
		{
			// filas
			for (int row=-1; row<=1; row++)
			{
				if (row!=0 || col!=0)
				{
					for (int mult=1; mult<=4; mult++)
					{
						row = ball->GetRow() + row * mult;
						col = ball->GetCol() + col * mult;
						// Se setea la posición del movimiento para cada lugar posible
						movement.SetSquareAfter(row, col);
						
						if ( IsValidBallMove(state, &movement)
							&& IsBallFree(board, col, row) )						
							return false;
					}
				}			
			}
		}
		return true;
	}

	bool RulesManager::IsBallFree(const Board* board, int colBall, int rowBall)
	{
		// Si es gol, la pelota está libre
		if (IsGoal(colBall, rowBall, LEFT) || IsGoal(colBall, rowBall, RIGHT)
			|| Majority(board, colBall, rowBall) == NONE)
			return true;

		return false;
	}

	bool RulesManager::IsSpecialSquare(const Square& position, ORIENTATION playerOrientation, int level)
	{
		if (level >= 3)
			return IsSpecial(position.GetCol(), position.GetRow(), playerOrientation);

		return false;
	}
}
