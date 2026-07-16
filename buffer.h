

#ifndef BUFFER_H
#define BUFFER_H

#define ROWS 150
#define COLS 100
#define PI 3.14

char buffer[ROWS][COLS];


typedef struct
{
  float a,b,c;
  int da,db,dc;
}POS_INFO;


typedef struct
{
  float x,y,z;
  POS_INFO P;   // center and directions
}POINT3D;


typedef struct
{
  int x;
  int y;
}POINTSCRN;


typedef struct
{
  POINT3D points[30000];
  int count;
}POINTARR;


POINT3D rotate_Z(POINT3D p, POINT3D c, int dir);

POINT3D rotate_Y(POINT3D p, POINT3D c, int dir);

POINT3D rotate_X(POINT3D p, POINT3D c, int dir);

POINTSCRN map(POINT3D p);

void display();

void init(char ch);

POINTARR* torus(float r, float R, POINT3D ctr, POINT3D rot, int d1, int d2, int d3);

POINTARR* bump(float r, float R, float c, POINT3D ctr, POINT3D rot, int d1, int d2, int d3);

POINTARR* square(POINT3D rot, int d1, int d2, int d3);



#endif
