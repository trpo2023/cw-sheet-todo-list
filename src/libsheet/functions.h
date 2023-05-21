#pragma once

#include <gtk/gtk.h>

void update_task_list();

void add_new_task(GtkWidget* widget, gpointer data);

void update_task_status(GtkWidget* button, gpointer data);