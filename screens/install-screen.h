/* install-screen.h - Installation progress screen */
#pragma once

#include <gtk/gtk.h>
#include <adwaita.h>
#include "../wave-installer.h" // Include the main application header

G_BEGIN_DECLS

void install_screen_create(WaveInstallerApplication *app);

G_END_DECLS
