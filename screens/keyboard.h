#ifndef KEYBOARD_SCREEN_H
#define KEYBOARD_SCREEN_H

#include <gtk/gtk.h>
#include "../installer.h"

G_BEGIN_DECLS

#define KEYBOARD_TYPE_SCREEN (keyboard_screen_get_type())
G_DECLARE_FINAL_TYPE(KeyboardScreen, keyboard_screen, KEYBOARD, SCREEN, GObject)

KeyboardScreen *keyboard_screen_new(void);

G_END_DECLS

#endif // KEYBOARD_SCREEN_H
