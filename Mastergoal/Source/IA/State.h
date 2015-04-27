#ifndef STATE_H
#define STATE_H

#include "Board.h"
#include "Player.h"
#include "PlayerPiece.h"

namespace Common
{
	class State
	{
	public:
		/** Constructores **/
		State(int level, ORIENTATION turn = LEFT, int nKicks = 0, int goalsRightTeam = 0, int goalsLeftTeam = 0);

		// Se hace una copia de board y *playerWithBall
		State(const Board& board, ORIENTATION turn = LEFT, int nKicks = 0, bool firstPly = false, const Player* playerWithBall = NULL, int goalsRightTeam = 0, int goalsLeftTeam = 0);

		State(const State& state);

		~State();

		/** Getters y Setters **/
		inline Board* GetBoard() const { return this->board; }

		inline ORIENTATION GetTurn() const { return this->turn; }

		inline void SetTurn(ORIENTATION turn) { this->turn = turn; }

		// Indica si es la primera jugada
		inline bool FirstPly() const { return this->firstPly; }

		inline void SetFirstPly(bool firstPly) { this->firstPly = firstPly; }

		// Indica el jugador que está en posesión de la pelota
		inline const Player* PlayerWithBall() const { return this->playerWithBall; }

		inline const PlayerPiece* GetPieceWithBall() const { return this->playerWithBall != NULL ? this->board->GetPlayer(this->turn, this->playerWithBall->GetId()) : NULL; }

		// Setea playerWithBall a una copia de *player
		void SetPlayerWithBall(const Player* player);

		inline int GetLevel() const { return this->level; }

		inline int GetGoalsRight() const { return this->goalsRightTeam; }

		inline int GetGoalsLeft() const { return this->goalsLeftTeam; }

		inline void SetGoalsRight(int nGoals) { this->goalsRightTeam = nGoals; }

		inline void SetGoalsLeft(int nGoals) { this->goalsLeftTeam = nGoals; }

		// Retorna la cantidad de pases hechos si se está pasando
		inline int GetNKicks() const { return this->nKicks; }

		inline void SetNKicks(int nKicks) { this->nKicks = nKicks; }

		/** Métodos **/
		// Aplica el movimiento al estado, no verifica si es correcto o no
		void DoMove(const PlayerMovement* movement);

		// Deshace un movimiento del estado, no verifica que el movimiento se haya hecho previamente
		void UndoMove(const PlayerMovement* movement);

		// Aplica el movimiento al estado, no verifica si es correcto o no
		void DoMove(const BallMovement* movement, const Player* toWhom);

		// Deshace un movimiento del estado, no verifica que el movimiento se haya hecho previamente
		void UndoMove(const BallMovement* movement);	

		// Aplica una jugada al estado
		// No hace los movimientos intermedios
		void DoPlay(const Play& play);

		// Deshace una jugada del estado
		void UndoPlay(const Play& play);

		inline void SwitchTurn() { this->turn = this->turn == RIGHT ? LEFT : RIGHT; }

		inline void ResetBoard() { delete this->board; this->board = new Board(this->level); }

		Board* board;

	private:
		//Board* board;

		// Jugador que posee la pelota
		Player* playerWithBall;

		ORIENTATION turn;

		// Es primera jugada
		bool firstPly;

		// Nivel de juego
		int level;

		// Marcador
		// Goles del equipo que mete a la izquierda
		int goalsLeftTeam;

		// Goles del equipo que mete a la derecha
		int goalsRightTeam;

		// Cantidad de pases hechos si es está pasando
		int nKicks;
		
		// Constructor privado
		State() {}
	};
}

#endif
