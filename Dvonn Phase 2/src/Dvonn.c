/*
 ============================================================================
 Name        : Dvonn.c
 Author      : Amir Hossein Sojoodi
 Version     : 1
 Copyright   : GNU
 Description : Dvonn in GTK+
 ============================================================================
 */

#include "Dvonn.h"

gboolean is_full(int i, int j) {
	if (board[i][j] > 0) {
		return TRUE;
	}
	return FALSE;
}

int board_count(int i, int j) {
	return board[i][j];
}

gboolean is_valid_age(char *age_str) {
	int age = 0;
	if (strlen(age_str) == 0) {
		return FALSE;
	}
	for (int i = 0; i < strlen(age_str); ++i) {
		if (!isdigit(age_str[i])) {
			return FALSE;
		}
	}
	age = atoi(age_str);
	if (age < 8) {
		return FALSE;
	}
	return TRUE;
}
gboolean is_valid_move(int selected_i, int selected_j, int i, int j) {
	if (selected_i == i) // Both of them in one row
		return TRUE;

	if (selected_i <= 2 && i <= 2) { // Both of them are in upper half
		if ((selected_i - i) == (selected_j - j)) {
			return TRUE;
		}
		if (selected_j == j) {
			return TRUE;
		}
	} else if (selected_i >= 2 && i >= 2) { // Both of them in lower half
		if ((selected_i - i) == -(selected_j - j)) {
			return TRUE;
		}
		if (selected_j == j) {
			return TRUE;
		}
	} else {
		int a = selected_i - 2;
		int tmp_i, tmp_j;
		if (a < 0) { //selected piece is in upper half
			a = -a;
			tmp_i = selected_i + a; //tmp_i is always 2
			tmp_j = selected_j;
			if (i - tmp_i == -(j - tmp_j)) {
				return TRUE;
			}
			tmp_i = selected_i + a;
			tmp_j = selected_j + a;
			if (j == tmp_j) {
				return TRUE;
			}
		} else { //selected piece is in upper half
			tmp_i = selected_i - a; //tmp_i is always 2
			tmp_j = selected_j;
			if (i - tmp_i == (j - tmp_j)) {
				return TRUE;
			}
			tmp_i = selected_i - a;
			tmp_j = selected_j + a;
			if (j == tmp_j) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

int distance(int selected_i, int selected_j, int i, int j) {
	if (selected_i == i) { // Both of them in one row
		if (selected_j == j) {
			return 0;
		}
		return abs(selected_j - j);
	}

	if (selected_i <= 2 && i <= 2) { // Both of them are in upper half
		if ((selected_i - i) == (selected_j - j)) {
			return abs(selected_j - j);
		}
		if (selected_j == j) {
			return abs(selected_i - i);
		}
	} else if (selected_i >= 2 && i >= 2) { // Both of them in lower half
		if ((selected_i - i) == -(selected_j - j)) {
			return abs(selected_j - j);
		}
		if (selected_j == j) {
			return abs(selected_i - i);
		}
	} else {
		int a = selected_i - 2;
		int tmp_i, tmp_j;
		if (a < 0) { //selected piece is in upper half
			a = -a;
			tmp_i = selected_i + a; //tmp_i is always 2
			tmp_j = selected_j;
			if (i - tmp_i == -(j - tmp_j)) {
				return abs(selected_i - i);
			}
			tmp_i = selected_i + a;
			tmp_j = selected_j + a;
			if (j == tmp_j) {
				return abs(selected_i - i);
			}
		} else { //selected piece is in upper half
			tmp_i = selected_i - a; //tmp_i is always 2
			tmp_j = selected_j;
			if (i - tmp_i == (j - tmp_j)) {
				return abs(selected_i - i);
			}
			tmp_i = selected_i - a;
			tmp_j = selected_j + a;
			if (j == tmp_j) {
				return abs(selected_i - i);
			}
		}
	}
	return -1;
}

void update_reds_position(int selected_i, int selected_j, int i, int j) {
	for (int k = 0; k < 3; ++k) {
		if (reds_position[k][0] == selected_i
				&& reds_position[k][1] == selected_j) {
			reds_position[k][0] = i;
			reds_position[k][1] = j;
		}
	}
	show_red_labels();
}

G_MODULE_EXPORT void on_end_game_response(GtkWidget *message, gpointer data){
	gtk_widget_destroy(GTK_WIDGET(message));
	new_game_clicked(message, data);
}

G_MODULE_EXPORT void on_dvonn_board_2_player_destroy(GtkWidget *window,
		gpointer data) {
	gtk_main_quit();
}

G_MODULE_EXPORT void pass_clicked(GtkWidget *button, gpointer data) {

	if (selected_i != -1 && selected_j == -1) {
		show_count_label(selected_i, selected_j, board[selected_i][selected_j]);
	}
	selected_i = -1;
	selected_j = -1;

	turn = (turn + 1) % player_no;

	hide_all_image_turn();

	show_image_turn(turn);

	pass_no++;

	if(pass_no == player_no){
		show_end_game();
	}
}

G_MODULE_EXPORT void on_button_clicked(GtkWidget *button, gpointer data) {
	if (phase == 1) {
		on_button_clicked_phase1(button, data);
	} else {
		on_button_clicked_phase2(button, data);
	}
}

void on_button_clicked_phase2(GtkWidget *button, gpointer data) {

	int i, j;

	i = GPOINTER_TO_INT(g_object_get_data (G_OBJECT (button), "button_i"));
	j = GPOINTER_TO_INT(g_object_get_data (G_OBJECT (button), "button_j"));

	hide_label("hint");

	if (selected_i == -1) { //select phase

		if (board_owner[i][j] == 3) {

			show_hint("You cannot move red pieces!");

		} else if (board_owner[i][j] != turn) {

			show_hint("You can only move your own pieces!");

		} else {

			show_count_label_markup(i, j, board[i][j],
					"<span foreground=\"yellow\"><b>\%i</b></span>");
			selected_i = i;
			selected_j = j;

		}
	} else { //move phase

		if (board_owner[i][j] == -1) {
			show_hint("You cannot move to an empty space!");
			return;
		}
		if (!is_valid_move(selected_i, selected_j, i, j)) {
			show_hint("This is not a valid move!");
			return;
		}
		int dist = distance(selected_i, selected_j, i, j);
		if (dist == 0) {
			show_count_label(selected_i, selected_j,
					board[selected_i][selected_j]);
			selected_i = -1;
			selected_j = -1;
			return;
		}
		if (dist > board[selected_i][selected_j]) {
			show_hint("You don't not have enough pieces!");
			return;
		}
		if (dist < board[selected_i][selected_j]) {
			show_hint("You have more pieces!");
			return;
		}

		save_game();
		if (board_owner[i][j] != 3) {
			pieces_count[board_owner[i][j]] -= board[i][j];
		}
		pieces_count[board_owner[selected_i][selected_j]] += board[i][j];

		board[i][j] += board[selected_i][selected_j];
		board[selected_i][selected_j] = 0;

		show_count_label(i, j, board[i][j]);
		hide_count_label(selected_i, selected_j);

		show_image(i, j, button_images[turn]);
		show_image(selected_i, selected_j, HEXAGON_EMPTY);

		show_player_count_label(turn, pieces_count[turn]);
		if (board_owner[i][j] != 3) {
			show_player_count_label(board_owner[i][j],
					pieces_count[board_owner[i][j]]);
		}

		update_reds_position(selected_i, selected_j, i, j);
		board_owner[selected_i][selected_j] = -1;
		board_owner[i][j] = turn;
		selected_i = -1;
		selected_j = -1;

		pass_no = 0;

		remove_disconnected();

		turn = (turn + 1) % player_no;

		hide_all_image_turn();

		show_image_turn(turn);
	}
	return;
}

void on_button_clicked_phase1(GtkWidget *button, gpointer data) {

	int i, j;

	i = GPOINTER_TO_INT(g_object_get_data (G_OBJECT (button), "button_i"));
	j = GPOINTER_TO_INT(g_object_get_data (G_OBJECT (button), "button_j"));

//sprintf(line, "button_%d_%d clicked!\n", i, j);

	hide_label("hint");

	if (is_full(i, j)) {

		show_hint("This place is full!");

		return;
	}

	save_game();

	if (round_play < 3) {

		show_image(i, j, HEXAGON_RED);

		board_owner[i][j] = 3; //red

		reds_position[round_play][0] = i;
		reds_position[round_play][1] = j;

	} else {

		show_image(i, j, button_images[turn]);

		show_player_count_label(turn, pieces_count[turn] - 1);

		pieces_count[turn]--;

		board_owner[i][j] = turn;
	}

	board[i][j] = 1;
	round_play++;
	turn = (turn + 1) % player_no;

	hide_all_image_turn();
	show_image_turn(turn);

	int tmpFlag = 0;
	for (int i = 0; i < player_no; ++i) {
		if (pieces_count[i] == 0) {
			continue;
		} else {
			tmpFlag = 1;
			break;
		}
	}
	if (tmpFlag == 0) {
		phase = 2;
		hide_label("phase1");
		show_label("phase2");
		if (player_no == 2) {
			pieces_count[0] = pieces_count[1] = TWO_PLAYERS_PIECE_COUNT;
			show_player_count_label(0, pieces_count[0]);
			show_player_count_label(1, pieces_count[1]);
		} else {
			pieces_count[0] = pieces_count[1] = pieces_count[2] =
			THREE_PLAYERS_PIECE_COUNT;
			show_player_count_label(0, pieces_count[0]);
			show_player_count_label(1, pieces_count[1]);
			show_player_count_label(2, pieces_count[2]);
		}
		turn = 0;
		hide_all_image_turn();
		show_image_turn(turn);
		show_button("pass");
		selected_i = -1;
		selected_j = -1;
	}
}

void set_buttons_data() {

	board = (int **) malloc(sizeof(int *) * BOARD_ROWS);
	board_owner = (int **) malloc(sizeof(int *) * BOARD_ROWS);

	columns_number[0] = (player_no == 2) ? 9 : 10;
	columns_number[1] = columns_number[0] + 1;
	columns_number[2] = columns_number[1] + 1;
	columns_number[3] = columns_number[1];
	columns_number[4] = columns_number[0];

	for (int i = 0; i < BOARD_ROWS; ++i) {
		board[i] = (int *) malloc(sizeof(int) * columns_number[i]);
		board_owner[i] = (int *) malloc(sizeof(int) * columns_number[i]);
		for (int j = 0; j < columns_number[i]; j++) {
			sprintf(line, "button_%d_%d", i, j);
			g_object_set_data(G_OBJECT(gtk_builder_get_object(builder, line)),
					"button_i", GINT_TO_POINTER(i));
			g_object_set_data(G_OBJECT(gtk_builder_get_object(builder, line)),
					"button_j", GINT_TO_POINTER(j));

			board[i][j] = 0;
			board_owner[i][j] = -1;
		}
	}

	if (player_no == 2) {
		pieces_count[0] = pieces_count[1] = TWO_PLAYERS_PIECE_COUNT;
	} else {
		pieces_count[0] = pieces_count[1] = pieces_count[2] =
		THREE_PLAYERS_PIECE_COUNT;
	}

}

void start_clicked(GtkWidget *button, gpointer data) {
	GtkMessageDialog *age_message;
	GtkWidget *entry;
	GtkStatusbar *status_bar;

	initialize_builder();

	for (int i = 0; i < player_no; ++i) {
		entry = ((GtkWidget **) data)[i * 2];
		strcpy(player_names[i], gtk_entry_get_text(GTK_ENTRY(entry)));
		if (strlen(player_names[i]) == 0) {
			age_message = GTK_MESSAGE_DIALOG(
					gtk_builder_get_object(builder, "name_message"));
			gtk_widget_show(GTK_WIDGET(age_message));
			return;
		}
		entry = ((GtkWidget **) data)[i * 2 + 1];
		strcpy(line, gtk_entry_get_text(GTK_ENTRY(entry)));
		if (!is_valid_age(line)) {
			age_message = GTK_MESSAGE_DIALOG(
					gtk_builder_get_object(builder, "age_message"));
			gtk_widget_show(GTK_WIDGET(age_message));
			return;
		}
	}

	gtk_widget_destroy(window);

	window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_NAME));

	initialize_css(CSS_PATH);

	set_buttons_data();

	initialize_neighbors(player_no);

	turn = 0;

	round_play = 0;

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

	hide_label("hint");

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

	show_image_turn(0);

	hide_button("pass");

	gtk_main();
}

int main(int argc, char *argv[]) {

	gtk_init(&argc, &argv);

//	g_log_set_always_fatal(G_LOG_FLAG_FATAL);
	fclose(stderr);
	fclose(stdout);
	fclose(stdin);

	greetings();

	return 0;
}
