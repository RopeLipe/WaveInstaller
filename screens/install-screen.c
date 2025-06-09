/* install-screen.c - Installation progress screen implementation */
#include "install-screen.h"
#include "../wave-installer.h"
#include <string.h>

typedef struct {
    const char *task;
    gboolean completed;
} InstallTask;

static InstallTask install_tasks[] = {
    {"Preparing installation...", FALSE},
    {"Partitioning disk...", FALSE},
    {"Installing base system...", FALSE},
    {"Installing bootloader...", FALSE},
    {"Configuring system...", FALSE},
    {"Setting up user account...", FALSE},
    {"Finalizing installation...", FALSE},
    {NULL, FALSE}
};

static GtkWidget *progress_bar = NULL;
static GtkWidget *status_label = NULL;
static GtkWidget *task_list = NULL;
static GtkWidget *complete_button = NULL;
static guint timer_id = 0;
static int current_task = 0;
static double progress = 0.0;

static gboolean
update_progress(gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    
    /* Update progress */
    progress += 0.01;
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), progress);
    
    /* Update current task */
    if (progress >= (double)(current_task + 1) / 7.0 && current_task < 7) {
        install_tasks[current_task].completed = TRUE;
        current_task++;
        
        if (current_task < 7) {
            gtk_label_set_text(GTK_LABEL(status_label), install_tasks[current_task].task);
        }
        
        /* Update task list visually */
        GtkWidget *child = gtk_widget_get_first_child(task_list);
        int task_index = 0;
        while (child && task_index <= current_task) {
            if (task_index < current_task) {
                gtk_widget_add_css_class(child, "task-completed");
                gtk_widget_remove_css_class(child, "task-current");
            } else if (task_index == current_task) {
                gtk_widget_add_css_class(child, "task-current");
                gtk_widget_remove_css_class(child, "task-completed");
            }
            child = gtk_widget_get_next_sibling(child);
            task_index++;
        }
    }
    
    /* Check if installation is complete */
    if (progress >= 1.0) {
        gtk_label_set_text(GTK_LABEL(status_label), "Installation completed successfully!");
        gtk_widget_set_visible(complete_button, TRUE);
        return G_SOURCE_REMOVE; /* Stop timer */
    }
    
    return G_SOURCE_CONTINUE; /* Continue timer */
}

static void
start_installation(WaveInstallerApplication *app) {
    /* Reset progress */
    current_task = 0;
    progress = 0.0;
    
    /* Set initial status */
    gtk_label_set_text(GTK_LABEL(status_label), install_tasks[0].task);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);
    
    /* Start timer */
    timer_id = g_timeout_add(100, update_progress, app); /* Update every 100ms */
}

static void
on_complete_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    /* Installation complete - normally would reboot or exit */
    wave_installer_application_quit_installer(app);
}

static void
on_back_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    /* Stop installation if going back */
    if (timer_id > 0) {
        g_source_remove(timer_id);
        timer_id = 0;
    }
    wave_installer_application_previous_screen(app);
}

void
install_screen_create(WaveInstallerApplication *app) {
    GtkWidget *container, *content, *title, *subtitle, *progress_box, *progress_label;
    GtkWidget *nav_box, *back_btn;
    
    /* Main container */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(container, "screen-container");
    
    /* Content area */
    content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_add_css_class(content, "screen-content");
    gtk_box_append(GTK_BOX(container), content);
    
    /* Title */
    title = gtk_label_new("Installing Wave Linux");
    gtk_widget_add_css_class(title, "screen-title");
    gtk_box_append(GTK_BOX(content), title);
    
    /* Subtitle */
    subtitle = gtk_label_new("Please wait while the system is being installed...");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_box_append(GTK_BOX(content), subtitle);
    
    /* Progress section */
    progress_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_vexpand(progress_box, TRUE);
    gtk_widget_set_valign(progress_box, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(content), progress_box);
    
    /* Progress bar */
    progress_bar = gtk_progress_bar_new();
    gtk_widget_add_css_class(progress_bar, "install-progress");
    gtk_widget_set_hexpand(progress_bar, TRUE);
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress_bar), TRUE);
    gtk_box_append(GTK_BOX(progress_box), progress_bar);
    
    /* Status label */
    status_label = gtk_label_new("Preparing installation...");
    gtk_widget_add_css_class(status_label, "status-text");
    gtk_box_append(GTK_BOX(progress_box), status_label);
    
    /* Task list */
    progress_label = gtk_label_new("Installation Steps:");
    gtk_widget_add_css_class(progress_label, "section-title");
    gtk_widget_set_halign(progress_label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(progress_label, 30);
    gtk_box_append(GTK_BOX(progress_box), progress_label);
    
    task_list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(task_list, "task-list");
    gtk_box_append(GTK_BOX(progress_box), task_list);
    
    /* Add task items */
    for (int i = 0; install_tasks[i].task != NULL; i++) {
        GtkWidget *task_item = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        gtk_widget_add_css_class(task_item, "task-item");
        
        /* Task icon */
        GtkWidget *task_icon = gtk_image_new_from_icon_name("emblem-default");
        gtk_image_set_pixel_size(GTK_IMAGE(task_icon), 16);
        gtk_widget_add_css_class(task_icon, "task-icon");
        gtk_box_append(GTK_BOX(task_item), task_icon);
        
        /* Task text */
        GtkWidget *task_text = gtk_label_new(install_tasks[i].task);
        gtk_widget_add_css_class(task_text, "task-text");
        gtk_widget_set_halign(task_text, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(task_item), task_text);
        
        gtk_box_append(GTK_BOX(task_list), task_item);
    }
    
    /* Complete button (initially hidden) */
    complete_button = gtk_button_new_with_label("Complete Installation");
    gtk_widget_add_css_class(complete_button, "primary-button");
    gtk_widget_set_halign(complete_button, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(complete_button, 30);
    gtk_widget_set_visible(complete_button, FALSE);
    g_signal_connect(complete_button, "clicked", G_CALLBACK(on_complete_clicked), app);
    gtk_box_append(GTK_BOX(progress_box), complete_button);
    
    /* Navigation buttons */
    nav_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_add_css_class(nav_box, "nav-buttons");
    gtk_widget_set_halign(nav_box, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(container), nav_box);
    
    /* Back button */
    back_btn = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_btn, "secondary-button");
    g_signal_connect(back_btn, "clicked", G_CALLBACK(on_back_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), back_btn);
    
    /* Store widget */
    app->screens[SCREEN_INSTALL].widget = container;
    
    /* Start installation automatically when screen is shown */
    start_installation(app);
}
