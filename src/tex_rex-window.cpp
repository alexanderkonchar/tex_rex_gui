/* tex_rex-window.cpp
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


#include <string>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "tex_rex-window.h"
#include "text_extractor.h"

struct _TexRexWindow {
    [[maybe_unused]] AdwApplicationWindow parent_instance;

    GdkClipboard* clipboard = gdk_display_get_clipboard(gdk_display_get_default());

    /* Template widgets */
    [[maybe_unused]] AdwWindowTitle *title;
    [[maybe_unused]] AdwStatusPage *welcome;
    [[maybe_unused]] GtkButton *screenshot_button;
    [[maybe_unused]] GtkButton *open_image_button;
    [[maybe_unused]] AdwViewStack *main_stack;
    [[maybe_unused]] GtkRevealer *toolbox;
    [[maybe_unused]] GtkScrolledWindow *text_scrollview;
    [[maybe_unused]] GtkTextView *shot_text;
    [[maybe_unused]] GtkButton *text_copy_btn;
};

[[maybe_unused]] G_DEFINE_FINAL_TYPE (TexRexWindow, tex_rex_window, ADW_TYPE_APPLICATION_WINDOW)

[[maybe_unused]] static void
tex_rex_window_class_init(TexRexWindowClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/tk/texrex/app/tex_rex-window.ui");

    // Bind the widgets for use in actions and functions
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, title);
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, welcome);
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, screenshot_button);
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, open_image_button);
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, main_stack);
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, toolbox);
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, text_scrollview);
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, shot_text);
    gtk_widget_class_bind_template_child (widget_class, TexRexWindow, text_copy_btn);
}

[[maybe_unused]] static void
tex_rex_window_init(TexRexWindow *self) {
    gtk_widget_init_template(GTK_WIDGET (self));

    // Create the necessary actions
    g_autoptr (GSimpleAction) screenshot_action =
            g_simple_action_new("screenshot", NULL);
    g_signal_connect (screenshot_action,
                      "activate",
                      G_CALLBACK(tex_rex_window__screenshot_dialog),
                      self);
    g_action_map_add_action(G_ACTION_MAP (self),
                            G_ACTION (screenshot_action));

    g_autoptr (GSimpleAction) open_image_action =
            g_simple_action_new("open_image", NULL);
    g_signal_connect (open_image_action,
                      "activate",
                      G_CALLBACK(tex_rex_window__open_file_picker),
                      self);
    g_action_map_add_action(G_ACTION_MAP (self),
                            G_ACTION (open_image_action));

    g_autoptr (GSimpleAction) copy_action =
            g_simple_action_new("copy", NULL);
    g_signal_connect (copy_action,
                      "activate",
                      G_CALLBACK(tex_rex_window__copy_text),
                      self);
    g_action_map_add_action(G_ACTION_MAP (self),
                            G_ACTION (copy_action));
}

/* Class Functions */
static void
tex_rex_window__screenshot_dialog(GAction *action G_GNUC_UNUSED,
                                  GVariant *parameter G_GNUC_UNUSED,
                                  TexRexWindow *self) {
//    ++self->counter;
//    std::string i_str = std::to_string(self->counter);
//    const char *i_char = i_str.c_str();
//    adw_status_page_set_title(self->welcome, i_char);

    system("python3 /home/alex/Code/screenshot/main.py");

    std::string image_path = "/home/alex/Projects/tex_rex/temp.png";

    // Set the margins for the text view
    gtk_text_view_set_left_margin(self->shot_text, 8);
    gtk_text_view_set_right_margin(self->shot_text, 8);
    gtk_text_view_set_top_margin(self->shot_text, 8);
    gtk_text_view_set_bottom_margin(self->shot_text, 8);

    // Reveal the bottom toolbox
    gtk_revealer_set_reveal_child(self->toolbox, true);

    // Switch the stack to display the extracted page
    adw_view_stack_set_visible_child_name(self->main_stack, "extracted");

    // Retrieve the GtkTextBuffer instance that stores the
    // text displayed by the GtkTextView widget
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->shot_text);

    // Set the text using the contents from the textExtractor class
    char *outText;
    text_extractor textExtractor;
    textExtractor.extract_text_from_image(image_path, outText);
    std::string text = outText;
    gsize length = text.length();
    gtk_text_buffer_set_text(buffer, outText, length);
    delete outText;
    system("rm /home/alex/Projects/tex_rex/temp.png");

    // Reposition the cursor, so it's at the start of the text
    GtkTextIter start;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_place_cursor(buffer, &start);

    // Set the title using the display name
    adw_window_title_set_title(self->title, "Screenshot");
}

static void
tex_rex_window__open_file_picker(GAction *action G_GNUC_UNUSED,
                                 GVariant *parameter G_GNUC_UNUSED,
                                 TexRexWindow *self) {
    // Create a new file selection dialog, using the "open" mode
    GtkFileChooserNative *native =
            gtk_file_chooser_native_new("Open File",
                                        GTK_WINDOW (self),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        "_Open",
                                        "_Cancel");

    // Connect the "response" signal of the file selection dialog;
    // this signal is emitted when the user selects a file, or when
    // they cancel the operation
    g_signal_connect (native,
                      "response",
                      G_CALLBACK(on_open_response),
                      self);

    // Present the dialog to the user
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(native));
}

static void
tex_rex_window__copy_text(GAction *action G_GNUC_UNUSED,
                          GVariant *parameter G_GNUC_UNUSED,
                          TexRexWindow *self) {
    gdk_clipboard_set_text(self->clipboard, reinterpret_cast<const char *>(self->shot_text));
}

/* Helper Functions */
static void
on_open_response(GtkNativeDialog *native,
                 int response,
                 TexRexWindow *self) {
    // If the user selected a file...
    if (response == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);

        // ... retrieve the location from the dialog...
        g_autoptr (GFile) file = gtk_file_chooser_get_file(chooser);

        // ... and open it
        open_file(self, file);
    }

    // Release the reference on the file selection dialog now that we
    // do not need it anymore
    g_object_unref(native);
}

static void
open_file(TexRexWindow *self,
          GFile *file) {
    g_file_load_contents_async(file,
                               NULL,
                               (GAsyncReadyCallback) open_file_complete,
                               self);
}

static void
open_file_complete(GObject *source_object,
                   GAsyncResult *result,
                   TexRexWindow *self) {
    GFile *file = G_FILE (source_object);

    g_autofree char *contents = NULL;
    gsize length = 0;

    g_autoptr (GError) error = NULL;

    // Complete the asynchronous operation; this function will either
    // give you the contents of the file as a byte array, or will
    // set the error argument
    g_file_load_contents_finish(file,
                                result,
                                &contents,
                                &length,
                                NULL,
                                &error);

    // Query the display name for the file
    g_autofree char *display_name = NULL;
    g_autoptr (GFileInfo) info =
            g_file_query_info(file,
                              "standard::display-name",
                              G_FILE_QUERY_INFO_NONE,
                              NULL,
                              NULL);
    if (info != NULL) {
        display_name =
                g_strdup(g_file_info_get_attribute_string(info, "standard::display-name"));
    } else {
        display_name = g_file_get_basename(file);
    }

    // In case of error, print a warning to the standard error output
    if (error != NULL) {
        g_printerr("Unable to open “%s”: %s\n",
                   g_file_peek_path(file),
                   error->message);
        return;
    }

    // Store the image path for use by text_extractor
    std::string image_path = g_file_peek_path(file);

    // Set the margins for the text view
    gtk_text_view_set_left_margin(self->shot_text, 8);
    gtk_text_view_set_right_margin(self->shot_text, 8);
    gtk_text_view_set_top_margin(self->shot_text, 8);
    gtk_text_view_set_bottom_margin(self->shot_text, 8);

    // Reveal the bottom toolbox
    gtk_revealer_set_reveal_child(self->toolbox, true);

    // Switch the stack to display the extracted page
    adw_view_stack_set_visible_child_name(self->main_stack, "extracted");

    // Retrieve the GtkTextBuffer instance that stores the
    // text displayed by the GtkTextView widget
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->shot_text);

    // Set the text using the contents from the textExtractor class
    char *outText;
    text_extractor textExtractor;
    textExtractor.extract_text_from_image(image_path, outText);
    std::string text = outText;
    delete outText;
    length = text.length();
    strcpy(contents, text.c_str());
    gtk_text_buffer_set_text(buffer, contents, length);

    // Reposition the cursor, so it's at the start of the text
    GtkTextIter start;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_place_cursor(buffer, &start);

    // Set the title using the display name
    adw_window_title_set_title(self->title, display_name);
}