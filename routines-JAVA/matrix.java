/* matrix exponentiation, see uva 10702 */

/*  we seek shortest path from s to t in z moves, we have cost matrix A.
	let B=A^z, desired solution is B[s][t] */

public class matrix {

	double EPS = 1e-8;

	void printmatrix(int [][]m) {
		for(int i=0;i<m.length;i++) {
			for(int j=0;j<m.length;j++) System.out.printf("%d ",m[i][j]);
			System.out.println();
		}
		System.out.println();
	}

	long[][] mul(long a[][],long b[][]) {
		int n=a.length;
		long res[][]=new long[n][n];
		for(int k=0;k<n;k++) for(int i=0;i<n;i++) for(int j=0;j<n;j++) {
//			res[i][j]=Math.max(a[i][k]+b[k][j],res[i][j]);	// maximise/minimise
//			res[i][j]+=a[i][k]*b[k][j];						 // multiply
			res[i][j]=(res[i][j]+a[i][k]*b[k][j])%234324;	 // multiply modulo something
		}
		return res;
	}
	long[][] mulpow(long a[][],long orig[][],int pow) {
		if(pow==1) return orig;
		int n=a.length;
		long res[][]=mulpow(a,orig,pow/2);
		res=mul(res,res);
		if((pow&1)>0) res=mul(res,orig);
		return res;
	}

	// find value such that a * value = 1 (mod b)
	int inverse(int a,int b) {
		int[] u=new int[]{1,0,a},v=new int[]{0,1,b};
		while(v[2]!=0) {
			int q=u[2]/v[2];
			for(int i=0;i<3;i++) {
				int t=v[i]; v[i]=u[i]-q*v[i]; u[i]=t;
			}
		}
		return (u[0]%b+b)%b;
	}

	// finn determinant modulo mod O(n^3)
	int det(int[][] M,int mod) {
		int factor=1,n=M.length,t;
		int m[][]=new int[n][];
		for(int i=0;i<n;i++) m[i]=M[i].clone();
		for(int i=0;i<n;i++) for(int j=0;j<n;j++) m[i][j]=(m[i][j]%mod+mod)%mod;
		for(int i=0;i<n;i++) {
			if(m[i][i]==0) {
				// we need to interchange with a row with != 0 in this cell
				int j=i+1;
				for(;j<n;j++) if(m[j][i]!=0) break;
				if(j==n) return 0;
				for(int k=0;k<n;k++) {
					t=m[j][k]; m[j][k]=m[i][k]; m[i][k]=t;
				}
				// multiply factor by -1
				factor=mod-factor;
			}
			if(m[i][i]!=1) {
				// make it 1
				int inv=inverse(m[i][i],mod),uninv=m[i][i];
				factor=(int)((long)factor*m[i][i]%mod);
				for(int j=0;j<n;j++) m[i][j]=(int)((long)m[i][j]*inv%mod);
			}
			// subtract such that all rows below has 0 in column
			for(int j=i+1;j<n;j++) if(m[j][i]!=0) {
				int mul=mod-m[j][i];
				for(int k=0;k<n;k++) m[j][k]=(int)((m[j][k]+(long)mul*m[i][k])%mod);
			}
		}
		return factor;
	}

	boolean deq(double a,double b) { return Math.abs(a-b)<EPS; }

	// find determinant with gaussian elimination
	double detgauss(double a[][]) {
		double adjust=1,r;
		int n=a.length,i,j,k,l;
		for(i=0;i<n;i++) {
			for(j=0;j<n;j++) {
				for(k=0;k<i;k++) if(!deq(a[j][k],0.0)) return -123123123;
				if(deq(a[j][i],0.0)) return -123123123;
				adjust*=a[j][i];
				for(l=i+1;l<n;l++) a[j][l]/=a[j][i];
				a[j][i]=1;
				for(k=0;k<n;k++) if(k!=j) {
					r=a[k][i];
					for(l=i;l<n;l++) a[k][l]-=a[j][l]*r;
					a[k][i]=0;
         		}
			}
   		}
   		for(i=0;i<n;i++) adjust*=a[i][i];
   		return adjust;
	}

	int brutedet(int [][] a,int mod) {
		int n=a.length,sign=1;
//		printmatrix(a);
		int ans=0;
		if(n==2) return a[0][0]*a[1][1]-a[1][0]*a[0][1];
		for(int i=0;i<n;i++,sign*=-1) {
			int b[][]=new int[n-1][n-1];
			for(int x=0,xx=0;x<n;x++) if(x!=i) {
				for(int y=0,yy=0;y<n;y++) if(y!=0) {
					b[xx][yy++]=a[x][y];
				}
				xx++;
			}
			long res=sign*a[i][0]*brutedet(b,mod)%mod;
			if(res<0) res+=mod;
			ans+=(int)res;
		}
		return ans;
	}

	int MOD = 65537;
	void test() {
		int rett=0,feil=0,test;
		for(test=0;test<101231230;test++) {
			int n=7;
			int a[][]=new int[n][n];
			double b[][]=new double[n][n];
			for(int i=0;i<n;i++) for(int j=0;j<n;j++) {
				b[i][j]=a[i][j]=(int)(Math.random()*10+1);
			}
			int ans=det(a,MOD);
			int ans2=brutedet(a,MOD);
			if((ans+MOD)%MOD!=(ans2+MOD)%MOD) {
/*				System.out.printf("feil\n");
				printmatrix(a);
				System.out.printf("%d %d\n",ans,ans2);*/
				feil++;
			} else { rett++; }
			if(test%100==0) System.out.printf("(%d %d)\n",rett,feil);
		}
		System.out.printf("rett %d feil %d av %d\n",rett,feil,test);
	}

	void testinv() {
		while(true) {
			int a=(int)(Math.random()*100000+1);
			int b=(int)(Math.random()*a+1);
			for(int i=2;i<a;i++) if((long)b*i%a==1) {
				int inv=inverse(b,a);
				if(inv!=i) System.out.printf("feil inv %d (mod %d) rett %d feil %d\n",a,b,inv,i);
				break;
			}
			System.out.printf(".");
		}
	}

	// ************** determinant mod, version for long ***********************

	// find value such that a * value = 1 (mod b)
	long longinverse(long a,long b) {
		long[] u=new long[]{1,0,a},v=new long[]{0,1,b};
		while(v[2]!=0) {
			long q=u[2]/v[2];
			for(int i=0;i<3;i++) {
				long t=v[i]; v[i]=u[i]-longmulmod(q,v[i],b); u[i]=t;
			}
		}
		return (u[0]%b+b)%b;
	}

	// multiply two longs
	long longmulmod(long a,long b,long mod) {
		long res=0;
		while(a>0) {
			if((a&1)>0) res=(res+b)%mod;
			a>>=1;
			b=(b<<1)%mod;
		}
		return res;
	}

	// finn determinant modulo mod O(n^3)
	long longdet(long[][] M,long mod) {
		int n=M.length;
		long factor=1,t;
		long m[][]=new long[n][];
		for(int i=0;i<n;i++) m[i]=M[i].clone();
		for(int i=0;i<n;i++) for(int j=0;j<n;j++) m[i][j]=(m[i][j]%mod+mod)%mod;
		for(int i=0;i<n;i++) {
			if(m[i][i]==0) {
				// we need to interchange with a row with != 0 in this cell
				int j=i+1;
				for(;j<n;j++) if(m[j][i]!=0) break;
				if(j==n) return 0;
				for(int k=0;k<n;k++) {
					t=m[j][k]; m[j][k]=m[i][k]; m[i][k]=t;
				}
				// multiply factor by -1
				factor=mod-factor;
			}
			if(m[i][i]!=1) {
				// make it 1
				long inv=longinverse(m[i][i],mod),uninv=m[i][i];
				factor=longmulmod(factor,m[i][i],mod);
				for(int j=0;j<n;j++) m[i][j]=longmulmod(m[i][j],inv,mod);
			}
			// subtract such that all rows below has 0 in column
			for(int j=i+1;j<n;j++) if(m[j][i]!=0) {
				long mul=mod-m[j][i];
				for(int k=0;k<n;k++) m[j][k]=(m[j][k]+longmulmod(mul,m[i][k],mod))%mod;
			}
		}
		return factor;
	}



	public static void main(String a[]) { new matrix().entry();}
	void entry() {
		long a[][]=new long[10][10];

		int x[][]=new int[4][];
		x[0]=new int[]{2,-1,0,0};
		x[1]=new int[]{-1,4,-1,-1};
		x[2]=new int[]{0,-1,1,0};
		x[3]=new int[]{0,-1,0,1};
		System.out.printf("trest %d\n",brutedet(x,65537));
		System.out.printf("det %d\n",det(x,5));
		System.out.printf("det %d\n",det(x,7));
		System.out.printf("det %d\n",det(x,11));
		System.out.printf("det %d\n",det(x,13));

//		a=mulpow(a,a,123); // return A^123

//		testinv();
		test();
	}
}
