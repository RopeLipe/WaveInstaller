#include "install_screen.h"
#include "user_screen.h" // To navigate back (though less common at this stage)
#include <gtk/gtk.h>

// Forward declarations (if any navigation is added later)
extern void switch_screen(GtkWidget *new_screen);
// extern void navigate_to_some_other_screen(GtkButton *button, gpointer user_data); // Example
extern void navigate_to_user_screen(GtkButton *button, gpointer user_data); // If a back button is desired

// Placeholder: In a real installer, this function would be called when the installation starts.
// It would then update the progress bar and status label based on actual installation steps.
static gboolean update_progress(gpointer user_data) {
    GtkProgressBar *progress_bar = GTK_PROGRESS_BAR(user_data);
    double current_fraction = gtk_progress_bar_get_fraction(progress_bar);
    double new_fraction = current_fraction + 0.01; // Increment by 1%

    if (new_fraction > 1.0) {
        new_fraction = 1.0;
        gtk_progress_bar_set_text(progress_bar, "Installation Complete!");
        // Here you might enable a "Finish" button or auto-close the installer
        // For now, we just stop the timer.
        return G_SOURCE_REMOVE; // Stop the timer
    }

    gtk_progress_bar_set_fraction(progress_bar, new_fraction);
    char buffer[100];
    sprintf(buffer, "Installing... %.0f%%", new_fraction * 100);
    gtk_progress_bar_set_text(progress_bar, buffer);

    return G_SOURCE_CONTINUE; // Keep timer running
}

// This function would be called when the "Next" button from the user_screen is clicked.
// Or, if there's a summary screen before this, then from that summary screen.
void start_actual_installation(GtkWidget *progress_bar) {
    // Disable back button if there is one, or other navigation controls.
    // gtk_widget_set_sensitive(back_button_widget, FALSE);

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), "Starting installation...");

    // Simulate installation progress with a timer
    // In a real installer, this would be driven by actual installation events/steps.
    g_timeout_add(100, update_progress, progress_bar); // Update every 100ms

    // TODO: Here you would trigger the backend installation logic.
    // This is where the core installation tasks (copying files, setting up bootloader, etc.) happen.
    // The backend would need to communicate its progress back to the UI to update the progress bar realistically.
}

GtkWidget *create_install_screen(void) {
    GtkWidget *box;
    GtkWidget *title_label;
    GtkWidget *summary_label; // Optional: To show a summary of selections
    GtkWidget *progress_bar;
    GtkWidget *status_label; // For detailed status messages below progress bar
    GtkWidget *buttons_box; // For Finish/Close or Restart buttons
    // GtkWidget *back_button; // Usually disabled or hidden at this stage

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(box, 30);
    gtk_widget_add_css_class(box, "install-screen-box");

    title_label = gtk_label_new("Installation Progress");
    gtk_widget_add_css_class(title_label, "title-label");
    gtk_widget_set_margin_bottom(title_label, 20);
    gtk_box_append(GTK_BOX(box), title_label);

    // Optional: A brief summary of what will be installed/configured
    // summary_label = gtk_label_new("Summary: Installing to /dev/sda, User: john, Lang: EN...");
    // gtk_widget_set_margin_bottom(summary_label, 15);
    // gtk_box_append(GTK_BOX(box), summary_label);

    progress_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress_bar), TRUE);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), "Preparing to install...");
    gtk_widget_set_size_request(progress_bar, 400, -1); // Set a decent width
    gtk_widget_set_margin_start(progress_bar, 50);
    gtk_widget_set_margin_end(progress_bar, 50);
    gtk_widget_set_margin_bottom(progress_bar, 10);
    gtk_widget_add_css_class(progress_bar, "install-progress-bar");
    gtk_box_append(GTK_BOX(box), progress_bar);

    status_label = gtk_label_new("Please wait while the system is being installed.");
    gtk_widget_add_css_class(status_label, "status-label");
    gtk_widget_set_margin_bottom(status_label, 30);
    gtk_box_append(GTK_BOX(box), status_label);

    // Placeholder for buttons like "Finish" or "Restart" that would appear upon completion/failure
    buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);
    // Example: Add a finish button (initially insensitive or hidden)
    // GtkWidget *finish_button = gtk_button_new_with_label("Finish");
    // gtk_widget_set_sensitive(finish_button, FALSE);
    // g_signal_connect_swapped(finish_button, "clicked", G_CALLBACK(gtk_window_destroy), main_window_ptr); // Assuming main_window_ptr is accessible
    // gtk_box_append(GTK_BOX(buttons_box), finish_button);
    gtk_box_append(GTK_BOX(box), buttons_box);

    // IMPORTANT: Trigger the actual installation process
    // This should ideally happen *after* the screen is shown and ready.
    // For simplicity, we call it here. A better way is to connect to a signal like "map" or use g_idle_add.
    // We pass the progress_bar to the function that will manage its updates.
    // In a real app, you might pass more context or use a dedicated installation manager object.
    start_actual_installation(progress_bar); 
    // Note: If you want a "Back" button here, it's unusual but possible.
    // It would typically only be active *before* installation starts.
    // GtkWidget *back_button = gtk_button_new_with_label("Back");
    // gtk_widget_add_css_class(back_button, "back-button");
    // g_signal_connect(back_button, "clicked", G_CALLBACK(navigate_to_user_screen), NULL);
    // gtk_box_append(GTK_BOX(buttons_box), back_button); // Add to buttons_box if needed

    return box;
}
