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

G_MODULE_EXPORT void on_dvonn_board_2_player_destroy(GtkWidget *window,
		gpointer data) {
	gtk_main_quit();
}

G_MODULE_EXPORT void on_button_clicked(GtkWidget *button, gpointer data) {

	GtkImage *image, *image_turn;
	GtkLabel *label;
	int i, j;

	i = GPOINTER_TO_INT(g_object_get_data (G_OBJECT (button), "button_i"));
	j = GPOINTER_TO_INT(g_object_get_data (G_OBJECT (button), "button_j"));

	sprintf(line, "button_%d_%d clicked!\n", i, j);

	if (is_full(i, j)) {
		return;
	}

	image = GTK_IMAGE(
			gtk_builder_get_object(builder,
					g_strdup_printf("image_%i_%i", i, j)));

	if (round_play < 3) {
		gtk_image_set_from_file(image, HEXAGON_RED);
	} else {
		if (turn == 0) {

			gtk_image_set_from_file(image, HEXAGON_BLACK);
			label = GTK_LABEL(gtk_builder_get_object(builder, "black_count"));
			gtk_label_set_text(label,
					g_strdup_printf("%i", pieces_count[0] - 1));
		} else if (turn == 1) {

			gtk_image_set_from_file(image, HEXAGON_WHITE);
			label = GTK_LABEL(gtk_builder_get_object(builder, "white_count"));
			gtk_label_set_text(label,
					g_strdup_printf("%i", pieces_count[1] - 1));

		} else if (turn == 2) {

			gtk_image_set_from_file(image, HEXAGON_YELLOW);
			label = GTK_LABEL(gtk_builder_get_object(builder, "yellow_count"));
			gtk_label_set_text(label,
					g_strdup_printf("%i", pieces_count[2] - 1));

		}
		pieces_count[turn]--;
	}
//	g_print(line);

	round_play++;
	turn = (turn + 1) % player_no;
	board[i][j]++;

	image_turn = GTK_IMAGE(gtk_builder_get_object(builder, "black_turn_image"));
	gtk_widget_hide(GTK_WIDGET(image_turn));
	image_turn = GTK_IMAGE(gtk_builder_get_object(builder, "white_turn_image"));
	gtk_widget_hide(GTK_WIDGET(image_turn));

	if (player_no > 2) {
		image_turn = GTK_IMAGE(
				gtk_builder_get_object(builder, "yellow_turn_image"));
		gtk_widget_hide(GTK_WIDGET(image_turn));
	}

	if (turn == 0) {
		image_turn = GTK_IMAGE(
				gtk_builder_get_object(builder, "black_turn_image"));
		gtk_widget_show(GTK_WIDGET(image_turn));
	} else if (turn == 1) {
		image_turn = GTK_IMAGE(
				gtk_builder_get_object(builder, "white_turn_image"));
		gtk_widget_show(GTK_WIDGET(image_turn));
	} else if (turn == 2) {
		image_turn = GTK_IMAGE(
				gtk_builder_get_object(builder, "yellow_turn_image"));
		gtk_widget_show(GTK_WIDGET(image_turn));
	}
}

void combo_selected(GtkWidget *widget, gpointer window) {
	gchar *text = gtk_combo_box_text_get_active_text(
			GTK_COMBO_BOX_TEXT(widget));
//	gtk_label_set_text(GTK_LABEL(window), text);
	if (g_strcmp0(text, "2") == 0) {
		player_no = 2;
	} else if (g_strcmp0(text, "3") == 0) {
		player_no = 3;
	} else {
		player_no = 0;
	}
	g_free(text);
}

void set_buttons_data() {

	int columns[5];

	board = (int **) malloc(sizeof(int *) * 5);

	columns[0] = (player_no == 2) ? 9 : 10;
	columns[1] = columns[0] + 1;
	columns[2] = columns[1] + 1;
	columns[3] = columns[1];
	columns[4] = columns[0];

	for (int i = 0; i < 5; ++i) {
		board[i] = (int *) malloc(sizeof(int) * columns[i]);
		for (int j = 0; j < columns[i]; j++) {
			sprintf(line, "button_%d_%d", i, j);
//			gchar *name = g_strdup_printf("num_%i", i);
			g_object_set_data(G_OBJECT(gtk_builder_get_object(builder, line)),
					"button_i", GINT_TO_POINTER(i));
			g_object_set_data(G_OBJECT(gtk_builder_get_object(builder, line)),
					"button_j", GINT_TO_POINTER(j));

			board[i][j] = 0;
		}
	}

	pieces_count = (int *) malloc(sizeof(int) * player_no);
	if (player_no == 2) {
		pieces_count[0] = pieces_count[1] = 23;
	} else {
		pieces_count[0] = pieces_count[1] = pieces_count[2] = 17;
	}

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
	window = GTK_WIDGET(gtk_builder_get_object(builder, WINDOW_NAME));

	gtk_builder_connect_signals(builder, NULL);

}

void undo_redo() {

	GtkWidget *toolbar;
	GtkToolItem *undo;
	GtkToolItem *redo;
	GtkToolItem *exit;

	toolbar = GTK_WIDGET(gtk_builder_get_object(builder, "toolbar"));

	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);

	undo = gtk_tool_button_new_from_stock(GTK_STOCK_UNDO);
	gtk_widget_set_name(GTK_WIDGET(undo), "undo");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), undo, -1);

	redo = gtk_tool_button_new_from_stock(GTK_STOCK_REDO);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), redo, -1);

	exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);

	g_signal_connect(G_OBJECT(undo), "clicked", G_CALLBACK(undo_redo), redo);

	g_signal_connect(G_OBJECT(redo), "clicked", G_CALLBACK(undo_redo), undo);

	g_signal_connect(G_OBJECT(exit), "clicked", G_CALLBACK(gtk_main_quit),
			NULL);
}

void button_clicked(GtkWidget *button, gpointer data) {

	GtkWidget *entries[player_no];
	GtkImage *image_turn;
	GtkLabel *labels[player_no], *label;
	gchar texts[player_no][100];
	GtkStatusbar *status_bar;

	for (int i = 0; i < player_no; ++i) {
		entries[i] = ((GtkWidget **) data)[i * 2];
		strcpy(texts[i], gtk_entry_get_text(GTK_ENTRY(entries[i])));
		if (strlen(texts[i]) == 0) {
			return;
		}
	}

	gtk_widget_destroy(window);

	initialize_css(CSS_PATH);

	initialize_builder();

	set_buttons_data();

	turn = 0;

	round_play = 0;

	labels[0] = GTK_LABEL(gtk_builder_get_object(builder, "black_player"));
	gtk_label_set_text(labels[0], texts[0]);
	labels[1] = GTK_LABEL(gtk_builder_get_object(builder, "white_player"));
	gtk_label_set_text(labels[1], texts[1]);

	sprintf(line, "%s vs. %s", texts[0], texts[1]);
	if (player_no > 2) {
		labels[2] = GTK_LABEL(gtk_builder_get_object(builder, "yellow_player"));
		gtk_label_set_text(labels[2], texts[2]);
		sprintf(line, "%s vs. %s", line, texts[2]);
	}

	label = GTK_LABEL(gtk_builder_get_object(builder, "black_count"));
	gtk_label_set_text(label, g_strdup_printf("%i", pieces_count[0]));

	label = GTK_LABEL(gtk_builder_get_object(builder, "white_count"));
	gtk_label_set_text(label, g_strdup_printf("%i", pieces_count[1]));

	if (player_no > 2) {
		label = GTK_LABEL(gtk_builder_get_object(builder, "yellow_count"));
		gtk_label_set_text(label, g_strdup_printf("%i", pieces_count[2]));
	}

	status_bar = GTK_STATUSBAR(gtk_builder_get_object(builder, "statusbar"));

	gtk_statusbar_push(status_bar,
			gtk_statusbar_get_context_id(status_bar, line), line);

	gtk_window_set_title(GTK_WINDOW(window), "Let's Play Dvonn!");

	undo_redo();

	gtk_widget_show_all(window);

	image_turn = GTK_IMAGE(
			(gtk_builder_get_object(builder, "white_turn_image")));
	gtk_widget_hide(GTK_WIDGET(image_turn));

	if (player_no > 2) {
		image_turn = GTK_IMAGE(
				gtk_builder_get_object(builder, "yellow_turn_image"));
		gtk_widget_hide(GTK_WIDGET(image_turn));
	}

	gtk_main();
}

void greetings() {
	GtkWidget *fixed;
	GtkWidget *combo;
	GtkWidget *label;
	GtkWidget *button;
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
	gtk_fixed_put(GTK_FIXED(fixed), combo, 233, 250);

	button = gtk_button_new_with_label("Start!");
	gtk_widget_set_size_request(button, 80, 35);
	gtk_fixed_put(GTK_FIXED(fixed), button, 300, 246);

	image = gtk_image_new_from_file(DVONN);
	gtk_fixed_put(GTK_FIXED(fixed), image, 10, 10);

	gtk_container_add(GTK_CONTAINER(window), fixed);

	label = gtk_label_new("Select Number of players:");
	gtk_fixed_put(GTK_FIXED(fixed), label, 50, 255);

	g_signal_connect_swapped(G_OBJECT(window), "destroy",
			G_CALLBACK(gtk_main_quit), G_OBJECT(window));

	g_signal_connect(G_OBJECT(combo), "changed", G_CALLBACK(combo_selected),
			NULL);

	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(entry_clicked),
			NULL);

	player_no = 0; //default value

	gtk_widget_show_all(window);

	gtk_main();

}

int main(int argc, char *argv[]) {

	gtk_init(&argc, &argv);

	g_log_set_always_fatal(G_LOG_FLAG_FATAL);

//	fclose(stderr);

	greetings();

	return 0;
}
