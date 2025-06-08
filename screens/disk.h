#ifndef DISK_SCREEN_H
#define DISK_SCREEN_H

#include <gtk/gtk.h>
#include "../installer.h"

G_BEGIN_DECLS

#define DISK_TYPE_SCREEN (disk_screen_get_type())
G_DECLARE_FINAL_TYPE(DiskScreen, disk_screen, DISK, SCREEN, GObject)

DiskScreen *disk_screen_new(void);

G_END_DECLS

#endif // DISK_SCREEN_H
