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


GLfloat dotProduct(vector* n, vector* l){
  return (n->x*l->x + n->y*l->y + n->z*l->z);
}

// Id = Il * kd * max(n . L, 0)
// Id = resulting intensity, Il = light source intensity, kd =  surface reflectance coefficient with 0<k<1, n = normal, L = light vector
void diffuseReflection(light* il, material* m, vector* n, vector* l, color* id){
  GLfloat dp = dotProduct(n, l);
  id->r = il->i * m->kd * m->r * fmax(dp, 0.0);
  id->g = il->i * m->kd * m->g * fmax(dp, 0.0);
  id->b = il->i * m->kd * m->b * fmax(dp, 0.0);
}

// Is = Il * ks * max(r . v, 0)^s
// Is = resulting intensity, Il = light source intensity, ks =  surface reflectance coefficient with 0<k<1, r = reflection direction, v = viewing direction, s = shininess factor
void specularReflection(light* il, material* m, vector* r, vector* v, color* is){
  GLfloat dp = dotProduct(r, v);
  is->r = pow(il->i * m->ks * fmax(dp, 0.0), m->s);
  is->g = pow(il->i * m->ks * fmax(dp, 0.0), m->s);
  is->b = pow(il->i * m->ks * fmax(dp, 0.0), m->s);
}

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat s) {
  material* m;
  
  /* allocate memory */
  m = (material*) malloc(sizeof(material));
  /* put stuff in it */
  m->r = r;
  m->g = g;
  m->b = b;
  m->ka = ka;
  m->kd = kd;
  m->ks = ks;
  m->s = s;
  return(m);
}

/* LIGHTING CALCULATIONS */

/* shade */
/* color of point p with normal vector n and material m returned in c */
/* in is the direction of the incoming ray and d is the recusive depth */
void shade(point* p, vector* n, material* m, vector* in, color* c, int d) {

  /* so far, just finds ambient component of color */
  c->r = m->ka * m->r;
  c->g = m->ka * m->g;
  c->b = m->ka * m->b;

  color id;  //diffusion color
  vector l;  //light vector
  l.x = light1->p->x - p->x; 
  l.y = light1->p->y - p->y; 
  l.z = light1->p->z - p->z;
  diffuseReflection(light1, m, n, &l, &id);
  c->r += id.r;
  c->g += id.g;
  c->b += id.b;

  // color is;
  // vector r;
  // vector v;
  // specularReflection(light1, m, &r, &v, &is);
  // c->r += is.r;
  // c->g += is.g;
  // c->b += is.b;
  
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

light* makeLight(GLfloat intensity, GLfloat x, GLfloat y, GLfloat z){
  light* l;
  /* allocate memory */
  l = (light*) malloc(sizeof(light));

  l->i = intensity;
  l->p = makePoint(x, y, z);
  return(l);
}