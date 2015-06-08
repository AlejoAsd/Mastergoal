#ifndef SQUARE_H
#define SQUARE_H

#include <string.h>
#include "Definitions.h"

namespace Common
{
	class Square
	{
	public:
		// Constructores
		Square();

		Square(int row, int col);

		Square(const std::basic_string<char> squareStr);

		Square(const Square& square);

		inline Square& operator = (const Square& square)
		{
			this->row = square.row;
			this->col = square.col;

			return *this;
		}

		inline bool operator == (const Square& square) const
		{
			if (this->row == square.row && this->col == square.col)
				return true;

			return false;
		}

		inline bool operator != (const Square& square) const { return !(*this == square); }

		// Getters y Setters
		inline int GetRow() const { return this->row; }

		void SetRow(int row) { this->row = row; }

		inline int GetCol() const { return this->col; }

		void SetCol(int col) { this->col = col; }

		/** Métodos **/

		void SetPosition(int row, int col);

		void StringToRow(const std::basic_string<char> rowStr);

		void StringToCol(const std::basic_string<char> colStr);

		void StringToSquare(const std::basic_string<char> squareStr);

		// Retorna true si dos Squares son adyacentes; si son el mismo los considera adyacentes
		inline bool Adjacent(const Square& square) const
		{
			if (ABS(this->row - square.row) <= 1
				&& ABS(this->col - square.col) <= 1)
				return true;

			return false;
		}

		inline bool Adjacent(int row, int col) const
		{
			if (this->row - row <= 1 && this->row - row >= -1
				&& this->col - col <= 1 && this->col - col >= -1)
				return true;

			return false;
		}

		// Retorna true si dos posiciones son adyacentes; si son el mismo los considera adyacentes
		inline static bool Adjacent(int row1, int col1, int row2, int col2)
		{
			if (ABS(row2 - row2) <= 1
				&& ABS(col1 - col2) <= 1)
				return true;

			return false;
		}

		// Retorna la distantica (no Manhattan) entre dos squares
		inline int Distance(const Square& square) const { 
			if ( ABS(this->col - square.col) >=  ABS(this->row - square.row) )
				return ABS(this->col - square.col);
			return ABS(this->row - square.row);
		}

		// Retorna la distantica (no Manhattan) a una coordenada
		inline int Distance(int row, int col) const { 
			if ( ABS(this->col - col) >= ABS(this->row - row) )
				return ABS(this->col - col);
			return ABS(this->row - row);
		}

	private:
		int row;
		int col;
	};
}

#endif
