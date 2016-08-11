/*************************************************************************/
/*  primitive.h                                                          */
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
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "scene/resources/surface_tool.h"

class ParameterEditor;

class Primitive : public SurfaceTool {
  OBJ_TYPE(Primitive, SurfaceTool);

 private:
  Ref<Mesh> mesh;

 protected:
  static const bool smooth = false;
  static const bool flip_normals = false;

  void begin();
  void add_plane(Vector3 dir1, Vector3 dir2, Vector3 offset);
  void add_tri(Vector3Array verts, Vector2Array uv = Vector2Array());
  void add_quad(Vector3Array verts, Vector2Array uv = Vector2Array());
  void commit();

  virtual bool _set(const StringName& name, const Variant& value) =0;

 public:
  void set_mesh(Ref<Mesh> r_mesh);
  Ref<Mesh> get_mesh();
  virtual String get_name() const { return ""; }
  virtual void update() =0;
  virtual void mesh_parameters(ParameterEditor *editor) =0;

  Primitive();
  ~Primitive();

};

#endif
