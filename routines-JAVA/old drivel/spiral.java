	/* spiral routines:
		789 (-1, 1) (0, 1) (1, 1)
		612 (-1, 0) (0, 0) (1, 0)
		543 (-1,-1) (0,-1) (1,-1)
	*/

	/* return coordinates of n >= 1 in spiral, O(1) */
	int[] spiralxy(int n) {
		int q=(int)Math.sqrt(n),dx=0,dy=-1,r[]=new int[2];
		if((q&1)==0) q--;
		int left=n-q*q;
		r[0]=r[1]=q/2;
		if(left==0) return r;
		r[0]++;
		left--;
		for(int i=0;i<4;i++) {
			if(left<=q) {
				r[0]+=dx*left;
				r[1]+=dy*left;
				return r;
			}
			left-=q;
			r[0]+=dx*q;
			r[1]+=dy*q;
			if(i==0) q++;
			int t=dx;
			dx=dy;
			dy=-t;
		}
		return r;
	}

	/* return the number at the coordinates x,y in spiral */
	int spiral(int x,int y) {
		int m=Math.max(Math.abs(x),Math.abs(y));
		int q=(m*2+1)*(m*2+1);
		if(y==m) return q+x-m;
		else if(x==m) return q-m*7-y;
		else if(m==-x) return q-m*3+y;
		else return q-x-m*5;
	}
