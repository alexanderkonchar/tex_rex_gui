/* tex_rex-application.cpp
 *
 * Copyright 2022 Alex Konchar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "tex_rex-application.h"
#include "tex_rex-window.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "UnusedValue"
#pragma clang diagnostic ignored "-Wunused-parameter"
struct [[maybe_unused]] _TexRexApplication {
    [[maybe_unused]] AdwApplication parent_instance;
};

[[maybe_unused]] [[maybe_unused]] G_DEFINE_TYPE (TexRexApplication, tex_rex_application, ADW_TYPE_APPLICATION)

TexRexApplication *
tex_rex_application_new(const char *application_id,
                        GApplicationFlags flags) {
    g_return_val_if_fail (application_id != nullptr, NULL);

    return static_cast<TexRexApplication *>(g_object_new(TEX_REX_TYPE_APPLICATION,
                                                         "application-id", application_id,
                                                         "flags", flags,
                                                         NULL));
}

static void
tex_rex_application_activate(GApplication *app) {
    GtkWindow *window;

    g_assert (TEX_REX_IS_APPLICATION(app));

    window = gtk_application_get_active_window(GTK_APPLICATION (app));
    if (window == nullptr)
        window = static_cast<GtkWindow *>(g_object_new(TEX_REX_TYPE_WINDOW,
                                                       "application", app,
                                                       NULL));

    gtk_window_present(window);
}

[[maybe_unused]] static void
tex_rex_application_class_init(TexRexApplicationClass *klass) {
    GApplicationClass *app_class = G_APPLICATION_CLASS (klass);

    app_class->activate = tex_rex_application_activate;
}

static void
tex_rex_application_about_action([[maybe_unused]] GSimpleAction *action,
                                 [[maybe_unused]] GVariant *parameter,
                                 gpointer user_data) {
    static const char *developers[] = {"Alex Konchar", nullptr};
    auto *self = static_cast<TexRexApplication *>(user_data);
    GtkWindow *window = nullptr;

    g_assert (TEX_REX_IS_APPLICATION(self));

    window = gtk_application_get_active_window(GTK_APPLICATION (self));

    adw_show_about_window(window,
                          "application-name", "tex_rex",
                          "application-icon", "tk.texrex.app",
                          "developer-name", "Alex Konchar",
                          "version", "0.1.0",
                          "developers", developers,
                          "copyright", "© 2022 Alex Konchar",
                          NULL);
}

static void
tex_rex_application_quit_action([[maybe_unused]] GSimpleAction *action,
                                [[maybe_unused]] GVariant *parameter,
                                gpointer user_data) {
    auto *self = static_cast<TexRexApplication *>(user_data);

    g_assert (TEX_REX_IS_APPLICATION(self));

    g_application_quit(G_APPLICATION (self));
}

static const GActionEntry app_actions[] = {
        {"quit",  tex_rex_application_quit_action},
        {"about", tex_rex_application_about_action},
};

[[maybe_unused]] static void
tex_rex_application_init(TexRexApplication *self) {
    g_action_map_add_action_entries(G_ACTION_MAP (self),
                                    app_actions,
                                    G_N_ELEMENTS (app_actions),
                                    self);
    const char *quit[] = {"<primary>q", nullptr};
    gtk_application_set_accels_for_action(GTK_APPLICATION (self),
                                          "app.quit",
                                          quit);

    const char *open_image[] = {"<Ctrl>o", nullptr};
    gtk_application_set_accels_for_action (GTK_APPLICATION (self),
                                           "win.open_image",
                                           open_image);

    const char *screenshot[] = {"<Ctrl>s", nullptr};
    gtk_application_set_accels_for_action (GTK_APPLICATION (self),
                                           "win.screenshot",
                                           screenshot);
}

#pragma clang diagnostic pop