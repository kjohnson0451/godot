/*************************************************************************/
/*  primitive_circle.cpp                                                 */
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
#include "primitive_circle.h"
#include "primitive_utils.h"

String PrimitiveCircle::get_name() const {
  return "Circle";
}

void PrimitiveCircle::update() {
  float slice_angle = TWO_PI - Math::deg2rad(slice_to);

  Vector3 center = Vector3();
  Vector2 center_uv = Vector2(0.5, 0.5);

  Vector3Array circle = build_circle(center, segments, radius, Math::deg2rad(slice_from), slice_angle);
  Vector2Array e_uv = ellipse_uv(center_uv, segments, Vector2(radius, radius), slice_angle);

  Vector3Array verts;
  Vector2Array uv;

  begin();

  add_smooth_group(smooth);

  //TODO: Find an easier way to define a Vector with pre-defined elements.
  //      Like {e1, e2, e3}.
  for (int i = 0; i < segments; i++) {
    verts = Vector3Array();
    verts.push_back(center); verts.push_back(circle[i]); verts.push_back(circle[i+1]);

    uv = Vector2Array();
    uv.push_back(center_uv); uv.push_back(e_uv[i]); uv.push_back(e_uv[i+1]);

    add_tri(verts, uv);
    }

  commit();
}

void PrimitiveCircle::mesh_parameters(ParameterEditor *editor) {
  editor->add_numeric_parameter("radius", radius);
  editor->add_numeric_parameter("segments", segments, 3, 64, 1);
}

bool PrimitiveCircle::_set(const StringName& name, const Variant& value) {
  if( name == "radius" )
    radius = value;
  else if( name == "segments" )
    segments = value;
  else
    return false;

  return true;
}


PrimitiveCircle::PrimitiveCircle() {
  radius = 1.0;
  segments = 16;
  slice_from = 0.0;
  slice_to = 0.0;
}
