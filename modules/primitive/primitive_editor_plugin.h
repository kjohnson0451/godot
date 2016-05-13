/*************************************************************************/
/*  primitive_editor_plugin.h                                            */
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
#ifndef PRIMITIVE_EDITOR_PLUGIN_H
#define PRIMITIVE_EDITOR_PLUGIN_H

#include "tools/editor/plugins/mesh_editor_plugin.h"
#include "tools/editor/plugins/spatial_editor_plugin.h"

class PrimitiveEditor : public HBoxContainer {
  OBJ_TYPE(PrimitiveEditor, HBoxContainer);

 private:
  enum Menu {
    MENU_OPTION_BOX
  };

  UndoRedo *undo_redo;
  EditorNode *editor;
  Spatial *selected;
  Node *edited_scene;
  HBoxContainer *spatial_editor_hb;
  MenuButton *add_primitive_button;
  MeshInstance *mesh_instance;


  void _menu_option(int);
  void _undo_redo(String name);

 protected:
  static void _bind_methods();

 public:
  PrimitiveEditor(EditorNode *p_editor);
  ~PrimitiveEditor();
};

class PrimitiveEditorPlugin : public EditorPlugin {
  OBJ_TYPE(PrimitiveEditorPlugin, EditorPlugin);

 private:
  PrimitiveEditor *primitive_editor;
  EditorNode *editor;

 public:
  //TODO: It may or may not be necessary to add the make_visible method. Find out.
  PrimitiveEditorPlugin(EditorNode *p_node);
};
#endif
