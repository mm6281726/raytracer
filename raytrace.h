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
  GLfloat ka;
  GLfloat kd;
  GLfloat ks;
  GLfloat s;
  GLfloat kr;
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

typedef struct cylinder {
  point* c;  /* center */
  GLfloat r;  /* radius */
  GLfloat h;  //height
  material* m;
} cylinder;

typedef struct light {
  GLfloat i;
  vector* p;
} light;

typedef struct plane {
  point* c;
  vector* n;
  material* m;
} plane;

extern light* light1;
extern light* light2;
extern light* lightlist[2];
extern sphere* s1;
extern sphere* s2;
extern sphere* spherelist[2];
extern cylinder* c1;
extern plane* pl1;

/* functions in raytrace.cpp */
void traceRay(ray*, color*, int);

/* functions in geometry.cpp */
GLfloat dotProduct(vector* n, vector* l);
color* makeColor(GLfloat r, GLfloat g, GLfloat b);
light* makeLight(GLfloat intensity, GLfloat x, GLfloat y, GLfloat z);
sphere* makeSphere(GLfloat, GLfloat, GLfloat, GLfloat);
cylinder* makeCylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat h);
plane* makePlane(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat nx, GLfloat ny, GLfloat nz);
point* makePoint(GLfloat, GLfloat, GLfloat);
point* copyPoint(point *);
void freePoint(point *);
void calculateDirection(point*,point*,point*);
void findPointOnRay(ray*,double,point*);
int raySphereIntersect(ray*,sphere*,double*);
void findSphereNormal(sphere*,point*,vector*);
int rayCylinderIntersect(ray*,cylinder*,double*);
void findCylinderNormal(cylinder*,point*,vector*);
int rayPlaneIntersect(ray* r, plane* s, double* t);

/* functions in light.cpp */
material* makeMaterial(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void shade(point*,vector*,material*,vector*,color*,int);

/* global variables */
extern int width;
extern int height;

#endif	/* _RAYTRACE_H_ */
