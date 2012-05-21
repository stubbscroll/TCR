struct p_s {
   int x,y;
};

int ccw(int a,int b,int c)
{
   return p[a].x * p[b].y - p[a].y * p[b].x + p[a].y * p[c].x
      -p[a].x * p[c].y + p[b].x * p[c].y - p[c].x * p[b].y;
}


