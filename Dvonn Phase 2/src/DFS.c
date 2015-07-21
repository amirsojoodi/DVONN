/*============================================================================
 Name        : DFS.c
 Author      : Amir Hossein Sojoodi
 Version     : 1
 Copyright   : GNU
 Description : Dvonn in GTK+
 ============================================================================*/

#include "Dvonn.h"

void depth_first_search(int i, int j) {
	if (connected[i][j] == TRUE) {
		return;
	}
	if(board_owner[i][j] == -1){
		return;
	}
	connected[i][j] = TRUE;
	for (int k = 0; k < board_neighbor_number[i][j]; ++k) {
		depth_first_search(board_neighbor[i][j][k * 2],
				board_neighbor[i][j][k * 2 + 1]);
	}
}

void coloring_connected() {
	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; ++j) {
			connected[i][j] = FALSE;
		}
	}
	for (int i = 0; i < 3; ++i) {
		depth_first_search(reds_position[i][0], reds_position[i][1]);
	}
}

void remove_disconnected() {
	coloring_connected();
	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; ++j) {
			if (connected[i][j] == FALSE && board[i][j] > 0) {
				pieces_count[board_owner[i][j]] -= board[i][j];
				board[i][j] = 0;
				board_owner[i][j] = -1;
				hide_count_label(i, j);
				show_image(i, j, HEXAGON_EMPTY);
			}
		}
	}
	for (int i = 0; i < player_no; ++i) {
		show_player_count_label(i, pieces_count[i]);
	}
}
