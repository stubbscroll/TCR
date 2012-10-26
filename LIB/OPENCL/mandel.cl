__kernel void mandel(__global int *a,float xleft,float yupper,float step,int XSIZE,int YSIZE) {
  const int i=get_global_id(0);
  const int j=get_global_id(1);
	float cx,cy,zx,zy,tempx;
	int iter=0;
	if(i<XSIZE && j<YSIZE) {
		zx=cx = (xleft + step*i);
		zy=cy = (yupper - step*j);
		while(zx*zx + zy*zy < 4.0) {
			tempx = zx*zx - zy*zy + cx;
			zy = 2.0*zx*zy + cy;
			zx = tempx;
			if(++iter==255) break;
		}
		a[i+j*XSIZE]=iter;
	}
}
