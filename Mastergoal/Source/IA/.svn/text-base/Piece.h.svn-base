#ifndef PIECE_H
#define PIECE_H

#include "Square.h"

namespace Common
{
	enum TYPEPIECE
	{
		TYPE_SQUARE = 0,
		TYPE_PLAYER,
		TYPE_BALL
		
	};

	class Piece
	{
	public:
		// Destructor virtual para que los punteros le llamen al de la sub clase
		virtual ~Piece();

		/** Getters y Setters **/
		inline TYPEPIECE GetType() const { return this->type; }

		inline const Square* GetPosition() const { return this->position; }

		inline int GetRow() const { return this->position->GetRow(); }

		inline int GetCol() const { return this->position->GetCol(); }

		// Setea la posición haciendo una copia de position
		void SetPosition(const Square& position);

		/** Métodos **/
		void SetPosition(int row, int col);

	protected:
		TYPEPIECE type;

		Square* position;

		/** Constructores **/
		// Setea la posición haciendo una copia de position
		Piece(TYPEPIECE type, const Square& position);

		Piece(TYPEPIECE type, int row, int col);

		Piece() { }
	};
}

#endif
