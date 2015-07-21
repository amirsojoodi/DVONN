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
#include <ctype.h>

#define DEFULT_PLAYERS_NO 2
#define TWO_PLAYERS_PIECE_COUNT 23
#define THREE_PLAYERS_PIECE_COUNT 17
#define CSS_PATH "css\\dvonn.css"
#define CSS_PATH_1 "css\\dvonn1.css"
#define GLADE_2_PLAYER_PATH "images\\Dvonn-Board-2-player.glade"
#define GLADE_3_PLAYER_PATH "images\\Dvonn-Board-3-player.glade"
#define WINDOW_NAME "dvonn_board"
#define HEXAGON_EMPTY "images\\hexagon_iconn.png"
#define HEXAGON_RED "images\\hexagon_iconn_red.png"
#define HEXAGON_BLACK "images\\hexagon_iconn_black.png"
#define HEXAGON_WHITE "images\\hexagon_iconn_white.png"
#define HEXAGON_YELLOW "images\\hexagon_iconn_yellow.png"
#define DVONN_IMAGE "images\\dvonn-2.jpg"
#define DVONN_SAVED_PATH "files\\dvonn.dvn"
#define DVONN_PREVIOUS_PATH "files\\previous.dvn"
#define DVONN_NEIGHBOR2_PATH "files\\neighbor2.dst"
#define DVONN_NEIGHBOR3_PATH "files\\neighbor3.dst"
#define BOARD_ROWS 5
#define BOARD_COLS 15

char line[100]; //Temporary character array
GtkBuilder *builder;
GtkWidget *window;
int player_no; //whether 2 or 3
int phase; //whether 1 or 2
int turn; //whether 1 or 2 (or 3)
int round_play; //Between 0 and 49 (or 54)
int pieces_count[3];
int **board;
int **board_owner; //every place is -1 or 0 or 1 (or 2) or 3
// -1 -> empty
// 0 -> owner is white
// 1 -> owner is black
// 2 -> owner is yellow
// 3 -> owner is red
int selected_i, selected_j;
char *turn_images[3];
char *button_images[3];
char *count_labels[3];
int columns_number[BOARD_ROWS];
char player_names[3][50];
int reds_position[3][2];
gboolean connected[BOARD_ROWS][BOARD_COLS];
int board_neighbor[BOARD_ROWS][BOARD_COLS][12];
int board_neighbor_number[BOARD_ROWS][BOARD_COLS];
int pass_no;

void on_button_clicked_phase1(GtkWidget *button, gpointer data);

void on_button_clicked_phase2(GtkWidget *button, gpointer data);

void start_clicked(GtkWidget *button, gpointer data);

void entry_clicked(GtkWidget *window, gpointer data);

void about_game_clicked(GtkWidget *window, gpointer data);

void save_game_clicked(GtkWidget *window, gpointer data);

void load_game_clicked(GtkWidget *window, gpointer data);

void new_game_clicked(GtkWidget *button, gpointer data);

void undo_clicked(GtkWidget *button, gpointer data);

void initialize_neighbors(int player_no);

void save_game();

G_MODULE_EXPORT void on_dvonn_board_2_player_destroy(GtkWidget *window, gpointer data);

G_MODULE_EXPORT void on_button_clicked(GtkWidget *button, gpointer data);

void show_hint(char *hint);

void show_red_labels();

void show_count_label(int i, int j, int value);

void show_count_label_markup(int i, int j, int value, char *format);

void hide_count_label(int i, int j);

void show_image_turn(int turn);

void hide_all_image_turn();

void show_image(int i, int j, char *file_name);

void show_player_count_label(int turn, int value);

void show_label(char *name);

void hide_label(char *name);

void show_button(char *name);

void hide_button(char *name);

void initialize_css(gchar *css_path);

void initialize_builder();

void undo_redo();

void set_label_text(char *label_name, char *text);

void show_end_game();

void greetings();

void remove_disconnected();

#endif /* DVONN_H_ */
