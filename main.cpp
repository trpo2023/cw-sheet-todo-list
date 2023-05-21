#include <fstream>
#include <gtk/gtk.h>
#include <iostream>

const std::string TASK_FILE = "tasks.txt";

GtkWidget* task_list_box;

void update_task_list();

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

    // Create a scrolled window for the task list
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
