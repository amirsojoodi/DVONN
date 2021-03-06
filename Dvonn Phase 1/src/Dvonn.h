/*============================================================================
 Name        : Dvonn.h
 Author      : Amir Hossein Sojoodi
 Version     : 1
 Copyright   : GNU
 Description : Dvonn in GTK+
 ============================================================================*/

#ifndef DVONN_H_
#define DVONN_H_

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#define DEFULT_PLAYERS_NO 2
#define CSS_PATH "css\\dvonn.css"
#define CSS_PATH_1 "css\\dvonn1.css"
#define GLADE_2_PLAYER_PATH "glade\\Dvonn-Board-2-player.glade"
#define GLADE_3_PLAYER_PATH "glade\\Dvonn-Board-3-player.glade"
#define WINDOW_NAME "dvonn_board"
#define HEXAGON_EMPTY "images\\hexagon_iconn.png"
#define HEXAGON_RED "images\\hexagon_iconn_red.png"
#define HEXAGON_BLACK "images\\hexagon_iconn_black.png"
#define HEXAGON_WHITE "images\\hexagon_iconn_white.png"
#define HEXAGON_YELLOW "images\\hexagon_iconn_yellow.png"
#define DVONN "images\\dvonn-2.jpg"

char line[100];
GtkBuilder *builder;
GtkWidget *window;
int player_no;
int turn;
int round_play;
int *pieces_count;
void button_clicked(GtkWidget *button, gpointer data);
int **board;

void entry_clicked(GtkWidget *window, gpointer data);

void on_dvonn_board_2_player_destroy(GtkWidget *window, gpointer data);

void on_button_clicked(GtkWidget *button, gpointer data);

#endif /* DVONN_H_ */
