/* spiral routines:
   789 (-1, 1) (0, 1) (1, 1)
   612 (-1, 0) (0, 0) (1, 0)
   543 (-1,-1) (0,-1) (1,-1)
*/

/* return coordinates of n >= 1 in spiral, O(1) */
void spiralxy(int n,int *x,int *y)
{
   int q=sqrt(n),left,i,dx=0,dy=-1,t;
   if((q&1)==0) q--;
   left=n-q*q;
   *x=*y=q>>1;
   if(left==0) return;
   (*x)++;
   left--;
   for(i=0;i<4;i++) {
      if(left<=q) {
         *x+=dx*left;
         *y+=dy*left;
         return;
      }
      left-=q;
      *x+=dx*q;
      *y+=dy*q;
      if(i==0) q++;
      t=dx;
      dx=dy;
      dy=-t;
   }
}

/* return the number at the coordinates x,y in spiral */
int spiral(int x,int y)
{
   int m=abs(x),q;
   if(abs(y)>m) m=abs(y);
   q=(m*2+1)*(m*2+1);
   if(y==m) return q+x-m;
   else if(x==m) return q-m*7-y;
   else if(m==-x) return q-m*3+y;
   else return q-x-m*5;
}

