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

void reflectionRay(ray* r, color* c, int d){
  traceRay(r, c, d-1);
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

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat s, GLfloat kr) {
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
  m->kr = kr;
  return(m);
}

//int raySphereIntersect(ray* r,sphere* s,double* t)
bool shadowRay(point* p, light* l){
  double t = 0;
  ray r;
  vector* dir = makePoint(l->p->x - p->x, l->p->y - p->y, l->p->z - p->z);
  p->x = p->x + 0.01 * dir->x;
  p->y = p->y + 0.01 * dir->y;
  p->z = p->z + 0.01 * dir->z;
  r.start = p;
  r.dir= dir;
  if(raySphereIntersect(&r, s1, &t) || raySphereIntersect(&r, s2, &t) || rayCylinderIntersect(&r, c1, &t) || rayPlaneIntersect(&r, pl1, &t))
    return true;
  return false;
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

  for(int i = 0; i < 1; i++){
    bool inShadow = shadowRay(p, lightlist[i]);
    if(!inShadow){
      color id;  //diffusion color
      vector l;  //light vector
      l.x = lightlist[i]->p->x - p->x; 
      l.y = lightlist[i]->p->y - p->y; 
      l.z = lightlist[i]->p->z - p->z;
      //light
      diffuseReflection(lightlist[i], m, n, &l, &id);
      c->r += id.r;
      c->g += id.g;
      c->b += id.b;

      color is;   //specular color
      vector r;   //reflection vector
      vector v;   //viewing vector
      //light1
      GLfloat dp = dotProduct(n, &l);
      r.x =  2 * dp * n->x - l.x; 
      r.y =  2 * dp * n->y - l.y; 
      r.z =  2 * dp * n->z - l.z;
      v.x = 0.0 - p->x; 
      v.y = 0.0 - p->y; 
      v.z = 0.0 - p->z;
      specularReflection(lightlist[i], m, &r, &v, &is);
      c->r += is.r;
      c->g += is.g;
      c->b += is.b;

      if(d > 0){
        //reflection
        ray rr;
        rr.start = p;
        rr.dir = &r;
        color reflection;
        reflectionRay(&rr, &reflection, d);
        c->r += reflection.r * m->kr;
        c->g += reflection.g * m->kr;
        c->b += reflection.b * m->kr;
      }
    }
  }
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