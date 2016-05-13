/*************************************************************************/
/*  primitive_editor_plugin.cpp                                          */
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
#include "primitive_editor_plugin.h"
#include "primitive.h"
#include "primitive_box.h"

void PrimitiveEditor::_menu_option(int option) {
  /******************************************************************************************************************************
   * A new MeshInstance is created. Then a Primitive subclass is created (PrimitiveBox, PrimitiveSphere, etc.)
   * What that subclass is depends on which menu button was pressed. The mesh that Primitive 'created' is then
   * assigned to the MeshInstance.
   ******************************************************************************************************************************/
  Primitive *p_primitive = NULL;

  edited_scene = get_tree()->get_edited_scene_root();
  selected = SpatialEditor::get_singleton()->get_selected();

  // if ( !selected && edited_scene )

  // If the scene is empty OR no node is selected, return.
  // TODO: Once _undo_redo() can accommodate for an empty scene,
  //       replace this with the above implementation.
  if( !selected || !edited_scene)
    return;

  mesh_instance = memnew(MeshInstance);

  switch(option) {
  case MENU_OPTION_BOX: {
    _undo_redo("Box");
    mesh_instance->set_name("Box");

    p_primitive = memnew(PrimitiveBox); //TODO: This doesn't need typecasting?
  } break;
  }

  if (p_primitive) {
    p_primitive->update();
    //TODO: In this case, mesh_instance doesn't get deallocated later. Is that OK?
    mesh_instance->set_mesh(p_primitive->get_mesh());

    memdelete(p_primitive);
  } else {
    memdelete(mesh_instance);
  }
}

void PrimitiveEditor::_bind_methods() {
  ObjectTypeDB::bind_method("_menu_option",&PrimitiveEditor::_menu_option);
}

void PrimitiveEditor::_undo_redo(String name) {
  String action = "Create ";
  action.insert(7, name);
  undo_redo->create_action(action);

  if(edited_scene){
    undo_redo->add_do_method(selected, "add_child", mesh_instance);
    undo_redo->add_do_method(mesh_instance, "set_owner", edited_scene);
    undo_redo->add_do_reference(mesh_instance);

    undo_redo->add_undo_method(selected, "remove_child", mesh_instance);
  } else {
    //TODO: If the edited scene is empty, then the new MeshInstance should
    //      become the root node.
    /*
    Node *parent = SpatialEditor::get_singleton()->get_parent();
    undo_redo->add_do_method(parent, "set_edited_scene", mesh_instance);
    undo_redo->add_do_reference(mesh_instance);
    undo_redo->add_undo_method(editor, "set_edited_scene", Variant());
    */
  }

  undo_redo->commit_action();
}

PrimitiveEditor::PrimitiveEditor(EditorNode *p_editor) {
  /******************************************************************************************************************************
   * The "Add Primitive" menu button is added to the Spatial Editor menu (For all things 3D).
   * Menu items are added to the button, and _menu_option() is run at the press of one of those
   * menu items (see _bind_methods() ).
   *
   * TODO: Place this button in a more appropriate place on the spatial editor.
   ******************************************************************************************************************************/
  editor=p_editor;
  undo_redo = SpatialEditor::get_singleton()->get_undo_redo();

  spatial_editor_hb = memnew( HBoxContainer );
  SpatialEditor::get_singleton()->add_control_to_menu_panel(spatial_editor_hb);

  // Puts a separator between the button and everything else.
  spatial_editor_hb->add_child(memnew(VSeparator));

  add_primitive_button = memnew( MenuButton );
  //The ONLY thing necessary here is to drop icon_add_primitive.png into tools/editor/icons/.
  add_primitive_button->set_icon(p_editor->get_gui_base()->get_icon("AddPrimitive", "EditorIcons"));
  add_primitive_button->set_tooltip("Add a new primitive.");
  spatial_editor_hb->add_child( add_primitive_button );

  add_primitive_button->get_popup()->add_item("Box", MENU_OPTION_BOX);

  add_primitive_button->get_popup()->connect("item_pressed", this,"_menu_option");
}

PrimitiveEditor::~PrimitiveEditor() {

}

PrimitiveEditorPlugin::PrimitiveEditorPlugin(EditorNode *p_node) {
  /******************************************************************************************************************************
   * All execution related to the Primitive module starts HERE.
   * This routine is called because PrimitiveEditorPlugin was added to EditorPlugins (see register_types.cpp).
   ******************************************************************************************************************************/
  editor=p_node;
  primitive_editor=memnew( PrimitiveEditor(p_node) );

  // I don't know why this is necessary but I get a segfault if I remove it.
  SpatialEditor::get_singleton()->get_palette_split()->add_child(primitive_editor);
  SpatialEditor::get_singleton()->get_palette_split()->move_child(primitive_editor,0);

}
