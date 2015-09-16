#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <math.h>


typedef struct struct_Point2 {
	float x;
	float y;
} Point2;

typedef struct struct_vector {
  float x;
  float y;
} Vector;


static void myInit() {
  glClearColor(1, 0, 0, 0);
  glColor3f(0, 0, 1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, 640, 0, 480);
}


Point2 polygonPoints[] = {{100, 100}, {100, 250}, {300, 400}, {350, 350}, {400, 150}};

Point2 linePoints[] = {{0, 400}, {600, 0}};

static void drawConvexPolygon() {
  int length = sizeof(polygonPoints) / sizeof(Point2);
  int i = 0;
  Point2 point;

  glBegin(GL_LINE_LOOP);
  for(; i < length; i++) {
    point = polygonPoints[i];
    glVertex2f(point.x, point.y);
  }
  glEnd();
}

static void drawLine() {
   glBegin(GL_LINES);
   glVertex2f(linePoints[0].x, linePoints[0].y);
   glVertex2f(linePoints[1].x, linePoints[1].y);
   glEnd();
}

static float dotProduct(Vector v1, Vector v2) {
  return v1.x * v2.x + v1.y * v2.y;
}

/**
 * 取得两条直线的交点.（两条直线分别过point1/point2和point3/point4）
 *
 * 0: 有交点
 * 1: 无交点
 * 2: 重和
 */
static int getIntersectionPoint(Point2 point1, Point2 point2, Point2 point3, Point2 point4, float *t, int *inOut) {
  Vector v12 = {point2.x - point1.x, point2.y - point1.y};
  Vector v34 = {point4.x - point3.x, point4.y - point3.y};
  Vector v34Orth = {-v34.y, v34.x};

  float dotProcV12V34orth = dotProduct(v12, v34Orth);
  if(dotProcV12V34orth == 0) {
    Vector v23 = {point3.x - point2.x, point3.y - point2.y};
    float docProcV23V34orth = dotProduct(v23, v34Orth);
    if(docProcV23V34orth == 0) {
      return 2;
    } else {
      return 1;
    }
  }

  if(dotProcV12V34orth < 0) {
    *inOut = 0;    //out
  } else {
    *inOut = 1;    //in
  }

  Vector v13 = {point3.x - point1.x, point3.y - point1.y};
  float dotProcV13V34orth = dotProduct(v13, v34Orth);
  *t = dotProcV13V34orth / dotProcV12V34orth; 
  return 0;
}

static void drawLineInPolygon() {
  Point2 linePoint1 = linePoints[0];
  Point2 linePoint2 = linePoints[1];
  Point2 polyPoint1 = polygonPoints[0];
  Point2 polyPoint2 = polygonPoints[1];
  Point2 temp;
  int polyPCount = sizeof(polygonPoints) / sizeof(Point2);
  int index;
  int result;
  int inOut;
  float x,y;
  float t;  
  float in = 0;
  float out = 1;


  Vector lineVector = {linePoint2.x - linePoint1.x, linePoint2.y - linePoint1.y};
  Vector lineOrth = {-lineVector.y, lineVector.x};
  
  
  for(index = 0; index < 5; index++) {
    result = getIntersectionPoint(linePoint1, linePoint2, polygonPoints[index], polygonPoints[(index + 1)% polyPCount], &t, &inOut);
    x = linePoint1.x + t * lineVector.x;
    y = linePoint1.y + t * lineVector.y;
    
    if(result == 1) {
      printf("#######parallel#######\n");
      continue;
    }
    if(result == 2) {
      printf("#######coincide########\n");
      break;
    }

    if(inOut == 0) {
      if(t - in > 0) {
	in = t;
      }
    } else {
      if(t - out < 0) {
        out = t;
      }
    }
 }
  

  if(in >= out) {
    printf("##########out of polygon######\n");
    return;
  }

  float basex = linePoint1.x;
  float basey = linePoint1.y;
  glPointSize(6.0f);
  glBegin(GL_POINTS);
  glVertex2f(basex + in * lineVector.x, basey + in * lineVector.y);
  glVertex2f(basex + out * lineVector.x, basey + out * lineVector.y);
  glEnd();      
}

static void render() {
  glClear(GL_COLOR_BUFFER_BIT);

  drawConvexPolygon();
  drawLine();

  drawLineInPolygon();
  
  glutSwapBuffers();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Cyrus-Beck");

  myInit();
  
  glutDisplayFunc(render);

  glutMainLoop();
  
  return 0;
}
