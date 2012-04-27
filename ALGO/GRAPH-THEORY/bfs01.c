#include <stdio.h>

/*  global var */
int dx[]={1,0,-1,0},dy[]={0,1,0,-1};
/*  bfs input */
int x,y;              /*  maze size */
char m[128][128];     /*  layout */
/*  bfs work variables */
char t[128][128];     /*  visited? */
#define Q 65538       /*  need to be divisible by state size (3 here) */
int qs,qe,q[Q];   /*  q */

/*  0-1-bfs idiom 1: one queue (which gets pushed to in both ends),
    need to store distance in state */
/*  can be used for finding the shortest path in graphs having cost
    0 or 1 for entering _nodes_ */
int bfs01(int sx,int sy,int gx,int gy) {
  int d,cx,cy,x2,y2,moves,next;
  if(sx==gx && sy==gy) return 0;      /*  check for start==stop */
  qs=qe=0;
  q[qe++]=sx; q[qe++]=sy; q[qe++]=0;  /*  x,y,moves */
  memset(t,-1,sizeof(t));
  while(qs!=qe) {
    cx=q[qs++]; cy=q[qs++]; moves=q[qs++];
    if(qs==Q) qs=0;
    for(d=0;d<4;d++) {
      x2=cx+dx[d]; y2=cy+dy[d];
      if(x2<0 || y2<0 || x2>=x || y2>=y || t[x2][y2]>-1) continue;
      if(x2==gx && y2==gy) return moves;
      if(m[x2][y2]=='*') {  /*  cost 1 */
        q[qe++]=x2; q[qe++]=y2; q[qe++]=moves+1;
        if(qe==Q) qe=0;
        t[x2][y2]=moves+1;
      } else {  /*  cost 0 */
        if(qs==0) qs=Q;
        q[--qs]=moves; q[--qs]=y2; q[--qs]=x2;
        t[x2][y2]=moves;
      }
    }
  }
  return -1;
}

/*  TODO implement 0-1-bfs idiom 2: double loop with cost 1 in outer loop
    and cost 0 in inner loop. need 2 queues, don't need to have distance
    in state */

/*  solve the following problem: given a grid consisting of .*
    (. is free to move through, * costs 1 to enter), and start/
    goal S/G (which is also free to move through), return the
    cheapest cost of moving from S to G.
*/

void crossing() {
  int cases,i,j,sx,sy,gx,gy;
  scanf("%d",&cases);
  while(cases--) {
    scanf("%d %d",&x,&y);
    for(i=0;i<x;i++) scanf("%s",m[i]);
    for(i=0;i<x;i++) for(j=0;j<y;j++) if(m[i][j]=='S') sx=i,sy=j,m[i][j]='.';
    else if(m[i][j]=='G') gx=i,gy=j,m[i][j]='.';
    printf("%d\n",bfs01(sx,sy,gx,gy));
  }
}

int main() {
  crossing();
}
