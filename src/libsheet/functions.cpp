#include <fstream>
#include <gtk/gtk.h>
#include <iostream>
#include <libsheet/functions.h>
#include <string>

std::string File_name = "tasks.txt";
GtkWidget* task_list_box;

int checking_entry(std::string str)
{
    if (str != "" && str != "Ошибка! Поле ввода пустое!")
        return 0;
    else
        return 1;
}

int checking_is_open_file(std::string File_name)
{
    std::ifstream file(File_name);
    if (file.is_open()) {
        file.close();
        return 0;
    } else
        return 1;
}

void update_task_list();

void add_new_task(GtkWidget* widget, gpointer data)
{
    const gchar* task_name = gtk_entry_get_text(GTK_ENTRY(data));
    std::string str = task_name;
    int check_entry = checking_entry(str);

    if (check_entry == 0) {
        std::ofstream file(File_name, std::ios_base::app);
        int check_file = checking_is_open_file(File_name);
        if (check_file == 0) {
            file << "0/" << task_name << "\n";
            file.close();

            gtk_entry_set_text(GTK_ENTRY(data), "");

            update_task_list();
        } else
            return;
    } else {
        gtk_entry_set_text(GTK_ENTRY(data), "Ошибка! Поле ввода пустое!");
    }
}

void update_task_status(GtkWidget* button, gpointer data)
{
    int index = GPOINTER_TO_INT(data);

    std::ifstream file(File_name);
    int check_file = checking_is_open_file(File_name);
    if (check_file == 0) {
        std::string line;
        std::string tasks;
        int current_index = 0;
        while (std::getline(file, line)) {
            if (line.empty())
                continue;

            int status = line[0] - '0';
            std::string task_name = line.substr(2);

            if (current_index == index)
                status = 1 - status;

            tasks += std::to_string(status) + "/" + task_name + "\n";

            current_index++;
        }
        file.close();

        std::ofstream outfile(File_name);
        int check_file = checking_is_open_file(File_name);
        if (check_file == 0) {
            outfile << tasks;
            outfile.close();

            update_task_list();
        } else
            return;

    } else
        return;
}

void delete_task(GtkWidget* button, gpointer data)
{
    int index = GPOINTER_TO_INT(data);

    std::ifstream file(File_name);
    int check_file = checking_is_open_file(File_name);
    if (check_file == 0) {
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

        std::ofstream outfile(File_name);
        int check_file = checking_is_open_file(File_name);
        if (check_file == 0) {
            outfile << tasks;
            outfile.close();

            update_task_list();
        } else
            return;

    } else
        return;
}

void update_task_list()
{
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(task_list_box));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    std::ifstream file(File_name);
    int check_file = checking_is_open_file(File_name);
    if (check_file == 0) {
        std::string line;
        int index = 0;
        while (std::getline(file, line)) {
            if (line.empty())
                continue;

            int status = line[0] - '0';
            std::string task_name = line.substr(2);

            if (status == 0) {
                GtkWidget* task_box
                        = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

                GtkWidget* task_label = gtk_label_new(task_name.c_str());
                gtk_box_pack_start(
                        GTK_BOX(task_box), task_label, TRUE, TRUE, 0);

                GtkWidget* task_button = gtk_button_new_with_label("✔");
                g_signal_connect(
                        task_button,
                        "clicked",
                        G_CALLBACK(update_task_status),
                        GINT_TO_POINTER(index));
                gtk_box_pack_start(
                        GTK_BOX(task_box), task_button, FALSE, FALSE, 0);

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
    } else
        return;
}