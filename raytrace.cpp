/******************************************************************/
/*         Main raytracer file                                    */
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
#include "lowlevel.h"
#include "raytrace.h"

/* local functions */
void initScene(void);
void initCamera (int, int);
void display(void);
void init(int, int);
void myKeyHandler(unsigned char ch, int x, int y);
void traceRay(ray*,color*);
void drawScene(void);
void firstHit(ray*,point*,vector*,material**);

/* local data */
light* lightlist[2];
sphere* spherelist[2];

/* the scene: so far, spheres */
sphere* s1;
sphere* s2;
cylinder* c1;
plane* pl1;

//lights
light* light1;
light* light2;

/* the viewing parameters: */
point* viewpoint;
GLfloat pnear;  /* distance from viewpoint to image plane */
GLfloat fovx;  /* x-angle of view frustum */
int width = 500;     /* width of window in pixels */
int height = 350;    /* height of window in pixels */

int main (int argc, char** argv) {
  int win;

  glutInit(&argc,argv);
  glutInitWindowSize(width,height);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  win = glutCreateWindow("raytrace");
  glutSetWindow(win);
  init(width,height);
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMainLoop();
  return 0;
}

void init(int w, int h) {

  /* OpenGL setup */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
  glClearColor(0.0, 0.0, 0.0, 0.0);  

  /* low-level graphics setup */
  initCanvas(w,h);

  /* raytracer setup */
  initCamera(w,h);
  initScene();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawScene();  /* draws the picture in the canvas */
  flushCanvas();  /* draw the canvas to the OpenGL window */
  glFlush();
}

void initScene () {
  s1 = makeSphere(-0.25,0.0,-2.0,0.25);
  s1->m = makeMaterial(1.0, 0.1, 0.15, 0.3, 0.5, 0.17, 50, 0.5);
  s2 = makeSphere(0.0,0.0,-5.0,0.25);
  s2->m = makeMaterial(0.2, 1.0, 0.15, 0.3, 0.5, 0.03, 20, 0.7);
  spherelist[0] = s1;
  spherelist[1] = s2;
  c1 = makeCylinder(0.25, 0.0 ,- 2.0, 0.15, 0.5);
  c1->m = makeMaterial(0.15, 0.1, 1.0, 0.3, 0.5, 0.15, 20, 0.9);
  pl1 = makePlane(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  pl1->m = makeMaterial(0.15, 0.1, 1.0, 0.3, 0.5, 0.15, 20, 0.4);
  light1 = makeLight(0.2, 10.0, 10.0, 10.0);
  light2 = makeLight(0.1, 0.0, 10.0, 2.0);
  lightlist[0] = light1;
  lightlist[1] = light2;
}

void initCamera (int w, int h) {
  viewpoint = makePoint(0.0,0.0,0.0);
  pnear = 1.0;
  fovx = PI/6;
}

void myKeyHandler(unsigned char ch, int x, int y){
  switch(ch){
    case 'q' :
      exit(0);
      break;

    default : 
      return;
  }
  glutPostRedisplay();
  return;
}

void drawScene () {
  int i,j;
  GLfloat imageWidth;
  /* declare data structures on stack to avoid dynamic allocation */
  point worldPix;  /* current pixel in world coordinates */
  point direction; 
  ray r;
  color c;

  /* initialize */
  worldPix.w = 1.0;
  worldPix.z = -pnear;

  r.start = &worldPix;
  r.dir= &direction;

  imageWidth = 2*pnear*tan(fovx/2);

  /* trace a ray for every pixel */
  for (i=0; i<width; i++) {
    /* Refresh the display */
    /* Comment this line out after debugging */
    flushCanvas();

    for (j=0; j<height; j++) {

      /* find position of pixel in world coordinates */
      /* y position = (pixel height/middle) scaled to world coords */ 
      worldPix.y = (j-(height/2))*imageWidth/width;
      /* x position = (pixel width/middle) scaled to world coords */ 
      worldPix.x = (i-(width/2))*imageWidth/width;

      /* find direction */
      /* note: direction vector is NOT NORMALIZED */
      calculateDirection(viewpoint,&worldPix,&direction);

      /* trace the ray! */
      traceRay(&r,&c,1);
      /* write the pixel! */
      drawPixel(i,j,c.r,c.g,c.b);
    }
  }
}

/* returns the color seen by ray r in parameter c */
/* d is the recursive depth */
void traceRay(ray* r, color* c, int d) {
  point p;  /* first intersection point */
  vector n;
  material* m;

  p.w = 0.0;  /* inialize to "no intersection" */
  firstHit(r,&p,&n,&m);

  if (p.w != 0.0) {
    shade(&p,&n,m,r->dir,c,d);  /* do the lighting calculations */
  } else {             /* nothing was hit */
    c->r = 0.0f;
    c->g = 0.0f;
    c->b = 0.0f;
  }
}

/* firstHit */
/* If something is hit, returns the finite intersection point p, 
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
void firstHit(ray* r, point* p, vector* n, material* *m) {
  double t = 0;     /* parameter value at first hit */
  int hit1 = FALSE;
  int hit2 = FALSE;
  int hit3 = FALSE;
  int hit4 = FALSE;
  
  hit1 = raySphereIntersect(r,s1,&t);
  if(!hit1){
    hit2 = raySphereIntersect(r,s2,&t);
    if(!hit2){
      hit3 = rayCylinderIntersect(r,c1,&t);
      if(!hit3){
      //  hit4 = rayPlaneIntersect(r, pl1, &t);
      }
    }
  }
  if (hit1) {
    *m = s1->m;
    findPointOnRay(r,t,p);
    findSphereNormal(s1,p,n);
  }else if(hit2){
    *m = s2->m;
    findPointOnRay(r,t,p);
    findSphereNormal(s2,p,n);
  }else if(hit3){
    *m = c1->m;
    findPointOnRay(r,t,p);
    findCylinderNormal(c1,p,n);
  //} else if(hit4){
    //*m = pl1->m;
    //findPointOnRay(r,t,p);
    //n = pl1->n;
  }else {
    /* indicates no hit */
    p->w = 0.0;
  }
}