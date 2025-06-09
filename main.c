#include <gtk/gtk.h>
#include <glib.h>
#include "installer.h"

static void activate(GtkApplication* app, gpointer user_data) {
    create_installer_window(app);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.waveinstaller.installer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
