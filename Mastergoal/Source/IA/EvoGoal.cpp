// Observaciones: La idea es obtener las posiciones de las fichas del tablero, pasar estas posiciones a la funcion doPlay()
// y obtener luego las nuevas posiciones de estas fichas. doPlay() retorna 1 si genera nuevas posiciones, retorna 0 si no
// genera las posiciones debido a que entro un gol, hubo un ciclo o hubo una pelota ahogada. 
// El tablero puede verse como una matriz de 15x11, en un estado inicial las fichas de la pc estan arriba y las del jugador
// abajo. Left se refiere al jugador y right se refiere a la pc. 
//

#include "stdafx.h"
#include "Game.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ficha ball, left[5], right[5];
	
	/**********Agregar Nivel**********/
	int nivel = 1;

	/**********Aca hay que leer el tablero e ir guardando las posiciones de las fichas**********/
	//pelota
	ball.row = 7;
	ball.col = 5;
	
	//ficha 1 (player y pc)
	left[0].row = 12;
	left[0].col = 5; 
	right[0].row = 2;
	right[0].col = 5;
	
	//ficha 2 (player y pc)
	if (nivel == 2 || nivel == 3){
		left[1].row = 10;
		left[1].col = 3;
		right[1].row = 4;
		right[1].col = 3;
	}

	//fichas 3, 4 y 5 (player y pc)
	if (nivel == 3){
		left[2].row = 10;
		left[2].col = 7;
		right[2].row = 4;
		right[2].col = 7;

		left[3].row = 8;
		left[3].col = 2;
		right[3].row = 6;
		right[3].col = 2;

		left[4].row = 8;
		left[4].col = 8;
		right[4].row = 6;
		right[4].col = 8;
	}
	
	/*******************************************************************************************/

	Agent* agentL = new Agent(nivel, LEFT, 1, "agentL1_1");
	Agent* agentR = new Agent(nivel, RIGHT, 1, "agentL1_2");
	Game game1(nivel, RIGHT, agentL, agentR);
	
	if (game1.DoPlay(nivel, ball, left, right)) 
	{
		/**********Aca hay que obtener las nuevas posiciones de las fichas**********/
		printf("\nDespues del movimiento\n");
		printf("ball: %d, %d\n", game1.ball.row, game1.ball.col);
		printf("leftPiece: %d, %d\n", game1.left[0].row, game1.left[0].col);
		printf("rightPiece: %d, %d\n", game1.right[0].row, game1.right[0].col);

		if (nivel == 2 || nivel == 3){
			printf("leftPiece1: %d, %d\n", game1.left[1].row, game1.left[1].col);
			printf("rightPiece1: %d, %d\n", game1.right[1].row, game1.right[1].col);
		}

		if (nivel == 3){
			printf("leftPiece2: %d, %d\n", game1.left[2].row, game1.left[2].col);
			printf("rightPiece2: %d, %d\n", game1.right[2].row, game1.right[2].col);

			printf("leftPiece3: %d, %d\n", game1.left[3].row, game1.left[3].col);
			printf("rightPiece3: %d, %d\n", game1.right[3].row, game1.right[3].col);

			printf("leftPiece4: %d, %d\n", game1.left[4].row, game1.left[4].col);
			printf("rightPiece4: %d, %d\n", game1.right[4].row, game1.right[4].col);
		}
		/***************************************************************************/
	}
	else
	{
		printf("\nDespues del movimiento\n");
		printf("Hubo gol, bucle o pelota ahogada\n");
	}

	getchar();
	return 0;	
}
