/*************************************************************************/
/*  register_types.cpp                                                   */
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
#include "register_types.h"
#ifndef _3D_DISABLED
#include "object_type_db.h"
#include "primitive_dialog_editors.h"
#include "primitive_dialog.h"
#include "primitive.h"
#include "primitive_box.h"
#include "primitive_circle.h"
#include "primitive_cone.h"
#include "primitive_plane.h"
#include "primitive_editor_plugin.h"
#endif

void register_primitive_types() {
#ifndef _3D_DISABLED
  ObjectTypeDB::register_virtual_type<Primitive>();
  ObjectTypeDB::register_type<PrimitiveBox>();
  ObjectTypeDB::register_type<PrimitiveCircle>();
  ObjectTypeDB::register_type<PrimitiveCone>();
  ObjectTypeDB::register_type<PrimitivePlane>();
#ifdef TOOLS_ENABLED
  ObjectTypeDB::register_type<PrimitiveDialog>();
  ObjectTypeDB::register_virtual_type<TreeEditor>();
  ObjectTypeDB::register_type<ModifierEditor>();
  ObjectTypeDB::register_type<ParameterEditor>();
  EditorPlugins::add_by_type<PrimitiveEditorPlugin>();
#endif
#endif

}

void unregister_primitive_types() {
}
