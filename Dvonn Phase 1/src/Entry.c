/*============================================================================
 Name        : Entry.c
 Author      : Amir Hossein Sojoodi
 Version     : 1
 Copyright   : GNU
 Description : Dvonn in GTK+
 ============================================================================*/
#include "Dvonn.h"

void entry_clicked(GtkWidget *widget, gpointer data) {

	if (player_no == 0) {
		return;
	}

	gtk_widget_destroy(window);

	GtkWidget *labels[player_no * 2];
	GtkWidget *entries[player_no * 2];

	GtkWidget *table;
	GtkWidget *button;
	GtkWidget *fixed;
	gchar *names[] = { "Black Player Name:", "Black Player Age:",
			"White Player Name:", "White Player Age:", "Yellow Player Name:",
			"Yellow Player Age:" };

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), "Enter Players name");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, (player_no == 2)? 200 : 270);
//	gtk_window_set_resizable(window, FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	fixed = gtk_fixed_new();

	button = gtk_button_new_with_label("Click To the Play!");
	gtk_widget_set_size_request(button, 80, 35);
	gtk_fixed_put(GTK_FIXED(fixed), button, 90, (player_no == 2)? 150 : 220);

	table = gtk_table_new(player_no * 2, 2, FALSE);
//	gtk_container_add(GTK_CONTAINER(window), table);

	for (int i = 0; i < player_no * 2; ++i) {

//		labels[i] = gtk_label_new(
//				g_strdup_printf("Player No.%i's name:", i + 1));
		labels[i] = gtk_label_new(names[i]);

		gtk_table_attach(GTK_TABLE(table), labels[i], 0, 1, i, i + 1,
				GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

		entries[i] = gtk_entry_new();

		gtk_entry_set_alignment(entries[i], 0.5);
		gtk_table_attach(GTK_TABLE(table), entries[i], 1, 2, i, i + 1,
				GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	}

	gtk_fixed_put(GTK_FIXED(fixed), table, 10, 10);

	gtk_container_add(GTK_CONTAINER(window), fixed);

	gtk_widget_show_all(window);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_clicked),
			(void * )entries);

	gtk_main();
}
