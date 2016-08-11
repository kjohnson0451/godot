/*************************************************************************/
/*  primitive_cone.cpp                                                   */
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
#include "primitive_cone.h"
#include "primitive_utils.h"
#include <algorithm>

String PrimitiveCone::get_name() const {
  return "Cone";
}

void PrimitiveCone::update() {
  float slice_angle = TWO_PI - (float)Math::deg2rad((double)slice_to);
  float circumference = (slice_angle * Math_PI * std::max(bottom_radius, top_radius)) / Math_PI;

  Vector3 top = Vector3(0, height/2, 0);
  Vector3 bottom = Vector3(0, -height/2, 0);
  Vector2 center_uv = Vector2(0.5, 0.5);

  Vector3Array bc = build_circle(bottom, sides, bottom_radius, Math::deg2rad(slice_from), slice_angle);
  Vector3Array tc;

  Vector2Array b_uv = ellipse_uv(center_uv, sides, Vector2(bottom_radius, bottom_radius), slice_angle);
  Vector2Array t_uv;

  Vector3Array verts;
  Vector2Array uv;

  begin();

  add_smooth_group(smooth);

  if(top_radius > 0) {
    tc = build_circle(top, sides, top_radius, Math::deg2rad(slice_from), slice_angle);
    t_uv = ellipse_uv(center_uv, sides, Vector2(top_radius, top_radius), slice_angle);

    float u1, u2;
    for( int i = 0; i < sides; i++ ) {
      u1 = (float)i/float(sides) * circumference;
      u2 = ((float)i+1)/float(sides) * circumference;

      //TODO: Find an easier way to define a Vector with pre-defined elements.
      //      Like {e1, e2, e3}. Apply it to every Vector in this file.
      verts = Vector3Array();
      verts.push_back(tc[i]); verts.push_back(bc[i]);
      verts.push_back(bc[i+1]); verts.push_back(tc[i+1]);

      uv = Vector2Array();
      uv.push_back(Vector2(u1, height)); uv.push_back(Vector2(u1, 0));
      uv.push_back(Vector2(u2, 0)); uv.push_back(Vector2(u2, height));

      add_quad(verts, uv);

    }
  } else {
    for( int i = 0; i < sides; i++ ) {
      verts = Vector3Array();
      verts.push_back(top); verts.push_back(bc[i]); verts.push_back(bc[i+1]);

      uv = Vector2Array();
      uv.push_back(center_uv); uv.push_back(b_uv[i]);uv.push_back(b_uv[i+1]);

      add_tri(verts, uv);
    }
  }

  add_smooth_group(false);

  if( generate_ends && (slice_to > 0)) {
    if (top_radius > 0) {
      verts = Vector3Array();
      verts.push_back(tc[0]); verts.push_back(top);
      verts.push_back(bottom); verts.push_back(bc[0]);

      uv = Vector2Array();
      uv.push_back(Vector2(top_radius, 0)); uv.push_back(Vector2());
      uv.push_back(Vector2(0, height)); uv.push_back(Vector2(bottom_radius, height));

      add_quad(verts, uv);

      if (!flip_normals)
        uv.invert();

      verts = Vector3Array();
      verts.push_back(bc[sides]); verts.push_back(bottom);
      verts.push_back(top); verts.push_back(tc[sides]);

      add_quad(verts, uv);
    } else {
      verts = Vector3Array();
      verts.push_back(bc[0]); verts.push_back(top); verts.push_back(bottom);

      uv = Vector2Array();
      uv.push_back(Vector2(bottom_radius, height)); uv.push_back(Vector2()); uv.push_back(Vector2(0, height));

      add_tri(verts, uv);

      if (!flip_normals)
        uv.invert();

      verts = Vector3Array();
      verts.push_back(bottom); verts.push_back(top); verts.push_back(bc[sides]);

      add_tri(verts, uv);
    }
  }

  bool gen_top = generate_top && (top_radius > 0);

  if (generate_bottom || gen_top) {
    for ( int i = 0; i < sides; i++ ) {
      if (gen_top) {
        verts = Vector3Array();
        verts.push_back(top); verts.push_back(tc[i]); verts.push_back(tc[i+1]);

        uv = Vector2Array();
        uv.push_back(center_uv); uv.push_back(t_uv[i]); uv.push_back(t_uv[i+1]);

        add_tri(verts, uv);
      }
      if (generate_bottom) {
        verts = Vector3Array();
        verts.push_back(bottom); verts.push_back(bc[i+1]); verts.push_back(bc[i]);

        uv = Vector2Array();
        uv.push_back(center_uv); uv.push_back(b_uv[i+1]); uv.push_back(b_uv[i]);

        add_tri(verts, uv);
      }
    }
  }
  commit();
}

void PrimitiveCone::mesh_parameters(ParameterEditor *editor) {
  editor->add_numeric_parameter("top_radius", top_radius, 0);
  editor->add_numeric_parameter("bottom_radius", bottom_radius);
  editor->add_numeric_parameter("height", height);
  editor->add_numeric_parameter("sides", sides, 3, 64, 1);
  editor->add_numeric_parameter("slice_from", slice_from, 0, 360, 1);
  editor->add_numeric_parameter("slice_to", 0, 359, 1);
}

bool PrimitiveCone::_set(const StringName& name, const Variant& value) {
  if( name == "top_radius" )
    top_radius = value;
  else if( name == "bottom_radius" )
    bottom_radius = value;
  else if( name == "height" )
    height = value;
  else if( name == "sides" )
    sides = value;
  else if( name == "slice_from" )
    slice_from = value;
  else if( name == "slice_to" )
    slice_to = value;
  else if( name == "generate_top" )
    generate_top = value;
  else if( name == "generate_bottom" )
    generate_bottom = value;
  else if( name == "generate_ends" )
    generate_ends = value;
  else
    return false;

  return true;
}


PrimitiveCone::PrimitiveCone() {
  top_radius = 0.0;
  bottom_radius = 1.0;
  height = 2.0;
  sides = 16;
  slice_from = 0;
  slice_to = 0;
  generate_top = true;
  generate_bottom = true;
  generate_ends = true;
}
