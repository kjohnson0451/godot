/*************************************************************************/
/*  primitive_utils.cpp                                                  */
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
#include "primitive_utils.h"

Vector3Array build_plane(Vector3 dir1, Vector3 dir2, Vector3 offset) {
  Vector3Array plane = Vector3Array(); plane.resize(4);

  plane.set(0, offset);
  plane.set(1, offset + dir1);
  plane.set(2, offset + dir1 + dir2);
  plane.set(3, offset + dir2);

  return plane;
}

Vector2Array plane_uv(real_t width, real_t height, bool last) {
  Vector2Array uv  = Vector2Array(); uv.resize(4);

  uv.set(0, Vector2());
  uv.set(1, Vector2(width, 0));
  uv.set(2, Vector2(width, height));
  uv.set(3, Vector2(0, height));

  if (!last)
    uv.remove(3);

  return uv;
}
