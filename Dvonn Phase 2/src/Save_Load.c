/*
 ============================================================================
 Name        : Save_Load.c
 Author      : Amir Hossein Sojoodi
 Version     : 1
 Copyright   : GNU
 Description : Dvonn in GTK+
 ============================================================================
 */

#include "Dvonn.h"

void new_game_clicked(GtkWidget *button, gpointer data) {
	gtk_widget_destroy(window);
	greetings();
}

// dvonn file contains:
// phase number
// round
// No. of players
// turn
// player name each in one line
// piece count of players each in one line
// red pieces places in one line
// board contain
// board_owner contain

void load_game_clicked(GtkWidget *button, gpointer data) {

	if (data != (int *) 1) {
		load_game_clicked(button, (int *) 1);
	}

	GtkStatusbar *status_bar;
	FILE *fp = fopen(DVONN_SAVED_PATH, "r");

	board = (int **) malloc(sizeof(int *) * BOARD_ROWS);
	board_owner = (int **) malloc(sizeof(int *) * BOARD_ROWS);

	fscanf(fp, "%d", &phase);
	fscanf(fp, "%d", &round_play);
	fscanf(fp, "%d", &player_no);
	fscanf(fp, "%d", &turn);

	for (int i = 0; i < player_no; ++i) {
		fscanf(fp, "%s", player_names[i]);
	}

	for (int i = 0; i < player_no; ++i) {
		fscanf(fp, "%d", &pieces_count[i]);
	}

	for (int i = 0; i < 3; ++i) {
		fscanf(fp, "%d", &reds_position[i][0]);
		fscanf(fp, "%d", &reds_position[i][1]);
	}

	columns_number[0] = (player_no == 2) ? 9 : 10;
	columns_number[1] = columns_number[0] + 1;
	columns_number[2] = columns_number[1] + 1;
	columns_number[3] = columns_number[1];
	columns_number[4] = columns_number[0];

	for (int i = 0; i < BOARD_ROWS; ++i) {
		board[i] = (int *) malloc(sizeof(int) * columns_number[i]);
		board_owner[i] = (int *) malloc(sizeof(int) * columns_number[i]);
		for (int j = 0; j < columns_number[i]; j++) {
			fscanf(fp, "%d", &board[i][j]);
		}
	}

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; j++) {
			fscanf(fp, "%d", &board_owner[i][j]);
		}
	}

	fclose(fp);

	initialize_builder();

	initialize_neighbors(player_no);

	gtk_widget_destroy(window);

	window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_NAME));

	initialize_css(CSS_PATH);

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; j++) {
			sprintf(line, "button_%d_%d", i, j);
			g_object_set_data(G_OBJECT(gtk_builder_get_object(builder, line)),
					"button_i", GINT_TO_POINTER(i));
			g_object_set_data(G_OBJECT(gtk_builder_get_object(builder, line)),
					"button_j", GINT_TO_POINTER(j));

			if (board[i][j] > 1) {
				show_count_label(i, j, board[i][j]);
			}
		}
	}

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; j++) {
			if (board_owner[i][j] == -1) {
				show_image(i, j, HEXAGON_EMPTY);
			} else if (board_owner[i][j] == 3) {
				show_image(i, j, HEXAGON_RED);
			} else {
				show_image(i, j, button_images[board_owner[i][j]]);
			}
		}
	}

	show_red_labels();

	set_label_text("white_player", player_names[0]);
	set_label_text("black_player", player_names[1]);

	sprintf(line, "%s vs. %s", player_names[0], player_names[1]);

	if (player_no > 2) {
		set_label_text("yellow_player", player_names[2]);
		sprintf(line, "%s vs. %s", line, player_names[2]);
	}

	set_label_text("white_count", g_strdup_printf("%i", pieces_count[0]));
	set_label_text("black_count", g_strdup_printf("%i", pieces_count[1]));

	if (player_no > 2) {
		set_label_text("yellow_count", g_strdup_printf("%i", pieces_count[2]));
	}

	status_bar = GTK_STATUSBAR(gtk_builder_get_object(builder, "statusbar"));

	gtk_statusbar_push(status_bar,
			gtk_statusbar_get_context_id(status_bar, line), line);

	gtk_window_set_title(GTK_WINDOW(window), "Let's Play Dvonn!");

	undo_redo();

	gtk_widget_show_all(window);

	hide_label("phase2");
	hide_label("phase1");

	hide_label("hint");

	hide_button("pass");

	turn_images[0] = "white_turn_image";
	turn_images[1] = "black_turn_image";
	turn_images[2] = "yellow_turn_image";

	button_images[0] = HEXAGON_WHITE;
	button_images[1] = HEXAGON_BLACK;
	button_images[2] = HEXAGON_YELLOW;

	count_labels[0] = "white_count";
	count_labels[1] = "black_count";
	count_labels[2] = "yellow_count";

	hide_all_image_turn();

	show_image_turn(turn);

	if (phase == 2) {
		show_button("pass");
		show_label("phase2");
		selected_i = -1;
		selected_j = -1;
	} else {
		show_label("phase1");
	}

	if (data == (int *) 1) {
		return;
	}
	gtk_main();
}

void save_game_clicked(GtkWidget *button, gpointer data) {

	FILE *fp = fopen(DVONN_SAVED_PATH, "w");

	fprintf(fp, "%d\n", phase);
	fprintf(fp, "%d\n", round_play);
	fprintf(fp, "%d\n", player_no);
	fprintf(fp, "%d\n", turn);

	for (int i = 0; i < player_no; ++i) {
		fprintf(fp, "%s\n", player_names[i]);
	}

	for (int i = 0; i < player_no; ++i) {
		fprintf(fp, "%d\n", pieces_count[i]);
	}

	for (int i = 0; i < 3; ++i) {
		fprintf(fp, "%d ", reds_position[i][0]);
		fprintf(fp, "%d ", reds_position[i][1]);
	}

	fprintf(fp, "\n");

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; ++j) {
			fprintf(fp, "%d ", board[i][j]);
		}
		fprintf(fp, "\n");
	}
	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; ++j) {
			fprintf(fp, "%d ", board_owner[i][j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}

// previous file contains
// phase
// round play
// turn
// piece count each in one line
// red positions
// board content
// board_owner content
void undo_clicked(GtkWidget *button, gpointer data) {

	FILE *fp = fopen(DVONN_PREVIOUS_PATH, "r");

	fscanf(fp, "%d", &phase);
	fscanf(fp, "%d", &round_play);
	fscanf(fp, "%d", &turn);

	for (int i = 0; i < player_no; ++i) {
		fscanf(fp, "%d", &pieces_count[i]);
	}

	for (int i = 0; i < 3; ++i) {
		fscanf(fp, "%d", &reds_position[i][0]);
		fscanf(fp, "%d", &reds_position[i][1]);
	}

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; j++) {
			fscanf(fp, "%d", &board[i][j]);
		}
	}

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; j++) {
			fscanf(fp, "%d", &board_owner[i][j]);
		}
	}

	fclose(fp);

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; j++) {
			hide_count_label(i, j);
			if (board[i][j] > 1) {
				show_count_label(i, j, board[i][j]);
			}
		}
	}
	show_red_labels();

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; j++) {
			if (board_owner[i][j] == -1) {
				show_image(i, j, HEXAGON_EMPTY);
			} else if (board_owner[i][j] == 3) {
				show_image(i, j, HEXAGON_RED);
			} else {
				show_image(i, j, button_images[board_owner[i][j]]);
			}
		}
	}

	set_label_text("white_count", g_strdup_printf("%i", pieces_count[0]));
	set_label_text("black_count", g_strdup_printf("%i", pieces_count[1]));

	if (player_no > 2) {
		set_label_text("yellow_count", g_strdup_printf("%i", pieces_count[2]));
	}

	hide_label("phase2");
	hide_label("phase1");
	hide_label("hint");
	hide_button("pass");

	hide_all_image_turn();

	show_image_turn(turn);

	if (phase == 2) {
		show_button("pass");
		show_label("phase2");
		selected_i = -1;
		selected_j = -1;
	} else {
		show_label("phase1");
	}
}

void save_game() {

	FILE *fp = fopen(DVONN_PREVIOUS_PATH, "w");

	fprintf(fp, "%d\n", phase);
	fprintf(fp, "%d\n", round_play);
	fprintf(fp, "%d\n", turn);

	for (int i = 0; i < player_no; ++i) {
		fprintf(fp, "%d\n", pieces_count[i]);
	}

	for (int i = 0; i < 3; ++i) {
		fprintf(fp, "%d ", reds_position[i][0]);
		fprintf(fp, "%d ", reds_position[i][1]);
	}

	fprintf(fp, "\n");

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; ++j) {
			fprintf(fp, "%d ", board[i][j]);
		}
		fprintf(fp, "\n");
	}
	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; ++j) {
			fprintf(fp, "%d ", board_owner[i][j]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}

void initialize_neighbors(int player_no) {
	FILE *fp;

	if (player_no == 2) {
		fp = fopen(DVONN_NEIGHBOR2_PATH, "r");
	} else {
		fp = fopen(DVONN_NEIGHBOR3_PATH, "r");
	}

	for (int i = 0; i < BOARD_ROWS; ++i) {
		for (int j = 0; j < columns_number[i]; ++j) {
			fscanf(fp, "%d", &board_neighbor_number[i][j]);
			for (int k = 0; k < board_neighbor_number[i][j]; ++k) {
				fscanf(fp, "%d", &board_neighbor[i][j][k * 2]);
				fscanf(fp, "%d", &board_neighbor[i][j][k * 2 + 1]);
			}
		}
	}

	fclose(fp);
}

void about_game_clicked(GtkWidget *button, gpointer data) {

}

