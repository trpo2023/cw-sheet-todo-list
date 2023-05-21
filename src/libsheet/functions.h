#pragma once

#include <fstream>
#include <gtk/gtk.h>
#include <iostream>

const std::string TASK_FILE = "tasks.txt";

GtkWidget* task_list_box;

void update_task_list();

void add_new_task(GtkWidget* widget, gpointer data);