#include <functions.h>

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
