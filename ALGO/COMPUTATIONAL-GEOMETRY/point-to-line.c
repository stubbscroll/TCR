/* TODO point to line in 2d */

/* squared distance 3d */
double distsq3d(double x1,double y1,double z1,double x2,double y2,double z2) {
	double dx=x1-x2,dy=y1-y2,dz=z1-z2;
	return dx*dx+dy*dy+dz*dz;
}

/* distance (3d) from point (x,y,z) to line segment (x1,y1,z1)-(x2,y2,z2) */
/* OK idi open 2013 "space travel" 2013-04-27 */
double pointtoline3d(double x,double y,double z,double x1,double y1,
                     double z1,double x2,double y2,double z2) {
	double vx=x2-x1, vy=y2-y1, vz=z2-z1;
	double wx=x-x1, wy=y-y1, wz=z-z1;
	double c1=vx*wx+vy*wy+vz*wz;
	/* left endpoint closest? */
	if(c1<0) return sqrt(distsq3d(x,y,z,x1,y1,z1));
	double c2=vx*vx+vy*vy+vz*vz;
	/* right endpoint closest? */
	if(c2<=c1) return sqrt(distsq3d(x,y,z,x2,y2,z2));
	double b=c1/c2;
	/* find the actual point in the segment */
	double bx=x1+vx*b, by=y1+vy*b, bz=z1+vz*b;
	return sqrt(distsq(bx,by,bz,x,y,z));
}

/* find shortest distance between two line segments in 3d:
   do ternary search on one of the lines */
