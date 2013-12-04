#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

/******************************************************************/
/*         Raytracer declarations                                 */
/******************************************************************/


/* constants */
#define TRUE 1
#define FALSE 0

#define PI 3.14159265358979323846264338327

/* data structures */

typedef struct point {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat w;
} point;

/* a vector is just a point */
typedef point vector;

/* a ray is a start point and a direction */
typedef struct ray {
  point* start;
  vector* dir;
} ray;

typedef struct material {
  /* color */
  GLfloat r;
  GLfloat g;
  GLfloat b; 
  /* ambient reflectivity */
  GLfloat amb;
} material;

typedef struct color {
  GLfloat r;
  GLfloat g;
  GLfloat b; 
  /* these should be between 0 and 1 */
} color;

typedef struct sphere {
  point* c;  /* center */
  GLfloat r;  /* radius */
  material* m;
} sphere;

typedef struct light {
  color* ambient_color;
  color* diffuse_color;
  color* specular_color;
  GLfloat i;
  vector* p;
} light;

extern light* light1;

/* functions in raytrace.cpp */
void traceRay(ray*, color*, int);

/* functions in geometry.cpp */
color* makeColor(GLfloat r, GLfloat g, GLfloat b);
light* makeLight(GLfloat a_r, GLfloat a_g, GLfloat a_b, GLfloat d_r, GLfloat d_g, GLfloat d_b, GLfloat s_r, GLfloat s_g, GLfloat s_b, GLfloat intensity, GLfloat x, GLfloat y, GLfloat z);

sphere* makeSphere(GLfloat, GLfloat, GLfloat, GLfloat);
point* makePoint(GLfloat, GLfloat, GLfloat);
point* copyPoint(point *);
void freePoint(point *);
void calculateDirection(point*,point*,point*);
void findPointOnRay(ray*,double,point*);
int raySphereIntersect(ray*,sphere*,double*);
void findSphereNormal(sphere*,point*,vector*);

/* functions in light.cpp */
material* makeMaterial(GLfloat, GLfloat, GLfloat, GLfloat);
void shade(point*,vector*,material*,vector*,color*,int);

/* global variables */
extern int width;
extern int height;

#endif	/* _RAYTRACE_H_ */
