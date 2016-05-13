/*************************************************************************/
/*  primitive.cpp                                                        */
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
#include "primitive.h"
#include "primitive_utils.h"

Ref<Mesh> Primitive::get_mesh() {
  return mesh;
}

void Primitive::begin() {
  SurfaceTool::begin(Mesh::PRIMITIVE_TRIANGLES);
}

void Primitive::add_plane(Vector3 dir1, Vector3 dir2, Vector3 offset = Vector3()) {
  Vector3Array verts;
  Vector2Array uv;
  real_t width;
  real_t height;

  verts = build_plane(dir1, dir2, offset);

  width = verts.get(0).distance_to(verts.get(1));
  height = verts.get(0).distance_to(verts.get(3));

  uv = plane_uv(width, height);

  add_quad(verts, uv);
}

void Primitive::add_quad(Vector3Array verts, Vector2Array uv) {
  if (flip_normals) {
    verts.invert();
    uv.invert();
  }

  if (uv.size()) {
    add_uv(uv[0]);
    add_vertex(verts[0]);
    add_uv(uv[1]);
    add_vertex(verts[1]);
    add_uv(uv[2]);
    add_vertex(verts[2]);
    add_vertex(verts[2]);
    add_uv(uv[3]);
    add_vertex(verts[3]);
    add_uv(uv[0]);
    add_vertex(verts[0]);
  } else {
    add_vertex(verts[0]);
    add_vertex(verts[1]);
    add_vertex(verts[2]);
    add_vertex(verts[2]);
    add_vertex(verts[3]);
    add_vertex(verts[0]);
  }
}

void Primitive::commit() {
  generate_normals();
  index();

  if( mesh->get_surface_count())
    mesh->surface_remove(0);

  SurfaceTool::commit(mesh);

  clear();
}

Primitive::Primitive() {
  mesh = Ref<Mesh>(memnew(Mesh)); //TODO: This doesn't need to be cleared later?
  mesh->set_name(get_name().replace(" ","_").to_lower());
}

Primitive::~Primitive() {

}
