

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
    POS_INFO P;   // to help with individual rotations

}POINT3D;

typedef struct
{
    int x;
    int y;

}POINTSCRN;


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
    float F= 100;
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

/*

POINT3D* ellipse(int* size)
{
    int s=0;
    float i,a=20,b=20;
    float x,y;

    POINT3D* points= malloc(5000*sizeof(POINT3D));

    for(i=0;i<=2*PI;i+=0.002)
    {
        x= a*cos(i);
        y= b*sin(i);
        points[s]= (POINT3D){y,x,5};
        s++;
    }

    *size= s;
    return points;
}

POINT3D* cone(int* size)
{
    float i,j;
    int s=0;

    float h=35,r=10,x,y,z;

    POINT3D* points= malloc(17000*sizeof(POINT3D));

    for(i=0;i<=1;i+=0.02)
    {
        for(j=0;j<=2*PI;j+=0.02)
        {
            x= (1-i)*r*cos(j);
            y= (1-i)*r*sin(j);
            z= i*h-h/2;

            points[s]= (POINT3D){x,y,z};
            s++;
        }
    }

    *size= s;
    return points;
}*/

POINT3D* torus(float r, float R, POINT3D ctr, POINT3D rot, int d1, int d2, int d3, int* size)
{
    int s=0;
    float i,j,x,y,z,a,b,c;

    POINT3D* points= malloc(15000*sizeof(POINT3D));

    for(i=0;i<=2*PI;i+=0.05)
    {
        for(j=0;j<=2*PI;j+=0.06)
        {
            x= ctr.x + (R+r*cos(i))*cos(j);
            y= ctr.y + (R+r*cos(i))*sin(j);
            z= ctr.z + r*sin(i);
            a= rot.x;
            b= rot.y;
            c= rot.z;

            POS_INFO P= {a,b,c,d1,d2,d3};
            points[s]=(POINT3D){x,y,z,P};
            s++;
        }
    }

    *size= s;
    return points;
}


int main()
{
    char ch= ' ';
    init(ch);

    int la,lb,l,lp,ld;
    POS_INFO P;
    POINT3D t= {40,1,1,P};
    POINT3D x= {0,0,0,P};
    POINT3D C= {-60,0,160,P};

    POINT3D* line= torus(7.5,30,x,x,-1,1,-1,&l);

    int i,c=220;

    system("clear");

    printf("\e[?25l");

    while(c)
    {
        for(i=0;i<l;i++)
        {
            POINTSCRN p= map(line[i]);
            if(p.x>=0 && p.y>=0 && p.x<ROWS && p.y<COLS)
            {
                buffer[p.x][p.y]='@';
            }
        }

        display();

        for(i=0;i<l;i++)
        {
            POINTSCRN p= map(line[i]);
            if(p.x>=0 && p.y>=0 && p.x<ROWS && p.y<COLS)
            {
                buffer[p.x][p.y]=ch;
            }

            POINT3D t= {line[i].P.a,line[i].P.b,line[i].P.c,P};
            line[i]= rotate_Y(line[i],t,line[i].P.da);
            line[i]= rotate_X(line[i],t,line[i].P.db);
            line[i]= rotate_Z(line[i],t,line[i].P.dc);
            t= rotate_Z(t,C,1);
            line[i].P.a= t.x;
            line[i].P.b= t.y;
            line[i].P.c= t.z;
            line[i]= rotate_Z(line[i],C,1);
        }

        usleep(50000);
        c--;
        printf("\033[H");

        //system("clear");
    }

    printf("\e[?25h");
    printf("This shape had %d points\n",l);
    return 0;
}
