/*************************************************************************/
/*  primitive_dialog.cpp                                                 */
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
#include "primitive_dialog.h"
#include "scene/gui/color_picker.h"

static const Vector2 DIALOG_SIZE = Vector2(260, 280);
static const Color DEFAULT_COLOR = Color(0, 1, 0);

void PrimitiveDialog::show_dialog() {
  Size2 rect_size = get_viewport_rect().size;

  color_hb->show();

  set_pos((rect_size - DIALOG_SIZE)/2);
  set_size(DIALOG_SIZE);
  show();
}

void PrimitiveDialog::_dialog_hide() {
  emit_signal("dialog_closed");
}

void PrimitiveDialog::edit(MeshInstance *node, Primitive* builder) {
  mesh_instance = node;

  if (!mesh_instance)
    return;

  set_title("New " + builder->get_name());

  parameter_editor->edit(builder);

  set_current_editor(0);
}

void PrimitiveDialog::set_current_editor(int index) {
  int child_count = main_panel->get_child_count();

  if (index >= child_count)
    return;

  int i;
  for(i=0;i<child_count;i++) {
    //TODO: It may be faster to get the entire array and iterate through,
    //      rather then get each individual child.
    if (index == i)
      main_panel->get_child(i)->cast_to<CanvasItem>()->show();
    else
      main_panel->get_child(i)->cast_to<CanvasItem>()->hide();
  }
  current_editor = index;
}

TreeEditor *PrimitiveDialog::get_editor(String name) {
  if (main_panel->has_node(name))
    return main_panel->get_node(name)->cast_to<TreeEditor>();
  return NULL;
}

void PrimitiveDialog::connect_editor(const StringName& name, Object *obj, String method) {
  TreeEditor *editor = get_editor(name);
  String signal_;

  if (!editor)
    return;

  signal_ = editor->get_signal();

  editor->connect(signal_, obj, method);

}

void PrimitiveDialog::_bind_methods() {
  ObjectTypeDB::bind_method("_dialog_hide",&PrimitiveDialog::_dialog_hide);

  ADD_SIGNAL( MethodInfo("dialog_closed"));
}

PrimitiveDialog::PrimitiveDialog() {
  VBoxContainer *vbc = memnew(VBoxContainer);
  HBoxContainer *hbc = memnew(HBoxContainer);
  Label *l = memnew(Label);
  ColorPickerButton *color_picker = memnew(ColorPickerButton);
  options = memnew(OptionButton);
  color_hb = memnew(HBoxContainer);
  float sy;
  main_panel = memnew(PanelContainer);
  text_display = memnew(Label);
  parameter_editor = memnew(ParameterEditor);
  current_editor = 0;
  mesh_instance = NULL;

  vbc->set_area_as_parent_rect(get_constant("margin", "Dialogs"));
  add_child(vbc);

  hbc->set_h_size_flags(HBoxContainer::SIZE_EXPAND_FILL);
  vbc->add_child(hbc);

  options->set_custom_minimum_size(Vector2(120,0));
  hbc->add_child(options);
  options->connect("item_selected", this, "set_current_editor");

  color_hb->set_h_size_flags(HBoxContainer::SIZE_EXPAND_FILL);
  color_hb->set_alignment(HBoxContainer::ALIGN_END);
  hbc->add_child(color_hb);

  l->set_text("Display ");
  color_hb->add_child(l);

  color_picker->set_color(DEFAULT_COLOR);
  color_picker->set_edit_alpha(false);
  color_hb->add_child(color_picker);

  sy = color_picker->edit_get_minimum_size().y;
  color_picker->set_custom_minimum_size(Vector2(sy * 1.5, sy));

  color_picker->connect("color_changed", this, "_color_changed");

  main_panel->set_v_size_flags(PanelContainer::SIZE_EXPAND_FILL);
  vbc->add_child(main_panel);

  main_panel->add_child(parameter_editor);

  const String parameter_editor_name = parameter_editor->get_name();
  options->add_item(parameter_editor->get_name());

  text_display->set_align(Label::ALIGN_CENTER);
  vbc->add_child(text_display);

  connect("hide", this, "_dialog_hide");
}

PrimitiveDialog::~PrimitiveDialog() {

}
