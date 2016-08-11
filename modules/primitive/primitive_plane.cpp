/*************************************************************************/
/*  primitive_plane.cpp                                                  */
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
#include "primitive_plane.h"
#include "primitive_utils.h"

String PrimitivePlane::get_name() const {
  return "Plane";
}

void PrimitivePlane::update() {
  //TODO: Find an easier way to define a Vector with pre-defined elements.
  //      Like {e1, e2, e3}. Apply it to every Vector in this file.
  Vector3Array verts = Vector3Array();
  verts.push_back(Vector3(-width/2, end_height, -length/2));
  verts.push_back(Vector3(width/2, end_height, -length/2));
  verts.push_back(Vector3(width/2, start_height, length/2));
  verts.push_back(Vector3(-width/2, start_height, length/2));

  float uv_width = verts[0].distance_to(verts[1]);
  float uv_length = verts[0].distance_to(verts[3]);

  begin();

  add_smooth_group(smooth);

  add_quad(verts, plane_uv(uv_width, uv_length));

  commit();
}

void PrimitivePlane::mesh_parameters(ParameterEditor *editor) {
  editor->add_numeric_parameter("width", width);
  editor->add_numeric_parameter("length", length);
}

bool PrimitivePlane::_set(const StringName& name, const Variant& value) {
  if( name == "width" )
    width = value;
  else if( name == "length" )
    length = value;
  else
    return false;

  return true;
}


PrimitivePlane::PrimitivePlane() {
  width = 2.0;
  length = 2.0;
  start_height = 0.0;
  end_height = 0.0;
}
