#include "install_screen.h"
#include "../utils/utils.h"

// This would interact with the backend installer process
static GtkWidget *progress_bar;
static GtkWidget *status_label;
static GtkWidget *log_view_scrolled_window;
static GtkWidget *log_text_view;
static GtkTextBuffer *log_buffer;
static GtkWidget *finish_button;
static GtkApplication *app_instance; // To close the app

// Simulate installation progress
static gboolean update_progress(gpointer user_data) {
    double fraction = gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progress_bar));
    fraction += 0.05; // Increment progress

    if (fraction > 1.0) {
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0);
        gtk_label_set_text(GTK_LABEL(status_label), "Installation Complete!");
        gtk_widget_set_sensitive(finish_button, TRUE);

        // Append to log
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(log_buffer, &end);
        gtk_text_buffer_insert(log_buffer, &end, "\nINFO: All tasks finished successfully.\n", -1);
        return G_SOURCE_REMOVE; // Stop the timer
    }

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), fraction);

    // Update status label and log (example messages)
    GtkTextIter end_iter;
    gtk_text_buffer_get_end_iter(log_buffer, &end_iter);
    if (fraction > 0.8 && fraction < 0.85) {
        gtk_label_set_text(GTK_LABEL(status_label), "Finalizing setup...");
        gtk_text_buffer_insert(log_buffer, &end_iter, "INFO: Finalizing system configuration...\n", -1);
    } else if (fraction > 0.5 && fraction < 0.55) {
        gtk_label_set_text(GTK_LABEL(status_label), "Installing bootloader...");
        gtk_text_buffer_insert(log_buffer, &end_iter, "INFO: Installing GRUB bootloader...\n", -1);
    } else if (fraction > 0.2 && fraction < 0.25) {
        gtk_label_set_text(GTK_LABEL(status_label), "Copying system files...");
        gtk_text_buffer_insert(log_buffer, &end_iter, "INFO: Copying base system files...\n", -1);
    }
    // Auto-scroll log view
    GtkAdjustment *vadj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(log_view_scrolled_window));
    gtk_adjustment_set_value(vadj, gtk_adjustment_get_upper(vadj));

    return G_SOURCE_CONTINUE; // Continue timer
}

static void start_installation_simulation(GtkWidget *widget, gpointer user_data) {
    gtk_widget_set_sensitive(widget, FALSE); // Disable the "Start Install" button (if it were previous screen's next)
    gtk_label_set_text(GTK_LABEL(status_label), "Starting installation...");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);
    gtk_widget_set_visible(log_view_scrolled_window, TRUE);

    // Clear previous log
    gtk_text_buffer_set_text(log_buffer, "INFO: Installation process started.\n", -1);

    g_timeout_add(500, update_progress, NULL); // Update every 500ms
}

static void on_finish_button_clicked(GtkWidget *widget, gpointer user_data) {
    if(app_instance) {
        g_application_quit(G_APPLICATION(app_instance));
    }
}

GtkWidget* create_install_screen(GtkApplication *app) {
    app_instance = app;
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "content-box");

    GtkWidget *icon = create_image_from_file("install_icon.png", 64, 64);
    if (icon) {
        gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(box), icon);
    } else {
        gtk_box_append(GTK_BOX(box), gtk_label_new("(Install Icon)"));
    }

    GtkWidget *title = gtk_label_new("Installing WaveOS"); // Or your product name
    gtk_widget_add_css_class(title, "welcome-subtitle");
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), title);

    status_label = gtk_label_new("Ready to install. This process cannot be undone.");
    gtk_widget_set_halign(status_label, GTK_ALIGN_CENTER);
    gtk_label_set_wrap(GTK_LABEL(status_label), TRUE);
    gtk_box_append(GTK_BOX(box), status_label);

    progress_bar = gtk_progress_bar_new();
    gtk_widget_set_margin_top(progress_bar, 10);
    gtk_widget_set_margin_bottom(progress_bar, 10);
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress_bar), TRUE);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), "0%");
    g_signal_connect(progress_bar, "notify::fraction", 
        G_CALLBACK(+[](GObject *object, GParamSpec *pspec, gpointer user_data) {
            char text[10];
            g_snprintf(text, sizeof(text), "%.0f%%", gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(object)) * 100);
            gtk_progress_bar_set_text(GTK_PROGRESS_BAR(object), text);
        }), NULL);
    gtk_box_append(GTK_BOX(box), progress_bar);

    // Log view (initially hidden or empty)
    log_text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(log_text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(log_text_view), FALSE);
    gtk_widget_add_css_class(log_text_view, "code"); // For monospaced font if defined in CSS
    log_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_text_view));

    log_view_scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(log_view_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(log_view_scrolled_window, -1, 150); // Height of 150px
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(log_view_scrolled_window), log_text_view);
    gtk_widget_set_vexpand(log_view_scrolled_window, FALSE); // Don't let it expand indefinitely
    gtk_widget_set_visible(log_view_scrolled_window, FALSE); // Initially hidden
    gtk_box_append(GTK_BOX(box), log_view_scrolled_window);

    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(box), spacer);

    // In a real scenario, the "Next" button from the previous screen (User Config)
    // would trigger the installation. Here, we simulate it being part of this screen's logic.
    // For now, the installation starts when this screen becomes visible (handled in main.c or by a signal).
    // We add a finish button that is enabled when installation completes.

    finish_button = gtk_button_new_with_label("Finish & Reboot"); // Or "Finish & Close"
    gtk_widget_set_sensitive(finish_button, FALSE); // Enabled upon completion
    g_signal_connect(finish_button, "clicked", G_CALLBACK(on_finish_button_clicked), NULL);
    gtk_widget_set_halign(finish_button, GTK_ALIGN_END);
    gtk_widget_set_margin_top(finish_button, 10);
    gtk_box_append(GTK_BOX(box), finish_button);

    // Automatically start the installation simulation when this screen is shown.
    // This is a simple way to trigger it. A more robust way would be a signal from the stack switch.
    // For now, we will call start_installation_simulation directly when the screen is created
    // or rely on the main application logic to call a function to start it.
    // Let's assume the `main.c` will call a function like `trigger_install_start()`
    // when this screen is made visible if we don't want it to start immediately.
    // For this example, let's add a small delay then start.
    g_timeout_add_seconds(1, (GSourceFunc)start_installation_simulation, NULL); // Start after 1 second

    return box;
}
