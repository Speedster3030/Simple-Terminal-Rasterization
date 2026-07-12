

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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
  POS_INFO P; //center and directions
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


POINT3D rotate_Z(POINT3D p, POINT3D c, int dir)
{
  float a= dir*0.1;
  float nx= c.x + (p.x-c.x)*cos(a) + (p.y-c.y)*sin(a);
  float ny= c.y + (p.y-c.y)*cos(a) - (p.x-c.x)*sin(a);

  return (POINT3D){nx,ny,p.z,p.P};
}

POINT3D rotate_Y(POINT3D p, POINT3D c, int dir)
{
  float a= dir*0.1;
  float nx= c.x + (p.x-c.x)*cos(a) + (p.z-c.z)*sin(a);
  float nz= c.z + (p.z-c.z)*cos(a) - (p.x-c.x)*sin(a);

  return (POINT3D){nx,p.y,nz,p.P};
}

POINT3D rotate_X(POINT3D p, POINT3D c, int dir)
{
  float a= dir*0.1;
  float ny= c.y + (p.y-c.y)*cos(a) + (p.z-c.z)*sin(a);
  float nz= c.z + (p.z-c.z)*cos(a) - (p.y-c.y)*sin(a);

  return (POINT3D){p.x,ny,nz,p.P};
}

POINTSCRN map(POINT3D p)
{
  p.z+= 200;
  float F= 150;
  int x= (int)(F*p.x/p.z);
  int y= (int)(F*p.y/p.z);

  return (POINTSCRN){x+(ROWS/2-1),y+(COLS/2-1)};
}

void display()
{
  int i,j;

  for(i=0;i<ROWS;i++)
  {
    for(j=0;j<COLS;j++)
    {
      printf("%c",buffer[i][j]);
    }
    printf("\n");
  }
}

void init(char ch)
{
  int i,j;

  for(i=0;i<ROWS;i++)
  {
    for(j=0;j<COLS;j++)
    {
      buffer[i][j]=ch;
    }
  }
}

POINTARR* torus(float r, float R, POINT3D ctr, POINT3D rot, int d1, int d2, int d3)
{
  int s=0;
  float i,j,x,y,z;

  POINTARR* arr= malloc(sizeof(POINTARR));

  for(i=0;i<=2*PI;i+=0.05)
  {
    for(j=0;j<=2*PI;j+=0.06)
    {
      x= ctr.x + (R+r*cos(i))*cos(j);
      y= ctr.y + (R+r*cos(i))*sin(j);
      z= ctr.z + r*sin(i);

      POS_INFO P= {rot.x,rot.y,rot.z,d1,d2,d3};
      arr->points[s]=(POINT3D){x,y,z,P};
      s++;
    }
  }

  arr->count= s;
  return arr;
}

POINTARR* bump(float r, float R, float c, POINT3D ctr, POINT3D rot, int d1, int d2, int d3)
{
  int s=0;
  float i,j,x,y,z;
  POS_INFO P;

  POINTARR* arr= malloc(sizeof(POINTARR));

  for(i=0;i<2*PI;i+=0.1)
  {
    for(j=0;j<PI;j+=0.1)
    {
      x= ctr.x + (R+r*sin(c*i)*sin(c*j))*sin(j)*cos(i);
      y= ctr.y + (R+r*sin(c*i)*sin(c*j))*sin(j)*sin(i);
      z= ctr.z + (R+r*sin(c*i)*sin(c*j))*cos(j);

      P= (POS_INFO){rot.x,rot.y,rot.z,d1,d2,d3};
      arr->points[s++]= (POINT3D){x,y,z,P};
    }
  }

  arr->count= s;
  return arr;
}

POINTARR* square(POINT3D rot, int d1, int d2, int d3)
{
  int s=0;
  float i,j,x,y,z,sx=-10,sy=-10;
  POINTARR* arr= malloc(sizeof(POINTARR));
  POS_INFO P= {rot.x,rot.y,rot.z,d1,d2,d3};

  for(i=0;i<100;i+=1)
  {
    for(j=0;j<100;j+=1)
    {
      x= sx + i;
      y= sy + j;
      arr->points[s++]= (POINT3D){x,y,5,P};
    }
  }

  arr->count= s;
  return arr;
}


int main()
{
  char ch= ' ';
  init(ch);

  POS_INFO P;
  POINT3D x= {0,0,0,P};
  POINT3D C= {-90,40,70,P};
  POINT3D C2= {90,-40,10,P};
  POINT3D C3= {90,40,80,P};
  POINT3D C4= {-90,-40,50,P};

  POINTARR* sld= bump(30,10,1,x,x,1,1,1);
  POINTARR* tor= bump(55,0,1,C,C,-1,0,0);
  POINTARR* sld2= bump(25,0,1,C2,C2,0,0,-1);
  POINTARR* sld3= bump(40,0,1,C3,C3,-1,0,0);
  POINTARR* tor2= bump(30,0,1,C4,C4,0,1,0);

  POINTARR* shapes[5]= {sld,tor,sld2,sld3,tor2};

  int i,k,ls= sizeof(shapes)/sizeof(shapes[0]),c=220;

  system("clear");char chr;

  printf("\e[?25l");

  while(c)
  {
    for(k=0;k<ls;k++)
    {
      for(i=0;i<shapes[k]->count;i++)
      {
        POINTSCRN p= map(shapes[k]->points[i]);
        if(p.x>=0 && p.y>=0 && p.x<ROWS && p.y<COLS)
        {
          buffer[p.x][p.y]='@';
        }
      }
    }

    display();

    for(k=0;k<ls;k++)
    {
      POINTARR* a= shapes[k];
      for(i=0;i<a->count;i++)
      {
        POINTSCRN p= map(a->points[i]);
        if(p.x>=0 && p.y>=0 && p.x<ROWS && p.y<COLS)
        {
          buffer[p.x][p.y]=ch;
        }

        POINT3D t= {a->points[i].P.a,a->points[i].P.b,a->points[i].P.c,P};
        a->points[i]= rotate_Y(a->points[i],t,a->points[i].P.da);
        a->points[i]= rotate_X(a->points[i],t,a->points[i].P.db);
        a->points[i]= rotate_Z(a->points[i],t,a->points[i].P.dc);
      }
    }

    usleep(50000);
    c--;
    printf("\033[H");

    //system("clear");
  }

  printf("\e[?25h");
  return 0;
}
