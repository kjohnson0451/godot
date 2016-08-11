/*************************************************************************/
/*  primitive_dialog_editors.cpp                                         */
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
#include "primitive_dialog_editors.h"

TreeItem* TreeEditor::_create_item(const String& text, const StringName& type) {
  TreeItem* item = tree->create_item(current);

  item->set_text(0,text);
  item->set_icon(0, get_icon(type, "EditorIcons"));
  item->set_selectable(0, false);

  item->set_editable(1, true);

  return item;
}

void TreeEditor::add_numeric_parameter(const String& text, float value, float min_, float max_, float step) {
  TreeItem* item;

  item = _create_item(text, "Real");

  item->set_cell_mode(1, TreeItem::CELL_MODE_RANGE);
  item->set_range_config(1, min_, max_, step);
  item->set_range(1, value);
}

void TreeEditor::add_numeric_parameter(const String& text, int value, float min_, float max_, float step) {
  TreeItem* item;

  item = _create_item(text, "Integer");

  item->set_cell_mode(1, TreeItem::CELL_MODE_RANGE);
  item->set_range_config(1, min_, max_, step);
  item->set_range(1, value);
}

TreeEditor::TreeEditor() {
  Node *slider;

  current = NULL;
  tree = memnew(Tree);

  tree->set_hide_root(true);
  tree->set_columns(2);
  tree->set_column_expand(0, true);
  tree->set_column_min_width(0, 30);
  tree->set_column_expand(1, true);
  tree->set_column_min_width(1, 15);

  tree->set_v_size_flags(SIZE_EXPAND_FILL);

  //tree->connect("item_edited", this, "item_edited");
  //tree->connect("cell_selected", this, "item_selected");

  slider = tree->find_node("HSlider", true, false);

}

void ParameterEditor::edit(Primitive *p_primitive) {
  builder = p_primitive;

  tree->clear();

  //TODO: Reset Scroll Bar

  if (!builder)
    return;

  current = tree->create_item();
  builder->mesh_parameters(this);
}

void ParameterEditor::_item_edited() {
  if (!builder)
    return;

  TreeItem *item = tree->get_edited();

  String name = get_parameter_name(item);
  Variant value = get_parameter_value(item);

  builder->set(name, value);
  builder->get_mesh()->set_meta("primitive_" + name, value); //TODO: Check if entry already exists.

  emit_signal("parameter_edited");
}

String ParameterEditor::get_parameter_name(TreeItem *item) {
  return item->get_text(0).replace(" ", "_").to_lower();
}

Variant ParameterEditor::get_parameter_value(TreeItem *item) {
  TreeItem::TreeCellMode cell = item->get_cell_mode(1);

  switch(cell) {
  case TreeItem::CELL_MODE_CHECK:
    return item->is_checked(1);
    break;
  case TreeItem::CELL_MODE_STRING:
    return item->get_text(1);
    break;
  case TreeItem::CELL_MODE_RANGE:
    return item->get_range(1);
    break;
  case TreeItem::CELL_MODE_CUSTOM:
    return item->get_metadata(1);
    break;
  }
}

String ParameterEditor::get_signal() {
  return "parameter_edited";
}

void ParameterEditor::_bind_methods() {
  ObjectTypeDB::bind_method("_item_edited",&ParameterEditor::_item_edited);

  ADD_SIGNAL( MethodInfo("parameter_edited"));
}

ParameterEditor::ParameterEditor() {
  hb = memnew(HBoxContainer);
  //smooth_button = memnew(CheckBox);
  flip_button = memnew(CheckBox);

  set_name("parameters");

  add_child(tree);

  hb->set_h_size_flags(SIZE_EXPAND_FILL);
  add_child(hb);

  flip_button->set_text("Flip Normals");
  flip_button->set_h_size_flags(SIZE_EXPAND_FILL);
  hb->add_child(flip_button);

  tree->connect("item_edited", this, "_item_edited");
}

ParameterEditor::~ParameterEditor() {
  //hb->remove_child(flip_button);
  //memdelete(smooth_button);
  //memdelete(flip_button);
}

String ModifierEditor::get_signal() {
  return "modifier_edited";
}

ModifierEditor::ModifierEditor() {
  set_name("modifiers");

}
