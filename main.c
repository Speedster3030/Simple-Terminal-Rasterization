


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "buffer.c"


int main()
{
  char ch= ' ';
  init(ch);

  POS_INFO P, S= {5,5,20,1,1,1};
  POINT3D x= {0,0,0,P};
  POINT3D C= {-90,40,70,P};
  POINT3D C2= {90,-40,10,P};
  POINT3D C3= {90,40,80,P};
  POINT3D C4= {-90,-40,50,P};

  POINTARR* sld= bump(70,0,2,x,x,0,1,0);
  //POINTARR* sld2= bu(20,28,C2,C2,-1,0,0);

  POINTARR* shapes[1]= {sld};

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
  //printf("This shape had %d points\n",l);
  return 0;
}
