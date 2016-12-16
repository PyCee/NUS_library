#include "vector.h"
#include "pythag.h"
#include <stdlib.h>
#include <stdio.h>

vector gen_vector(void)
{
  static vector _vector = DEFAULT_VECTOR;
  return _vector;
}
vector build_vector(float x, float y, float z)
{
  vector _vector;
  _vector.x = x;
  _vector.y = y;
  _vector.z = z;
  return _vector;
}
float mag_sq_vector(vector _vector)
{
  return _vector.x * _vector.x + _vector.y * _vector.y + _vector.z * _vector.z;
}
vector normalize_vector(vector _vector)
{
  double change = pythag(_vector.x, _vector.y, _vector.z);
  return (change == 0.0) ? build_vector(0.0, 0.0, -1.0) :
    scale_vector(_vector, 1 / change);
}
vector add_vector(vector _vector1, vector _vector2)
{
  vector _vector;
  _vector.x = _vector1.x + _vector2.x;
  _vector.y = _vector1.y + _vector2.y;
  _vector.z = _vector1.z + _vector2.z;
  return _vector;
}
vector sub_vector(vector _vector1, vector _vector2)
{
  return add_vector(_vector1, scale_vector(_vector2, -1.0));
}
unsigned char cmp_vector(vector _vector1, vector _vector2)
{
  return _vector1.x == _vector2.x &&
    _vector1.y == _vector2.y &&
    _vector1.z == _vector2.z;
}
vector scale_vector(vector _vector, double scale)
{
  return build_vector(_vector.x * scale, _vector.y * scale, _vector.z * scale);
}
float dot_vector(vector _vector1, vector _vector2)
{
  return _vector1.x * _vector2.x + _vector1.y * _vector2.y + _vector1.z * _vector2.z;
}
vector cross_vector(vector _vector1, vector _vector2)
{
  return build_vector(_vector1.y * _vector2.z - _vector1.z * _vector2.y,
		      _vector1.z * _vector2.x - _vector1.x * _vector2.z,
		      _vector1.x * _vector2.y - _vector1.y * _vector2.x);
}
vector interpolate_vector(vector _vector1, vector _vector2, float t)
{
  t = (t < 1.0) ? t : 1.0;
  t = (t > 0.0) ? t : 0.0;
  float b = 1.0 - t;
  return add_vector(scale_vector(_vector1, b), scale_vector(_vector2, t));
}
void print_vector(vector _vector)
{
  printf("[%f, %f, %f]\n", _vector.x, _vector.y, _vector.z);
}
float point_line_sq_dist(vector point, vector p1, vector p2)
{
  vector v = sub_vector(p2, p1),
    w = sub_vector(point, p1);
  
  float b = dot_vector(w, v) / dot_vector(v, v);
  b = (b > 1) ? 1 : b;
  b = (b < 0) ? 0 : b;
  return mag_sq_vector(sub_vector(point, add_vector(p1, scale_vector(v, b))));
}
float line_line_sq_dist(vector p1, vector p2, vector l1, vector l2)
{
  vector u = sub_vector(p2, p1),
    v = sub_vector(l2, l1),
    w = sub_vector(l1, p1),
    base, change, difference;
  float a = dot_vector(u, u),
    b = dot_vector(u, v),
    c = dot_vector(v, v),
    d = dot_vector(u, w),
    e = dot_vector(v, w),
    s, t, den = a * c - (b * b);
  if(den == 0){
    s = 0;
    t = d / b;// or = e / c;
  } else{
    s = (b * e - c * d) / den;
    s *= (s < 0) ? -1: 1;
    t = (a * e - b * d) / den;
  }
  t *= (t < 0) ? -1: 1;
  base = sub_vector(p1, l1);
  change = sub_vector(scale_vector(u, s), scale_vector(v, t));
  difference = add_vector(base, change);
  return mag_sq_vector(difference);
}
float closest_point_line(vector p1, vector p2, vector point)
{
  vector v = sub_vector(p2, p1),
    w = sub_vector(point, p1);
  float c1 = dot_vector(w, v),
    c2 = dot_vector(v, v),
    b = c1 / c2;
  b = (b > 1) ? 1 : b;
  b = (b < 0) ? 0 : b;
  return b;
}
float *closest_point_lines(vector p1, vector p2, vector l1, vector l2)
{
  vector u = sub_vector(p2, p1),
    v = sub_vector(l2, l1),
    w = sub_vector(l1, p1);
  float a = dot_vector(u, u),
    b = dot_vector(u, v),
    c = dot_vector(v, v),
    d = dot_vector(u, w),
    e = dot_vector(v, w),
    *points = malloc(sizeof(float) * 2),
    den = a * c - (b * b);
  if(den == 0){
    points[0] = 0;
    points[1] = d / b;// or = e / c;
  } else{
    points[0] = (b * e - c * d) / den;
    points[0] *= (points[0] < 0) ? -1: 1;
    points[1] = (a * e - b * d) / den;
  }
  points[1] *= (points[1] < 0) ? -1: 1;
  
  return points;
}
