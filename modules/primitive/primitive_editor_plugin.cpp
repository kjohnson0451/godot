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
#include "primitive_circle.h"
#include "primitive_cone.h"
#include "primitive_plane.h"

void PrimitiveEditor::_menu_option(int option) {
  /******************************************************************************************************************************
   * A new MeshInstance is created. Then a Primitive subclass is created (PrimitiveBox, PrimitiveSphere, etc.)
   * What that subclass is depends on which menu button was pressed. The mesh that Primitive 'created' is then
   * assigned to the MeshInstance.
   ******************************************************************************************************************************/
  Ref<Mesh> mesh;
  String primitive_type;

  edited_scene = get_tree()->get_edited_scene_root();

  // if ( !selected && edited_scene )

  // If the scene is empty OR no node is selected, return.
  // TODO: Once _undo_redo() can accommodate for an empty scene,
  //       replace this with the above implementation.
  if( !selected || !edited_scene)
    return;

  if( option != MENU_OPTION_EDIT ) {
    mesh_instance = memnew(MeshInstance);
    mesh = Ref<Mesh>(memnew(Mesh));

    switch(option) {
    case MENU_OPTION_BOX:
      primitive_type = "Box";
      primitive = memnew(PrimitiveBox); //TODO: This doesn't need typecasting?
      break;
    case MENU_OPTION_CIRCLE:
      primitive_type = "Circle";
      primitive = memnew(PrimitiveCircle); //TODO: This doesn't need typecasting?
      break;
    case MENU_OPTION_CONE:
      primitive_type = "Cone";
      primitive = memnew(PrimitiveCone);
      break;
    case MENU_OPTION_PLANE:
      primitive_type = "Plane";
      primitive = memnew(PrimitivePlane); //TODO: This doesn't need typecasting?
      break;
    }

    _undo_redo("Create " + primitive_type);
    mesh_instance->set_name(primitive_type);
    mesh->set_meta("primitive_type", primitive_type);

    mesh_instance->set_mesh(mesh);
  } else {
    //TODO: Make an undo redo for this.

    //TODO: Make sure this mesh is valid.
    mesh = mesh_instance->get_mesh();

    primitive_type = mesh->get_meta("primitive_type");

    if( primitive_type == "Box" )
      primitive = memnew(PrimitiveBox);
    if( primitive_type == "Circle" )
      primitive = memnew(PrimitiveCircle);
    else if( primitive_type == "Cone" )
      primitive = memnew(PrimitiveCone);
    else if( primitive_type == "Plane" )
      primitive = memnew(PrimitivePlane);

    List<String> meta_list;
    mesh->get_meta_list(&meta_list);

    for(List<String>::Element *E=meta_list.front();E;E=E->next()) {
      String meta_name = E->get();
      if( meta_name == "primitive_type" )
        continue;
      Variant meta_value = mesh->get_meta(meta_name);
      primitive->set(meta_name.replace("primitive_", ""), meta_value);
    }
  }

  primitive->set_mesh(mesh);
  primitive->update();

  dialog->edit(mesh_instance, primitive);
  dialog->show_dialog();

  add_primitive_button->set_disabled(true);
}

void PrimitiveEditor::_bind_methods() {
  ObjectTypeDB::bind_method("_menu_option",&PrimitiveEditor::_menu_option);
  ObjectTypeDB::bind_method("_update_mesh",&PrimitiveEditor::_update_mesh);
  ObjectTypeDB::bind_method("_dialog_closed",&PrimitiveEditor::_dialog_closed);
  ObjectTypeDB::bind_method("_selection_changed",&PrimitiveEditor::_selection_changed);
}

void PrimitiveEditor::_update_mesh() {
  primitive->update();
}

void PrimitiveEditor::_display_info(uint32_t start) {
  if (!mesh_instance)
    return;

  uint32_t exec_time = OS::get_singleton()->get_ticks_msec();
  Ref<Mesh> mesh = mesh_instance->get_mesh();
  int i;
  int surface_count = mesh->get_surface_count();
  int surf_v, surf_idx;
  int verts = 0;
  int tris = 0;

  for (i = 0; i<surface_count; i++) {
    surf_v = mesh->surface_get_array_len(i);
    verts += surf_v;

    surf_idx = mesh->surface_get_array_index_len(i);
    if(surf_idx == Mesh::NO_INDEX_ARRAY)
      surf_idx = surf_v;

    tris += surf_idx/3;
  }

}

void PrimitiveEditor::_dialog_closed() {
  if(primitive) {
    memdelete(primitive);
    primitive = NULL;
  }
  add_primitive_button->set_disabled(false);
}

void PrimitiveEditor::_undo_redo(String name) {
  undo_redo->create_action(name);

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

void PrimitiveEditor::_selection_changed() {
  //TODO: This does not get the initial selected node on startup.
  //      Connecting enter_tree may make this possible.
  if (!(selected = SpatialEditor::get_singleton()->get_selected()))
    return;

  if( selected->get_type() == "MeshInstance" ) {
    mesh_instance = selected->cast_to<MeshInstance>();
  } else {
    add_primitive_button->get_popup()->set_item_disabled(edit_index, true);
    return;
  }

  Ref<Mesh> mesh = mesh_instance->get_mesh();

  if (mesh == NULL) {
    add_primitive_button->get_popup()->set_item_disabled(edit_index, true);
    return;
  }

  if( mesh->has_meta("primitive_type")) {
    add_primitive_button->get_popup()->set_item_disabled(edit_index, false);
  }
  else {
    add_primitive_button->get_popup()->set_item_disabled(edit_index, true);
  }
}

PrimitiveEditor::PrimitiveEditor(EditorNode *p_editor, EditorPlugin *p_plugin) {
  /******************************************************************************************************************************
   * The "Add Primitive" menu button is added to the Spatial Editor menu (For all things 3D).
   * Menu items are added to the button, and _menu_option() is run at the press of one of those
   * menu items (see _bind_methods() ).
   *
   * TODO: Place this button in a more appropriate place on the spatial editor.
   ******************************************************************************************************************************/
  editor=p_editor;
  primitive = NULL;
  mesh_instance = NULL;
  dialog = memnew(PrimitiveDialog);
  p_plugin->get_base_control()->add_child(dialog);
  //TODO: I need to access the PrimitiveEditorPlugin, but I'm not sure if this is the best implementation
  undo_redo = SpatialEditor::get_singleton()->get_undo_redo();
  spatial_editor_hb = memnew( HBoxContainer );
  SpatialEditor::get_singleton()->add_control_to_menu_panel(spatial_editor_hb);

  // Puts a separator between the button and everything else.
  spatial_editor_hb->add_child(memnew(VSeparator));

  add_primitive_button = memnew( MenuButton );
  // The ONLY thing necessary here is to drop icon_add_primitive.png into tools/editor/icons/.
  add_primitive_button->set_icon(p_editor->get_gui_base()->get_icon("AddPrimitive", "EditorIcons"));
  add_primitive_button->set_tooltip("Add a new primitive.");

  add_primitive_button->get_popup()->add_item("Box", MENU_OPTION_BOX);
  add_primitive_button->get_popup()->add_item("Circle", MENU_OPTION_CIRCLE);
  add_primitive_button->get_popup()->add_item("Cone", MENU_OPTION_CONE);
  add_primitive_button->get_popup()->add_item("Plane", MENU_OPTION_PLANE);
  add_primitive_button->get_popup()->add_separator();
  //TODO: The edit primitive button must be disabled or enabled on startup
  //      depending on the object that gets selected on startup.
  add_primitive_button->get_popup()->add_item("Edit Primitive", MENU_OPTION_EDIT);

  edit_index = add_primitive_button->get_popup()->get_item_index(MENU_OPTION_EDIT);

  add_primitive_button->get_popup()->set_item_disabled(edit_index, true);

  add_primitive_button->get_popup()->connect("item_pressed", this,"_menu_option");

  spatial_editor_hb->add_child( add_primitive_button );

  dialog->connect_editor("parameters", this, "_update_mesh");
  dialog->connect("dialog_closed", this, "_dialog_closed");

  //TODO: This run whenever the user clicks anywhere on the scene. Since this only needs
  //      to run when the user selects another item, there might be a more efficient signal.
  p_editor->get_editor_selection()->connect("selection_changed", this, "_selection_changed");
}

PrimitiveEditor::~PrimitiveEditor() {
  if(primitive){
    memdelete(primitive);
    primitive = NULL;
  }
}

PrimitiveEditorPlugin::PrimitiveEditorPlugin(EditorNode *p_node) {
  /******************************************************************************************************************************
   * All execution related to the Primitive module starts HERE.
   * This routine is called because PrimitiveEditorPlugin was added to EditorPlugins (see register_types.cpp).
   ******************************************************************************************************************************/
  editor=p_node;
  primitive_editor=memnew( PrimitiveEditor(p_node, this) );
  SpatialEditor::get_singleton()->get_palette_split()->add_child(primitive_editor);
  SpatialEditor::get_singleton()->get_palette_split()->move_child(primitive_editor,0);
}
