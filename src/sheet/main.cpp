#include <fstream>
#include <gtk/gtk.h>
#include <iostream>
#include <libsheet/functions.cpp>
#include <libsheet/functions.h>
#include <string>

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sheet");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GdkGeometry hints;
    hints.min_width = 500;
    hints.max_width = 500;
    hints.min_height = 400;
    hints.max_height = 800;
    gtk_window_set_geometry_hints(
            GTK_WINDOW(window),
            window,
            &hints,
            (GdkWindowHints)(GDK_HINT_MIN_SIZE | GDK_HINT_MAX_SIZE));

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget* task_entry = gtk_entry_new();
    g_signal_connect(
            task_entry, "activate", G_CALLBACK(add_new_task), task_entry);
    gtk_box_pack_start(GTK_BOX(vbox), task_entry, FALSE, FALSE, 0);

    GtkWidget* add_button = gtk_button_new_with_label("Add Task");
    g_signal_connect(
            add_button, "clicked", G_CALLBACK(add_new_task), task_entry);
    gtk_box_pack_start(GTK_BOX(vbox), add_button, FALSE, FALSE, 0);

    GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(
            GTK_SCROLLED_WINDOW(scrolled_window),
            GTK_POLICY_AUTOMATIC,
            GTK_POLICY_AUTOMATIC);

    task_list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(scrolled_window), task_list_box);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    update_task_list();

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}