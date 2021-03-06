#ifndef RULES_MANAGER_H
#define RULES_MANAGER_H

#include "State.h"
#include "Movements.h"
#include "Pieces.h"
#include <vector>
using namespace Common;
using namespace std;

namespace Utils
{
	enum RESPONSES
	{
		TURN_OK = 0,
		MOVE_OK,
		CLICK_OK,
		INVALID_CLICK_NOT_PLAYER,
		INVALID_CLICK_NOT_TURN,
		INVALID_CLICK_NOT_PIECE,
		INVALID_CLICK,
		//INVALID_MOVE,
		INVALID_KICK,
		NO_MOVES_LEFT,
		ERROR_NULL_PLAY,
		ERROR_NULL_MOVEMENT,
		IDLE,

		//INVALID - MOVIMIENTOS JUGADOR

		INVALID_GET_BALL, //(debe alcanzar la pelota)
		INVALID_PLAYER_MOVE, //(violo una regla basica)
		INVALID_NEUTRAL_IN_AREA, //(hay una casilla neutra en su area y puede llegar)
		INVALID_PLAYER_CORNER, //(no posicionarse en su propio corner)

		//INVALID - MOVIMIENTOS PELOTA

		INVALID_BALL_NEUTRAL, //(pelota en casilla neutra, no se puede mover)
		INVALID_MOVE, //(violo una regla basica de pelota)
		INVALID_AUTOPASS, //(quiso hacer un autopase)
		INVALID_PASS_OPPONENT, //(le quiso pasar al contrario)
		INVALID_BALL_CORNER, //(quiso enviar la pelota a su propio corner)
		INVALID_LEAVE_BALL, //(no puede dejar la pelota en su area en la primera jugada y despues de un gol)
		INVALID_FREE_BALL, //la pelota esta libre, no se puede mover
		INVALID_AUTOGOAL, //gol en contra
		INVALID_LITTLE_AREA_DEFENSE, //no se puede pasar la pelota sobre los defensores en el area chica
		INVALID_OVER_KEEPER, //no se puede pasar la pelota sobre los brazos del arquero en el area chica ni sobre su cuerpo
		                    //en el area grande, no se puede sobre el cuerpo

		//VALID - MOVIMIENTOS PELOTA

		VALID_NEUTRAL, //(dejo pelota en casilla neutra)
		VALID_NEUTRAL_REPEAT,
		VALID_PASS, // (paso la pelota a otro de su equipo)
		VALID_REPEAT_TURN, //(llego a casilla especial.. otro turno)
		VALID_GOAL, //(metio gol)
		VALID_GOAL_RIGHT,
		VALID_GOAL_LEFT,

		//VALID - MOVIMIENTOS JUGADOR

		VALID_OK, //(termino movimiento jugador y pelota, se usa para pelota tambien)
		VALID_HAS_BALL, // (termino movimiento jugador, alcanzo la pelota)

		CHANGE_PLAYER,

		// AGENT - MOVIMIENTOS AGENTE
		AGENT_START,

		MODE_HANDLED,

		SET_OK,

		GAME_STOPPED
	};

	enum 
	{
		MAX_MOVES = 4
	};

	enum AREAS 
	{
		GOAL = 1,
		SPECIAL,
		LITTLE,
		BIG,
		CORNER,
		AREA,
		CENTRAL
	};

	const AREAS boardAreas[11][15] = 
	{	{ AREA, CORNER, AREA, AREA, AREA, AREA, AREA, CENTRAL, AREA, AREA, AREA, AREA, AREA, CORNER, AREA },
		{ AREA, BIG ,BIG ,BIG ,BIG ,AREA ,AREA ,CENTRAL ,AREA ,AREA ,BIG ,BIG ,BIG ,BIG ,AREA },
		{ AREA, LITTLE, LITTLE, BIG, BIG, AREA, AREA, CENTRAL, AREA, AREA, BIG, BIG, LITTLE, LITTLE, AREA },
		{ GOAL, SPECIAL, LITTLE, BIG, BIG, AREA, AREA, CENTRAL, AREA, AREA, BIG, BIG, LITTLE, SPECIAL, GOAL },
		{ GOAL, SPECIAL, LITTLE, BIG, BIG, AREA, AREA, CENTRAL, AREA, AREA, BIG, BIG, LITTLE, SPECIAL, GOAL },
		{ GOAL, SPECIAL, LITTLE, BIG, BIG, AREA, AREA, CENTRAL, AREA, AREA, BIG, BIG, LITTLE, SPECIAL, GOAL },
		{ GOAL, SPECIAL, LITTLE, BIG, BIG, AREA, AREA, CENTRAL, AREA, AREA, BIG, BIG, LITTLE, SPECIAL, GOAL },
		{ GOAL, SPECIAL, LITTLE, BIG, BIG, AREA, AREA, CENTRAL, AREA, AREA, BIG, BIG, LITTLE, SPECIAL, GOAL },
		{ AREA, LITTLE, LITTLE, BIG, BIG, AREA, AREA, CENTRAL, AREA, AREA, BIG, BIG, LITTLE, LITTLE, AREA },
		{ AREA, BIG, BIG, BIG, BIG, AREA, AREA, CENTRAL, AREA, AREA, BIG, BIG, BIG, BIG, AREA },
		{ AREA, CORNER, AREA, AREA, AREA, AREA, AREA, CENTRAL, AREA, AREA, AREA, AREA, AREA, CORNER, AREA }
	};
	
	class RulesManager
	{
	public:
		static bool IsValidPlayerMove(const State& state, const PlayerMovement* movement);

		static bool IsValidBallMove(const State& state, const BallMovement* movement);

		/* 
			Retorna un vector de jugadores alrededor de la pelota del equipo que juega.
			Si la casilla de la pelota es neutra o si nadie est� alrededor retorna un vector vac�o.
			Se asume que nadie estaba en posesi�n del bal�n antes del movimiento.
			Par�metros: stateAfterPlayerMov -> el estado luego del movimiento de un jugador
		*/ 
		//static vector<Player> GetPossiblePlayersWithBall(const State& stateAfterPlayerMov);
		// Retorna true si el jugador alcarnz� la pelota
		//static bool GotBall(const State& stateAfterPlayerMov, const Player& player);

		/* 
			Retorna un vector de jugadores alrededor de la pelota del equipo que juega.
			Si la casilla de la pelota es neutra o si nadie est� alrededor retorna un vector vac�o.
			El vector retornado no contiene al jugador que anteriomente ten�a el bal�n.
			Se asume que alguien tiene el bal�n (stateBeforeBallMov.PlayerWithBall == movement->PlayerWhoMoves).
			Par�metros: stateBeforeBallMov -> estado antes del movimiento del bal�n
						ballMovement -> movimiento del bal�n a realizar
		*/ 
		static vector<Player> GetPossiblePlayersWithBall(const State& stateBeforeBallMov, const BallMovement* ballMovement);

		/* 
			Retorna un vector de jugadores alrededor de la pelota del equipo que juega.
			Si la casilla de la pelota es neutra o si nadie est� alrededor retorna un vector vac�o.
			El vector retornado no contiene al jugador que anteriomente ten�a el bal�n.
			Se asume que nadie tiene el bal�n
			Par�metros: stateBeforeBallMov -> estado antes del movimiento del bal�n
		*/ 
		static vector<Player> GetPossiblePlayersWithBall(const State& state);

		// Devuelve la respuesta de la movida evaluada
		static RESPONSES GetResponse() { return response; }

		// Retorna true si no hay lugar posible donde tirar la pelota y que quede libre
		static bool IsDrawnBall(const State& state);

		// Retorna true si la casilla es especial (en los niveles inferiores a 3 siempre retorna false)
		static bool IsSpecialSquare(const Square& position, ORIENTATION playerOrientation, int level);

		inline static bool IsGoal(int col, int row, ORIENTATION orientation) 
		{ 
			return ((orientation==RIGHT && boardAreas[col][row]==GOAL && row==14) 
				|| (orientation==LEFT && boardAreas[col][row]==GOAL && row==0));
		}

		static bool IsSpecial(int col, int row, ORIENTATION orientation);

		inline static bool IsValidSquare(const Square* sqr)
		{
			return ((sqr->GetRow() > 0 && sqr->GetRow() < 14 && sqr->GetCol() >= 0 && sqr->GetCol() <= 10)
				|| IsGoal(sqr->GetCol(), sqr->GetRow(), LEFT)
				|| IsGoal(sqr->GetCol(), sqr->GetRow(), RIGHT));
		}

		inline static bool IsValidSquare(const Square* sqr, ORIENTATION turn)
		{
			return ((sqr->GetRow() > 0 && sqr->GetRow() < 14 && sqr->GetCol() >= 0 && sqr->GetCol() <= 10)
				|| IsGoal(sqr->GetCol(), sqr->GetRow(), turn));
		}

		//Reglas basicas Mov Pc
		inline static bool IsCorrectPlayerMovePC(const Board* board, const PlayerMovement* movement)
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
			if(ABS(colBefore-colAfter)>2 || ABS(rowBefore-rowAfter)>2) 
				return false; 

			/* No moverse fuera de las dimensiones del tablero (el usuario tal vez no, pero la inteligencia si) */
			if(colAfter<0 || colAfter>10 || rowAfter<1 || rowAfter>13) 
				return false;

			/* El jugador no puede saltar sobre ninguna ficha */
			if(colBefore == colAfter) //si se mueve hacia la derecha o hacia la izquierda (avanza o retrocede)
			{					
				if((rowAfter>rowBefore) && (ABS(rowAfter-rowBefore) != 1))	//si se mueve hacia la derecha 2 casillas
				{
					if(IsPieceInSquare(board, colBefore,rowAfter-1)) //si hay alguna pieza en esa casilla intermedia
						return false; //movimiento no valido
				}
				else if((rowBefore>rowAfter) && (ABS(rowAfter-rowBefore) != 1)) //si se mueve hacia la izquierda 2 casillas
				{
					if(IsPieceInSquare(board, colBefore,rowAfter+1)) 
						return false;
				}
			}
			else if(rowBefore == rowAfter) //si se mueve hacia arriba o hacia abajo (hacia los costados)
			{
				if((colAfter > colBefore) && (ABS(colAfter-colBefore)!=1)) //si se mueve hacia abajo 2 casillas
				{
					if(IsPieceInSquare(board, colAfter-1,rowBefore)) 
						return false;
				}
				else if((colBefore > colAfter) && (ABS(colAfter-colBefore)!=1)) //si se mueve hacia arriba 2 casillas
				{
					if(IsPieceInSquare(board, colAfter+1,rowBefore)) 
						return false;
				}
			}	
			else //si se mueve en diagonal
			{	
				/* Movimiento en L no es valido */
				if(ABS(colBefore-colAfter) != ABS(rowBefore-rowAfter)) //movimiento diagonal, diferencias entre i's y j's iguales
					return false;

				if(ABS(colBefore-colAfter)!=1) //si se mueve 2 casillas, no 1
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

			/* Reglas Nivel 2 y 3 */
			if(level > 1) 
				/* Jugador no puede ocupar casilla de corner propia (pero si puede apoderarse de la pelota en esa casilla) */
				if(IsCorner(colAfter,rowAfter,((PlayerPiece*)movement->GetPiece())->GetOrientation())
					&& !IsBallAround(board, colAfter,rowAfter))		
					return false;

			/* Reglas Nivel 3. Reglas del arquero */
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
				}		
			}
			return true;
		}

		inline static bool IsValidPlayerMovePC(const State& state, const PlayerMovement* movement)
		{
			int level = state.GetLevel();
			const Board* board = state.GetBoard();

			/* En la primera jugada y despues de un gol, debe tomar la pelota si o si */
			if(state.FirstPly() && !IsBallAround(board, movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow())) 	
			{
				return false;
			}
				
			/* Reglas basicas */
			if(!IsCorrectPlayerMovePC(board, movement))
			{
				return false;
			}
		
			// Indica que la pelota se encuentra en una casilla neutra
			bool neutralSquare = IsNeutralSquare(board, level, board->GetBall()->GetCol(), board->GetBall()->GetRow());

			/* Si esta en posesion de la pelota no puede moverse el jugador */ 
			if(IsBallAround(board, movement->GetPiece()->GetCol(), movement->GetPiece()->GetRow())
				&& neutralSquare==false)
			{
				return false;
			}

			/* Reglas Nivel 2 y 3 */
			if(level > 1) 
			{
				/* Reglas Casilla Neutra */
				if (neutralSquare)
				{
					/* Jugador no puede dejar su posicion si participa de una casilla neutra */
					if(IsBallAround(board, movement->GetPiece()->GetCol(),movement->GetPiece()->GetRow())
						&& !IsBallAround(board, movement->GetSquareAfter()->GetCol(),movement->GetSquareAfter()->GetRow()))
					{
						return false;
					}

					/* Cuando un jugador puede apoderarse de una casilla neutra que esta en su campo, debe hacerlo */
					if(IsNeutralSquareInArea(board, movement))
					{
						return false;
					}
				}
			}
			return true;
		}

		// Controles para la PC (son menos que para el humano)
		// No se controla la cantidad de pases
		// No se controla que la pelota est� adyacente a un jugador antes de chutar
		// No se controla si la pelota se mueve m�s de 4 casillas
		// No se controla si se hace un movimieno en L
		inline static bool IsValidBallMovePC(const State& state, const BallMovement* movement)
		{
			// Si la pelota no est� dentro del campo de juego
			if (!IsValidSquare(movement->GetSquareAfter(), state.GetTurn()))
				return false;		
			
			int ballRow = movement->GetSquareAfter()->GetRow();
			int ballCol = movement->GetSquareAfter()->GetCol();			

			// Si es gol en contra
			// TODO: se puede meter gol en contra, para dar m�s libertad a los individuos
			//if (IsAutogoal(ballCol, ballRow, state.GetTurn()))
			//	return false;			

			int adjTeam = 0;
			int adjOp = 0;
			PlayerPiece** teamPlayers = state.GetTurn() == LEFT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
			PlayerPiece** opPlayers = state.GetTurn() == RIGHT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
			// Se recorren todas las fichas del tablero y se verifican varias propiedades
			for (int i = 0; i < state.GetBoard()->GetNPieces(); i++)
			{
				// Si la pelota se chuta a una casilla ocupada
				if ((teamPlayers[i]->GetRow() == ballRow && teamPlayers[i]->GetCol() == ballCol)
					|| (opPlayers[i]->GetRow() == ballRow && opPlayers[i]->GetCol() == ballCol))
					return false;

				// Si el jugador est� al lado de la pelota
				if (teamPlayers[i]->GetPosition()->Adjacent(ballRow, ballCol))
					adjTeam++;				
				if (opPlayers[i]->GetPosition()->Adjacent(ballRow, ballCol))
					adjOp++;
			}
			
			bool ballInPlArea = IsBigArea(ballCol, ballRow, state.GetTurn());
			
			if (state.GetLevel() >= 3)
			{
				int rowBefore = movement->GetSquareBefore()->GetRow();
				int colBefore = movement->GetSquareBefore()->GetCol();
				int rowInc = ballRow - rowBefore;
				int colInc = ballCol - colBefore;
				int nSteps;
				if ( ABS(rowInc) >= ABS(colInc) )
					nSteps = ABS(rowInc);
				else
					nSteps = ABS(colInc);
				
				if (rowInc > 0) rowInc = 1;
				else if (rowInc < 0) rowInc = -1;
				if (colInc > 0) colInc = 1;
				else if (colInc < 0) colInc = -1;

				ORIENTATION opOrient = state.GetTurn() == LEFT ? RIGHT : LEFT;
				const PlayerPiece* keeper = state.GetBoard()->GetPlayer(state.GetTurn(), ID_G);	
				const PlayerPiece* opKeeper = state.GetBoard()->GetPlayer(opOrient, ID_G);	
				// Si el arquero est� dentro de su �rea
				bool keeperInArea = IsBigArea(keeper->GetCol(), keeper->GetRow(), state.GetTurn());
				bool opKeeperInArea = IsBigArea(opKeeper->GetCol(), opKeeper->GetRow(), opOrient);
									
				int auxRow, auxCol;
				// Camino recorrido por la pelota
				for (int step = 1; step < nSteps; step++)
				{
					auxRow = rowBefore + rowInc * step;
					auxCol = colBefore + colInc * step;

					// Si la pelota pasa encima del arquero	
					if (keeperInArea
						&& keeper->GetRow() == auxRow && keeper->GetCol() == auxCol)
						return false;

					// Si la pelota pasa encima del arquero	oponente o sus brazos
					if (opKeeperInArea 
						&& opKeeper->GetRow() == auxRow 
						&& (opKeeper->GetCol() == auxCol // pasa encima del arquero
							|| (opKeeper->GetCol() - 1 == auxCol && IsBigArea(opKeeper->GetCol() - 1, auxRow, opOrient)) // pasa al costado
							|| (opKeeper->GetCol() + 1 == auxCol && IsBigArea(opKeeper->GetCol() + 1, auxRow, opOrient)))) // pasa al costado
							return false;

					// Si la pelota pasa encima de alg�n oponente en su �rea chica
					if (IsLittleArea(auxCol, auxRow, opOrient))
					{
						PlayerPiece** opPieces = opOrient == LEFT ? state.GetBoard()->GetLeftPieces() : state.GetBoard()->GetRightPieces();
						for (int i = 0; i < state.GetBoard()->GetNPieces() - 1; i++) // hasta nPieces - 1 porque el arquero ya se control�
						{
							if (opPieces[i]->GetRow() == auxRow 
								&& opPieces[i]->GetCol() == auxCol)
								return false;
						}
					}
				}

				// Si la pelota est� en brazos del arquero oponente en su �rea
				if (opKeeperInArea
					&& opKeeper->GetRow() == ballRow 
					&& ((opKeeper->GetCol() - 1 == ballCol && IsBigArea(opKeeper->GetCol() - 1, ballRow, opOrient))
						|| (opKeeper->GetCol() + 1 == ballCol && IsBigArea(opKeeper->GetCol() + 1, ballRow, opOrient))))
				{
					// El arquero contrario est� en posesi�n de la pelota
					return false;
				}

				// Si la pelota est� en los brazos del arquero en su �rea
				if (ballInPlArea 
					&& keeperInArea
					&& keeper->GetRow() == ballRow 
					&& (keeper->GetCol() - 1 == ballCol || keeper->GetCol() + 1 == ballCol))
				{
					// Seteo a 0 los oponentes alrededor del bal�n porque el 
					// arquero est� en posesi�n
					adjOp = 0; 
				}				
			}		

			// Si meti� gol
			if (IsGoal(ballCol, ballRow, state.GetTurn()))
				return true;

			// Si la pelota se deja en posesi�n del contrario
			if (adjOp > adjTeam)
				return false;

			// Si se deja la pelota en el �rea grande propia, 
			// o si la pelota est� en el corner propio
			if ((adjTeam == 0 || adjTeam == adjOp)
				&& (ballInPlArea || IsCorner(ballCol, ballRow, state.GetTurn())))
				return false;

			// Si es un autopase o deja la pelota en una casilla neutra adyacente
			if ((adjTeam == 1 || adjTeam == adjOp)
				&& state.GetPieceWithBall()->GetPosition()->Adjacent(ballRow, ballCol))
				return false;

			// En la primera jugada la pelota debe estar en o pasar el mediocampo
			if (state.FirstPly())
			{
				if (state.GetTurn() == RIGHT)
				{
					if (movement->GetSquareAfter()->GetRow() < 7)
						return false;
				}
				else
				{
					if (movement->GetSquareAfter()->GetRow() > 7)
						return false;
				}
			}

			return true;
		}

		inline static bool IsNeutralSquare(const State& state)
		{
			return IsNeutralSquare(state.GetBoard(), state.GetLevel(), state.GetBoard()->GetBall()->GetCol(), state.GetBoard()->GetBall()->GetRow());
		}

	private:		
		// Respuesta de la jugada o movida
		static RESPONSES response;

		// Retorna true si no hay jugadores alrededor de la pelota
		static bool IsBallFree(const Board* board, int colBall, int rowBall);

		// TODO: no se usa
		static vector<Player> GetPlayersAroundSquare(int squareCol, int squareRow, const Board* board, ORIENTATION orientation);

		// Verifica si la coordenada colA,rowA esta alrededor de la coordenada colB,rowB
		static bool IsBallAround(const Board* board, int col, int row);

		/* Verifica si la coordenada colA,rowA esta alrededor de la coordenada colB,rowB */
		inline static bool IsAround(int colA, int rowA, int colB, int rowB)
		{
			return (colB == 0) ? colA <= colB + 1 && rowA <= rowB + 1 && rowA >= rowB - 1 : colA <= colB + 1 && colA >= colB - 1 && rowA <= rowB + 1 && rowA >= rowB - 1;

			//return flag;
		}

		//metodos que controlan movimientos de los jugadores
		static bool IsCorrectPlayerMove(const Board* board, const PlayerMovement* movement);

		static bool IsOpponentInSquare(const Board* board, int col, int row, ORIENTATION);

		inline static bool IsCorner(int col, int row, ORIENTATION orientation)
		{
			return ((orientation==RIGHT && boardAreas[col][row]==CORNER && row==1) 
				|| (orientation==LEFT && boardAreas[col][row]==CORNER && row==13));
		}

		inline static bool IsAutogoal(int col, int row, ORIENTATION orientation) 
		{
			return ((orientation==RIGHT && boardAreas[col][row]==GOAL && row==0) 
				|| (orientation==LEFT && boardAreas[col][row]==GOAL && row==14));
		}

		static bool IsPlayerArea(int col, int row, ORIENTATION orientation);	

		static bool IsLittleArea(int col, int row, ORIENTATION orientation);

		inline static bool IsBigArea(int col, int row, ORIENTATION orientation) 
		{ 
			return ((orientation==RIGHT && boardAreas[col][row]<=BIG && row<7) 
				|| (orientation==LEFT && boardAreas[col][row]<=BIG && row>7));
		}

		static bool IsNeutralSquare(const Board* board, int level, int colBall, int rowBall);

		static bool IsNeutralSquareInArea(const Board* board, const PlayerMovement* movement);

		//metodos para las reglas del arquero
		static int IsGoalkeeper(const Board* board, int col, int row, ORIENTATION orientation);

		//metodos que controlan movimientos de la pelota
		// TODO: no se usa
		static const PlayerPiece* PiecePlaying(const State& state, int colBall, int rowBall);

		static ORIENTATION Majority(const Board* board, int colBall, int rowBall);

		inline static bool IsAutopass(const Board* board, int colBall, int rowBall, const PlayerPiece* piecePlaying) 
		{	
			/* Si es gol, no hay autopase aunque el jugador que mueve la pelota este en casilla contigua */
			if(IsGoal(colBall, rowBall, piecePlaying->GetOrientation()))
				return false;

			/* Si la pelota no queda en una casilla contigua al jugador que pateo, no es auto pase */
			if(!piecePlaying->GetPosition()->Adjacent(rowBall, colBall))
				return false;

			vector<Player> playersAround = GetPlayersAroundSquare(colBall, rowBall, board, piecePlaying->GetOrientation());

			/* Si la pelota queda en una casilla contigua al jugador que la mueve, verificar que haya
			   otro jugador del mismo equipo alrededor de esa casilla para saber que no es un autopase */
			for (vector<Player>::iterator it = playersAround.begin(); it != playersAround.end(); it++)
			{
				if (!(it->Equals(*piecePlaying)))
					return false;
			}

			return true;
		}

		inline static bool IsPieceInSquare(const Board* board, int col, int row)
		{
			if (board->GetBall()->GetRow() == row && board->GetBall()->GetCol() == col)
				return true;

			int nPieces = board->GetNPieces();
			PlayerPiece** pieces = board->GetLeftPieces();
			for (int i = 0; i < nPieces; i++)
			{
				if (pieces[i]->GetRow() == row && pieces[i]->GetCol() == col)
					return true;
			}

			pieces = board->GetRightPieces();
			for (int i = 0; i < nPieces; i++)
			{
				if (pieces[i]->GetRow() == row && pieces[i]->GetCol() == col)
					return true;
			}
			return false;
		}		
	};
}

#endif
