/* disk-screen.h - Disk partitioning screen */
#pragma once

#include <gtk/gtk.h>
#include <adwaita.h>
#include "../wave-installer.h" // Include the main application header

G_BEGIN_DECLS

void disk_screen_create(WaveInstallerApplication *app);

G_END_DECLS
