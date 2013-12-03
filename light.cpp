/******************************************************************/
/*         Lighting functions                                     */
/*                                                                */
/* Group Members: Michael Madden                                  */
/******************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "raytrace.h"

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb) {
  material* m;
  
  /* allocate memory */
  m = (material*) malloc(sizeof(material));
  /* put stuff in it */
  m->r = r;
  m->g = g;
  m->b = b;
  m->amb = amb;
  return(m);
}

/* LIGHTING CALCULATIONS */

/* shade */
/* color of point p with normal vector n and material m returned in c */
/* in is the direction of the incoming ray and d is the recusive depth */
void shade(point* p, vector* n, material* m, vector* in, color* c, int d) {

  /* so far, just finds ambient component of color */
  c->r = m->amb * m->r;
  c->g = m->amb * m->g;
  c->b = m->amb * m->b;
  
  /* clamp color values to 1.0 */
  if (c->r > 1.0) c->r = 1.0;
  if (c->g > 1.0) c->g = 1.0;
  if (c->b > 1.0) c->b = 1.0;

}


color* makeColor(GLfloat r, GLfloat g, GLfloat b){
  color* c;
  /* allocate memory */
  c = (color*) malloc(sizeof(color));

  /* put stuff in it */
  c->r = r;   /* center */
  c->g = g;   /* radius */
  c->b = b;   /* material */
  return(c);
}

light* makeLight(GLfloat a_r, GLfloat a_g, GLfloat a_b, GLfloat d_r, GLfloat d_g, GLfloat d_b, GLfloat s_r, GLfloat s_g, GLfloat s_b, GLfloat intensity, GLfloat x, GLfloat y, GLfloat z){
  light* l;
  /* allocate memory */
  l = (light*) malloc(sizeof(light));

  /* put stuff in it */
  l->ambient_color = makeColor(a_r, a_g, a_b);   /* ambient */
  l->diffuse_color = makeColor(d_r, d_g, d_b);   /* diffuse */
  l->specular_color = makeColor(s_r, s_g, s_b);   /* specular */
  l->i = intensity;   /* radius */
  l->p = makePoint(x, y, z);   /* material */
  return(l);
}