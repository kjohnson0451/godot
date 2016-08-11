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

Vector3Array build_circle(Vector3 pos, int segments, float radius, float start, float angle) {
  Vector3Array circle = Vector3Array(); circle.resize(segments + 1);

  float s_angle = angle / (float)segments;

  int i; float a;
  for( i = 0; i < segments; i++ ) {
    a = (s_angle * float(i)) + start;
    circle.set(i, Vector3(Math::cos(a), 0, Math::sin(a)) * radius + pos);
  }

  if (angle != TWO_PI) {
    angle += start;
    circle.set(segments, Vector3(Math::cos(angle), 0, Math::sin(angle)) * radius + pos);
  } else {
    circle.set(segments, circle.get(0));
  }

  return circle;
}

Vector2Array ellipse_uv(Vector2 pos, int segments, Vector2 radius, float angle) {
  Vector2Array ellipse = Vector2Array(); ellipse.resize(segments + 1);

  float s_angle = angle/segments;

  int i; float a;
  for( i = 0; i < segments; i++ ) {
    a = s_angle * float(i);
    ellipse.set(i, Vector2(Math::sin(a) * radius.x, Math::cos(a) * radius.y) + pos);
  }

  if (angle != TWO_PI) {
    ellipse.set(segments, Vector2(Math::sin(angle) * radius.x, Math::cos(angle) * radius.y) + pos);
  } else {
    ellipse.set(segments, ellipse.get(0));
  }

  return ellipse;
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
