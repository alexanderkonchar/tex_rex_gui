/* tex_rex-window.h
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

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define TEX_REX_TYPE_WINDOW (tex_rex_window_get_type())

G_DECLARE_FINAL_TYPE (TexRexWindow, tex_rex_window, TEX_REX, WINDOW, AdwApplicationWindow)

/* Function Definitions */
static void
tex_rex_window__screenshot_dialog(GAction *action G_GNUC_UNUSED,
                                  GVariant *parameter G_GNUC_UNUSED,
                                  TexRexWindow *self);

static void
tex_rex_window__open_file_picker(GAction *action G_GNUC_UNUSED,
                                 GVariant *parameter G_GNUC_UNUSED,
                                 TexRexWindow *self);

static void
tex_rex_window__copy_text(GAction *action G_GNUC_UNUSED,
                                 GVariant *parameter G_GNUC_UNUSED,
                                 TexRexWindow *self);

static void
on_open_response(GtkNativeDialog *native,
                 int response,
                 TexRexWindow *self);

static void
open_file(TexRexWindow *self,
          GFile *file);

static void
open_file_complete(GObject *source_object,
                   GAsyncResult *result,
                   TexRexWindow *self);
G_END_DECLS
