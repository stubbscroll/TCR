/* Construct suffix array in linear time */
/* Warning: code (except LCP) stolen from a paper, don't use on topcoder */

/* 07.11.2007: Algbio event OK */

class SuffixArray {
	public int[] genSuffixArray(String s) {
		int n=s.length(),maks=0,min=Integer.MAX_VALUE;
		int z[]=new int[n+3];
		for(int i=0;i<n;i++) {
			if(maks<s.charAt(i)) maks=s.charAt(i);
			if(min>s.charAt(i)) min=s.charAt(i);
		}
		min--;
		for(int i=0;i<n;i++) z[i]=s.charAt(i)-min;
		int a[]=new int[n];
		suffixArray(z,a,n,maks-min);
		return a;
	}
	public void suffixArray(int s[],int SA[],int n,int K) {
		int n0=(n+2)/3, n1=(n+1)/3, n2=n/3, n02=n0+n2;
		int[] s12=new int[n02+3],SA12=new int[n02+3],s0=new int[n0],SA0=new int[n0];
		for(int i=0,j=0;i<n+n0-n1;i++) if(i%3>0) s12[j++]=i;
		radixPass(s12,SA12,s,2,n02,K);
		radixPass(SA12,s12,s,1,n02,K);
		radixPass(s12,SA12,s,0,n02,K);
		int name=0,c0=-1,c1=-1,c2=-1;
		for(int i=0;i<n02;i++) {
			if(s[SA12[i]]!=c0 || s[SA12[i]+1]!=c1 || s[SA12[i]+2]!=c2) {
				name++; c0=s[SA12[i]]; c1=s[SA12[i]+1]; c2=s[SA12[i]+2];
			}
			if(SA12[i]%3==1) s12[SA12[i]/3]=name;
			else s12[SA12[i]/3+n0]=name;
		}
		if(name<n02) {
			suffixArray(s12,SA12,n02,name);
			for(int i=0;i<n02;i++) s12[SA12[i]]=i+1;
		} else for(int i=0;i<n02;i++) SA12[s12[i]-1]=i;
		for(int i=0,j=0;i<n02;i++) if(SA12[i]<n0) s0[j++]=3*SA12[i];
		radixPass(s0,SA0,s,0,n0,K);
  		for(int p=0,t=n0-n1,k=0;k<n;k++) {
			int i=(SA12[t]<n0?SA12[t]*3+1:(SA12[t]-n0)*3+2),j=SA0[p];
			if(SA12[t]<n0?
					leq(s[i],s12[SA12[t]+n0],s[j],s12[j/3]):
					leq(s[i],s[i+1],s12[SA12[t]-n0+1],s[j],s[j+1],s12[j/3+n0])) {
				SA[k]=i; t++;
				if(t==n02) for(k++;p<n0;p++,k++) SA[k]=SA0[p];
			} else {
				SA[k]=j; p++;
				if(p==n0) for(k++;t<n02;t++,k++) SA[k]=(SA12[t]<n0?SA12[t]*3+1:(SA12[t]-n0)*3+2);
			}
		}
	}
	public void radixPass(int a[],int b[],int r[],int o,int n,int K) {
		int c[]=new int[K+1],t;
		for(int i=0;i<n;i++) c[r[o+a[i]]]++;
		for(int i=0,sum=0;i<=K;i++) { t=c[i]; c[i]=sum; sum+=t; }
		for(int i=0;i<n;i++) b[c[r[o+a[i]]]++]=a[i];
	}
	public boolean leq(int a1,int a2,int b1,int b2) {
		return a1<b1 || (a1==b1 && a2<=b2);
	}
	public boolean leq(int a1,int a2,int a3,int b1,int b2,int b3) {
		return a1<b1 || (a1==b1 && leq(a2,a3,b2,b3));
	}
	/* index [i] contains the number of equal characters in suffixes i-1 and i */
	int rank[];
	public int[] LCP(String s,int sa[]) {
		int n=s.length(),height[]=new int[n],h=0;
		rank=new int[n+1];
		for(int i=0;i<n;i++) rank[sa[i]]=i;
		for(int i=0;i<n;i++) if(rank[i]>0) {
			while(sa[rank[i]-1]+h<n && sa[rank[i]]+h<n && s.charAt(sa[rank[i]-1]+h)==s.charAt(sa[rank[i]]+h)) h++;
			height[rank[i]]=h;
			if(h>0) h--;
		}
		return height;
	}
}
