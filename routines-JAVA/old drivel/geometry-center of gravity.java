	// signed triangle area*2
	long striarea2(int x[],int y[],int a,int b,int c) {
		return (long)x[a]*y[b]+(long)x[b]*y[c]+(long)x[c]*y[a]
           -(long)x[b]*y[a]-(long)x[c]*y[b]-(long)x[a]*y[c];

	}

	// calculate center of gravity of any non-intersecting polygon with total area != 0
	double[] polygongravcenter(int x[],int y[],int n)
	{
		long totarea=0;
		long wx=0,wy=0;
		for(int i=1;i<n-1;i++) {
			long tarea=striarea2(x,y,0,i,i+1);
			totarea+=tarea;
			wx+=tarea*(x[0]+x[i]+x[i+1]);
			wy+=tarea*(y[0]+y[i]+y[i+1]);
		}
		double d[]=new double[2];
		d[0]=wx/(totarea*3.0);
		d[1]=wy/(totarea*3.0);
		return d;
	}
