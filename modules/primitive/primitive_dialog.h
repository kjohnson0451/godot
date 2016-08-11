/*************************************************************************/
/*  primitive_dialog.h                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef PRIMITIVE_DIALOG_H
#define PRIMITIVE_DIALOG_H

#include "scene/3d/mesh_instance.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel_container.h"
#include "primitive_dialog_editors.h"

class PrimitiveEditor;

class PrimitiveDialog : public WindowDialog {
  OBJ_TYPE(PrimitiveDialog, WindowDialog);

 private:
  MeshInstance *mesh_instance;
  OptionButton *options;
  HBoxContainer *color_hb;
  PanelContainer *main_panel;
  Label *text_display;
  ParameterEditor *parameter_editor;
  int current_editor;
  void _dialog_hide();
  void set_current_editor(int index);
  TreeEditor *get_editor(String name);

 protected:
  static void _bind_methods();

 public:
  void edit(MeshInstance *node, Primitive* builder);
  void show_dialog();
  void connect_editor(const StringName& name, Object *obj, String method);
  PrimitiveDialog();
  ~PrimitiveDialog();

};

#endif
