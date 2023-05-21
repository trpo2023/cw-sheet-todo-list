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