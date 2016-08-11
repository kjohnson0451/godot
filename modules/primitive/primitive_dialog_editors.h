/*************************************************************************/
/*  primitive_dialog_editors.h                                           */
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
#ifndef PRIMITIVE_DIALOG_EDITORS_H
#define PRIMITIVE_DIALOG_EDITORS_H

#include "scene/gui/box_container.h"
#include "scene/gui/tree.h"
#include "scene/gui/check_box.h"
#include "primitive.h"

class TreeEditor : public VBoxContainer {
  OBJ_TYPE(TreeEditor, VBoxContainer);

 protected:
  Tree *tree;
  Primitive *builder;
  TreeItem *current;
  TreeItem* _create_item(const String& text, const StringName& type);

 public:
  virtual String get_signal() =0;
  void add_numeric_parameter(const String& text, float value, float min_ = 0.001, float max_ = 100, float step = 0.001);
  void add_numeric_parameter(const String& text, int value, float min_ = 0.001, float max_ = 100, float step = 0.001);
  TreeEditor();
};

class ParameterEditor : public TreeEditor {
  OBJ_TYPE(ParameterEditor, TreeEditor);

 private:
  HBoxContainer *hb;
  CheckBox *smooth_button;
  CheckBox *flip_button;
  void reset_scrollbar();
  void _item_edited();
  String get_parameter_name(TreeItem *item);
  Variant get_parameter_value(TreeItem *item);

 protected:
  static void _bind_methods();
  String get_signal();

 public:
  void edit(Primitive *p_primitive);
  ParameterEditor();
  ~ParameterEditor();
};


class ModifierEditor : public TreeEditor {
  OBJ_TYPE(ModifierEditor, TreeEditor);

 private:
  PopupMenu *menu;

 protected:
  String get_signal();

 public:
  ModifierEditor();
};

#endif
