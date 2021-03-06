/*============================================================================
 Name        : GTK_Utils.c
 Author      : Amir Hossein Sojoodi
 Version     : 1
 Copyright   : GNU
 Description : Dvonn in GTK+
 ============================================================================*/

#include "Dvonn.h"

void show_hint(char *hint) {
	GtkLabel *label;
	label = GTK_LABEL(gtk_builder_get_object(builder, "hint"));
	gtk_label_set_text(label, hint);
	gtk_widget_show(GTK_WIDGET(label));
}

void show_red_labels() {
	for (int k = 0; k < 3; ++k) {
		if (board[reds_position[k][0]][reds_position[k][1]] > 1) {
			show_count_label_markup(reds_position[k][0], reds_position[k][1],
					board[reds_position[k][0]][reds_position[k][1]],
					"<span foreground=\"red\"><b>\%i</b></span>");
		}
	}
}

void show_count_label(int i, int j, int value) {
	GtkLabel *label;
	label = GTK_LABEL(
			gtk_builder_get_object(builder,
					g_strdup_printf("count_%i_%i", i, j)));
	gtk_label_set_text(label, g_strdup_printf("%i", value));
	gtk_widget_show(GTK_WIDGET(label));
}

void show_count_label_markup(int i, int j, int value, char *format) {
	GtkLabel *label;
	label = GTK_LABEL(
			gtk_builder_get_object(builder,
					g_strdup_printf("count_%i_%i", i, j)));
	//gtk_label_set_text(label, g_strdup_printf("%i", board[i][j]));
	gtk_label_set_markup(GTK_LABEL(label), g_strdup_printf(format, value));
	gtk_widget_show(GTK_WIDGET(label));

}

void hide_count_label(int i, int j) {
	GtkLabel *label;
	label = GTK_LABEL(
			gtk_builder_get_object(builder,
					g_strdup_printf("count_%i_%i", i, j)));
	gtk_widget_hide(GTK_WIDGET(label));
}

void show_image_turn(int turn) {
	GtkImage *image_turn;
	image_turn = GTK_IMAGE(gtk_builder_get_object(builder, turn_images[turn]));
	gtk_widget_show(GTK_WIDGET(image_turn));
}

void hide_all_image_turn() {
	GtkImage *image_turn;
	image_turn = GTK_IMAGE(gtk_builder_get_object(builder, turn_images[0]));
	gtk_widget_hide(GTK_WIDGET(image_turn));
	image_turn = GTK_IMAGE(gtk_builder_get_object(builder, turn_images[1]));
	gtk_widget_hide(GTK_WIDGET(image_turn));

	if (player_no > 2) {
		image_turn = GTK_IMAGE(gtk_builder_get_object(builder, turn_images[2]));
		gtk_widget_hide(GTK_WIDGET(image_turn));
	}
}

void show_image(int i, int j, char *file_name) {
	GtkImage *image;
	image = GTK_IMAGE(
			gtk_builder_get_object(builder,
					g_strdup_printf("image_%i_%i", i, j)));

	gtk_image_set_from_file(image, file_name);
}

void show_player_count_label(int turn, int value) {
	GtkLabel *label;
	label = GTK_LABEL(gtk_builder_get_object(builder, count_labels[turn]));
	gtk_label_set_text(label, g_strdup_printf("%i", value));
}

void show_label(char *name) {
	GtkLabel *label;
	label = GTK_LABEL(gtk_builder_get_object(builder, name));
	gtk_widget_show(GTK_WIDGET(label));
}

void hide_label(char *name) {
	GtkLabel *label;
	label = GTK_LABEL(gtk_builder_get_object(builder, name));
	gtk_widget_hide(GTK_WIDGET(label));
}

void show_button(char *name) {
	GtkButton *button;
	button = GTK_BUTTON(gtk_builder_get_object(builder, name));
	gtk_widget_show(GTK_WIDGET(button));
}

void hide_button(char *name) {
	GtkButton *button;
	button = GTK_BUTTON(gtk_builder_get_object(builder, name));
	gtk_widget_hide(GTK_WIDGET(button));
}

void initialize_css(gchar *css_path) {
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;

	provider = gtk_css_provider_new();
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);
	gtk_style_context_add_provider_for_screen(screen,
			GTK_STYLE_PROVIDER(provider),
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	gsize bytes_written, bytes_read;

	const gchar* home = css_path;

	GError *error = 0;

	gtk_css_provider_load_from_path(provider,
	g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),
	NULL);
	g_object_unref(provider);
}

void initialize_builder() {

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder,
			(player_no == 2) ? GLADE_2_PLAYER_PATH : GLADE_3_PLAYER_PATH,
			NULL);

	gtk_builder_connect_signals(builder, NULL);

}

void undo_redo() {

	GtkWidget *toolbar;
	GtkToolItem *undo, *load, *save, *about, *exit, *new;

	toolbar = GTK_WIDGET(gtk_builder_get_object(builder, "toolbar"));

	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);

	load = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	gtk_widget_set_name(GTK_WIDGET(undo), "load");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), load, -1);

	new = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new, -1);

	save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_widget_set_name(GTK_WIDGET(undo), "save");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);

	undo = gtk_tool_button_new_from_stock(GTK_STOCK_UNDO);
	gtk_widget_set_name(GTK_WIDGET(undo), "undo");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), undo, -1);

	about = gtk_tool_button_new_from_stock(GTK_STOCK_ABOUT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), about, -1);

	exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);

	g_signal_connect(G_OBJECT(new), "clicked", G_CALLBACK(new_game_clicked),
			new);

	g_signal_connect(G_OBJECT(load), "clicked", G_CALLBACK(load_game_clicked),
			load);

	g_signal_connect(G_OBJECT(save), "clicked", G_CALLBACK(save_game_clicked),
			save);

	g_signal_connect(G_OBJECT(undo), "clicked", G_CALLBACK(undo_clicked), undo);

	g_signal_connect(G_OBJECT(about), "clicked", G_CALLBACK(about_game_clicked),
			about);

	g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit),
			NULL);
}

void set_label_text(char *label_name, char *text) {
	GtkLabel *label;
	label = GTK_LABEL(gtk_builder_get_object(builder, label_name));
	gtk_label_set_text(label, text);
}

void combo_selected(GtkWidget *widget, gpointer window) {
	gchar *text = gtk_combo_box_text_get_active_text(
			GTK_COMBO_BOX_TEXT(widget));

	if (g_strcmp0(text, "2") == 0) {
		player_no = 2;
	} else if (g_strcmp0(text, "3") == 0) {
		player_no = 3;
	} else {
		player_no = 0;
	}
	g_free(text);
}

gboolean is_draw() {
	if (player_no == 2 && pieces_count[0] != pieces_count[1]) {
		return FALSE;
	} else if (player_no == 3
			&& (pieces_count[0] != pieces_count[1]
					|| pieces_count[0] != pieces_count[2]
					|| pieces_count[1] != pieces_count[2])) {
		return FALSE;
	}

	return TRUE;
}

int find_winner() {
	int winner = -1;
	int max = -1;
	for (int i = 0; i < player_no; ++i) {
		if (pieces_count[i] > max) {
			max = pieces_count[i];
			winner = i;
		}
	}
	return winner;
}

void show_end_game() {
	GtkMessageDialog *end_message;
	GtkImage *image;

	image = GTK_IMAGE(gtk_builder_get_object(builder, "winner"));

	end_message = GTK_MESSAGE_DIALOG(
			gtk_builder_get_object(builder, "end_game"));
	gtk_message_dialog_set_markup(end_message,
			"<span font_desc=\"Comic Sans MS Bold 15\">Game Ended!</span>");

	if (is_draw()) {
		gtk_message_dialog_format_secondary_markup(end_message,
				"<span font_desc=\"Comic Sans MS 11\">No one wins!</span>");
		gtk_image_set_from_file(image, HEXAGON_RED);

	} else {
		int winner = find_winner();
		if (winner == 0) {
			gtk_message_dialog_format_secondary_markup(end_message,
					"<span font_desc=\"Comic Sans MS 11\">White Player Won! Congratulations!</span>");
			gtk_image_set_from_file(image, HEXAGON_WHITE);
		} else if (winner == 1) {
			gtk_message_dialog_format_secondary_markup(end_message,
					"<span font_desc=\"Comic Sans MS 11\">Black Player Won! Congratulations!</span>");
			gtk_image_set_from_file(image, HEXAGON_BLACK);
		} else {
			gtk_message_dialog_format_secondary_markup(end_message,
					"<span font_desc=\"Comic Sans MS 11\">Yellow Player Won! Congratulations!</span>");
			gtk_image_set_from_file(image, HEXAGON_YELLOW);
		}
	}

//	gtk_message_dialog_set_image(end_message, GTK_WIDGET(image));
	gtk_widget_show(GTK_WIDGET(end_message));

}

void greetings() {
	GtkWidget *fixed;
	GtkWidget *combo;
	GtkWidget *label;
	GtkWidget *button, *load;
	GtkWidget *image;

	initialize_css(CSS_PATH_1);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Let's Play Dvonn!");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 425, 285);

	fixed = gtk_fixed_new();

	combo = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "2");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "3");
	gtk_fixed_put(GTK_FIXED(fixed), combo, 153, 250);

	button = gtk_button_new_with_label("New Game");
	gtk_widget_set_size_request(button, 80, 35);
	gtk_fixed_put(GTK_FIXED(fixed), button, 210, 246);

	load = gtk_button_new_with_label("Continue");
	gtk_widget_set_size_request(load, 80, 35);
	gtk_fixed_put(GTK_FIXED(fixed), load, 310, 246);

	image = gtk_image_new_from_file(DVONN_IMAGE);
	gtk_fixed_put(GTK_FIXED(fixed), image, 10, 10);

	gtk_container_add(GTK_CONTAINER(window), fixed);

	label = gtk_label_new("Number of players:");
	gtk_fixed_put(GTK_FIXED(fixed), label, 30, 255);

	g_signal_connect_swapped(G_OBJECT(window), "destroy",
			G_CALLBACK(gtk_main_quit), G_OBJECT(window));

	g_signal_connect(G_OBJECT(combo), "changed", G_CALLBACK(combo_selected),
			NULL);

	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(entry_clicked),
			NULL);

	g_signal_connect(G_OBJECT(load), "clicked", G_CALLBACK(load_game_clicked),
			NULL);

	player_no = 0; //default value

	phase = 1;

	gtk_widget_show_all(window);

	gtk_main();
}
