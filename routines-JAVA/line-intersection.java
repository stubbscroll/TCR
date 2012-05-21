/* line intersection */

/* 18.10.2007: SRM 368 div1-500 OK */

class Line {
   	public int x1,y1,x2,y2; /* line segment from (x1,y1) to (x2,y2) */
   	public Line(int x1,int y1,int x2,int y2) {
   		this.x1=x1; this.y1=y1;
   		this.x2=x2; this.y2=y2;
   	}
   	/*	robust test to see if two line segments intersect
   	returns:
   		no intersection: {0}
   		parallel and overlapping lines: {1}
   		otherwise, intersection point as a fraction: {1, xteller, xnevner, yteller, ynevner}
   	*/
   	public long[] linesIntersect(Line line) {
   		// special case, both lines are points
   		if(x1==x2 && y1==y2 && line.x1==line.x2 && line.y1==line.y2) {
   			if(x1==line.x1 && y1==line.y1) return new long[] {1, x1,1,y1,1};
   			else return new long[] {0};
   		}
   		long a1 = y2 - y1, b1 = x1 - x2, c1 = x2*y1 - x1*y2;
   		long r3 = a1*line.x1 + b1*line.y1 + c1, r4 = a1*line.x2 + b1*line.y2 + c1;
   		if(r3!=0 && r4!=0 && (r3<0 == r4<0)) return new long[] {0};
   		long a2 = line.y2 - line.y1, b2 = line.x1 - line.x2, c2 = line.x2*line.y1 - line.x1*line.y2;
   		long r1 = a2*x1 + b2*y1 + c2, r2 = a2*x2 + b2*y2 + c2;
   		if(r1!=0 && r2!=0 && (r1<0 == r2<0)) return new long[] {0};
   		if((long)a1*b2 == (long)a2*b1) {
   			// colinear, check for overlap
   			int x1=this.x1, y1=this.y1, x2=this.x2, y2=this.y2;
   			int temp, x3=line.x1, y3=line.y1, x4=line.x2, y4=line.y2;
   			if((x1==x2 && y1!=y2) || (x3==x4 && y3!=y4)) {
   				temp=x1; x1=y1; y1=temp;
   				temp=x2; x2=y2; y2=temp;
   				temp=x3; x3=y3; y3=temp;
   				temp=x4; x4=y4; y4=temp;
   			}
   			if(x1>x2) { temp=x1; x1=x2; x2=temp; }
   			if(x3>x4) { temp=x3; x3=x4; x4=temp; }
   			if(x1>x3) { temp=x1; x1=x3; x3=temp; temp=x2; x2=x4; x4=temp; }
   			return new long[] { x2>=x3 ? 1 : 0 };
   		}
   		return new long[] { 1, b1*c2 - b2*c1, a1*b2 - a2*b1,
   							   a2*c1 - a1*c2, a1*b2 - a2*b1 };
   	}
   	public String toString() {
   	   return "("+x1+" "+y1+" "+x2+" "+y2+")";
   	}
}
