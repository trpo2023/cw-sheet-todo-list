#include <fstream>
#include <functions.h>
#include <gtk/gtk.h>
#include <iostream>
#include <string>

extern const std::string TASK_FILE;

extern GtkWidget* task_list_box;

void update_task_list();

void add_new_task(GtkWidget* widget, gpointer data)
{
    const gchar* task_name = gtk_entry_get_text(GTK_ENTRY(data));

    if (task_name && *task_name) {
        std::ofstream file(TASK_FILE, std::ios_base::app);
        file << "0/" << task_name << "\n";
        file.close();

        gtk_entry_set_text(GTK_ENTRY(data), "");

        update_task_list();
    }
}

void update_task_status(GtkWidget* button, gpointer data)
{
    // Get the index of the task from the button's data
    int index = GPOINTER_TO_INT(data);

    // Read tasks from file and update the selected task's status
    std::ifstream file(TASK_FILE);
    std::string line;
    std::string tasks;
    int current_index = 0;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        int status = line[0] - '0';
        std::string task_name = line.substr(2);

        if (current_index == index)
            status = 1 - status; // Toggle the status

        tasks += std::to_string(status) + "/" + task_name + "\n";

        current_index++;
    }
    file.close();

    std::ofstream outfile(TASK_FILE);
    outfile << tasks;
    outfile.close();

    update_task_list();
}

void delete_task(GtkWidget* button, gpointer data)
{
    // Get the index of the task from the button's data
    int index = GPOINTER_TO_INT(data);

    // Read tasks from file and remove the selected task
    std::ifstream file(TASK_FILE);
    std::string line;
    std::string tasks;
    int current_index = 0;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        if (current_index != index)
            tasks += line + "\n";

        current_index++;
    }
    file.close();

    std::ofstream outfile(TASK_FILE);
    outfile << tasks;
    outfile.close();

    update_task_list();
}

void update_task_list()
{
    // Clear the existing task list
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(task_list_box));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    // Read tasks from file and display outstanding tasks
    std::ifstream file(TASK_FILE);
    std::string line;
    int index = 0;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;

        int status = line[0] - '0';
        std::string task_name = line.substr(2);

        if (status == 0) {
            GtkWidget* task_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

            GtkWidget* task_label = gtk_label_new(task_name.c_str());
            gtk_box_pack_start(GTK_BOX(task_box), task_label, TRUE, TRUE, 0);

            GtkWidget* task_button = gtk_button_new_with_label("✔");
            g_signal_connect(
                    task_button,
                    "clicked",
                    G_CALLBACK(update_task_status),
                    GINT_TO_POINTER(index));
            gtk_box_pack_start(GTK_BOX(task_box), task_button, FALSE, FALSE, 0);

            GtkWidget* delete_button = gtk_button_new_with_label("✘");
            g_signal_connect(
                    delete_button,
                    "clicked",
                    G_CALLBACK(delete_task),
                    GINT_TO_POINTER(index));
            gtk_box_pack_start(
                    GTK_BOX(task_box), delete_button, FALSE, FALSE, 0);

            gtk_box_pack_start(
                    GTK_BOX(task_list_box), task_box, FALSE, FALSE, 0);
        }

        index++;
    }
    file.close();

    gtk_widget_show_all(task_list_box);
}