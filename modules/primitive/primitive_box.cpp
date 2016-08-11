/*************************************************************************/
/*  primitive_box.cpp                                                    */
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
#include "primitive_box.h"

String PrimitiveBox::get_name() const {
  return "Box";
}

void PrimitiveBox::update() {
  Vector3 fd = Vector3(width, 0, 0);
  Vector3 rd = Vector3(0, 0, length);
  Vector3 ud = Vector3(0, height, 0);

  Vector3 ofs = Vector3(width/2, height/2, length/2);

  begin();

  add_smooth_group(smooth);

  if (right_face)
    add_plane(-rd, -ud, ofs);

  if (left_face)
    add_plane(ud, rd, -ofs);

  if (top_face)
    add_plane(-fd, -rd, ofs);

  if (bottom_face)
    add_plane(rd, fd, -ofs);

  if (front_face)
  add_plane(-ud, -fd, ofs);

  if (back_face)
    add_plane(fd, ud, -ofs);

  commit();
}

void PrimitiveBox::mesh_parameters(ParameterEditor *editor) {
  editor->add_numeric_parameter("width", width);
  editor->add_numeric_parameter("length", length);
  editor->add_numeric_parameter("height", height);
}

bool PrimitiveBox::_set(const StringName& name, const Variant& value) {
  if( name == "width" )
    width = value;
  else if( name == "length" )
    length = value;
  else if( name == "height" )
    height = value;
  else
    return false;

  return true;
}


PrimitiveBox::PrimitiveBox() {
  width = 2.0;
  length = 2.0;
  height = 2.0;
  right_face = true;
  left_face = true;
  top_face = true;
  bottom_face = true;
  front_face = true;
  back_face = true;
}
