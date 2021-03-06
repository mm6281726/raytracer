/******************************************************************/
/*         Geometry functions                                     */
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

point* makePoint(GLfloat x, GLfloat y, GLfloat z) {
  point* p;
  /* allocate memory */
  p = (point*) malloc(sizeof(point));
  /* put stuff in it */
  p->x = x; p->y = y; p->z = z; 
  p->w = 1.0;
  return (p);
}

/* makes copy of point (or vector) */
point* copyPoint(point *p0) {
  point* p;
  /* allocate memory */
  p = (point*) malloc(sizeof(point));

  p->x = p0->x;
  p->y = p0->y;
  p->z = p0->z;
  p->w = p0->w;         /* copies over vector or point status */
  return (p);
}

/* unallocates a point */
void freePoint(point *p) {
  if (p != NULL) {
    free(p);
  }
}

/* vector from point p to point q is returned in v */
void calculateDirection(point* p, point* q, point* v) {
  v->x = q->x - p->x;
  v->y = q->y - p->y;
  v->z = q->z - p->z;
  /* a direction is a point at infinity */
  v->w = 0.0;

  /* NOTE: v is not unit length currently, but probably should be */
}

/* given a vector, sets its contents to unit length */
void normalize(vector* v) {
	GLfloat length = sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));

  v->x = v->x/length;
  v->y = v->y/length;
  v->z = v->z/length;
}

/* point on ray r parameterized by t is returned in p */
void findPointOnRay(ray* r,double t,point* p) {
  p->x = r->start->x + t * r->dir->x;
  p->y = r->start->y + t * r->dir->y;
  p->z = r->start->z + t * r->dir->z;
  p->w = 1.0;
}

/* SPHERES */

sphere* makeSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
  sphere* s;
  /* allocate memory */
  s = (sphere*) malloc(sizeof(sphere));

  /* put stuff in it */
  s->c = makePoint(x,y,z);   /* center */
  s->r = r;   /* radius */
  s->m = NULL;   /* material */
  return(s);
}

/* Cylinder */

cylinder* makeCylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat h) {
  cylinder* c;
  /* allocate memory */
  c = (cylinder*) malloc(sizeof(cylinder));

  /* put stuff in it */
  c->c = makePoint(x,y,z);   /* center */
  c->r = r;   /* radius */
  c->h = h; //height
  c->m = NULL;   /* material */
  return(c);
}

/* plane */

plane* makePlane(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat nx, GLfloat ny, GLfloat nz) {
  plane* p;
  /* allocate memory */
  p = (plane*) malloc(sizeof(plane));
  vector n;
  n.x = nx;
  n.y = ny;
  n.z = nz;

  /* put stuff in it */
  p->c = makePoint(cx,cy,cz);   /* center */
  p->n = &n;
  p->m = NULL;   /* material */
  return(p);
}

/* returns TRUE if ray r hits sphere s, with parameter value in t */
int raySphereIntersect(ray* r,sphere* s,double* t) {
  point p;   /* start of transformed ray */
  double a,b,c;  /* coefficients of quadratic equation */
  double D;    /* discriminant */
  point* v;
  
  /* transform ray so that sphere center is at origin */
  /* don't use matrix, just translate! */
  p.x = r->start->x - s->c->x;
  p.y = r->start->y - s->c->y;
  p.z = r->start->z - s->c->z;
  v = r->dir; /* point to direction vector */


  a = v->x * v->x  +  v->y * v->y  +  v->z * v->z;
  b = 2*( v->x * p.x  +  v->y * p.y  +  v->z * p.z);
  c = p.x * p.x + p.y * p.y + p.z * p.z - s->r * s->r;

  D = b * b - 4 * a * c;
  
  if (D < 0) {  /* no intersection */
    return (FALSE);
  }
  else {
    D = sqrt(D);
    /* First check the root with the lower value of t: */
    /* this one, since D is positive */
    *t = (-b - D) / (2*a);
    /* ignore roots which are less than zero (behind viewpoint) */
    if (*t < 0) {
      *t = (-b + D) / (2*a);
    }
    if (*t < 0) { return(FALSE); }
    else return(TRUE);
  }
}

/* normal vector of s at p is returned in n */
/* note: dividing by radius normalizes */
void findSphereNormal(sphere* s, point* p, vector* n) {
  n->x = (p->x - s->c->x) / s->r;  
  n->y = (p->y - s->c->y) / s->r;
  n->z = (p->z - s->c->z) / s->r;
  n->w = 0.0;
}

/* returns TRUE if ray r hits cylinder s, with parameter value in t */
int rayCylinderIntersect(ray* r, cylinder* s, double* t) {
  point p;   /* start of transformed ray */
  double a,b,c;  /* coefficients of quadratic equation */
  double D;    /* discriminant */
  point* v;
  
  /* transform ray so that cylinder center is at origin */
  /* don't use matrix, just translate! */
  p.x = r->start->x - s->c->x;
  p.y = r->start->y - s->c->y;
  p.z = r->start->z - s->c->z;
  v = r->dir; /* point to direction vector */


  a = v->x * v->x  +  v->z * v->z;
  b = 2*( v->x * p.x  +  v->z * p.z);
  c = p.x * p.x + p.z * p.z - s->r * s->r;

  D = b * b - 4 * a * c;
  
  if (D < 0) {  /* no intersection */
    return (FALSE);
  }
  else {
    D = sqrt(D);
    /* First check the root with the lower value of t: */
    /* this one, since D is positive */
    *t = (-b - D) / (2*a);
    /* ignore roots which are less than zero (behind viewpoint) */
    if (*t < 0) {
      *t = (-b + D) / (2*a);
    }
    if (*t < 0) { return(FALSE); }
    else return(TRUE);
  }
}

/* normal vector of s at p is returned in n */
/* note: dividing by radius normalizes */
void findCylinderNormal(cylinder* c, point* p, vector* n) {
  n->x = (p->x - c->c->x) / c->r;  
  n->y = p->y;
  n->z = (p->z - c->c->z) / c->r;
  n->w = 0.0;
}

/* returns TRUE if ray r hits plane s, with parameter value in t */
int rayPlaneIntersect(ray* r, plane* s, double* t) {
  GLfloat np = dotProduct(s->n, r->start);
  GLfloat nd = dotProduct(s->n, r->dir);

  *t = -(np) / nd;
  /* ignore roots which are less than zero (behind viewpoint) */
  if (*t == 0) { return(TRUE); }
  else return(FALSE);
}