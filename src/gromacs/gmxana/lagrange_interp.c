// From Numerical Recipes in C.
#include <math.h>
#include "nrutil.h"
void polint(float xa[], float ya[], int n, float x, float *y, float *dy)
{
	int i,m,ns=1;
	float den,dif,dift,ho,hp,w;

	float *c,*d;

	dif=fabs(x-xa[1]);
	c=vector(1,n);
	d=vector(1,n);
	for (i=1;i<=n;i++) {
		if ( (dift=fabs(x-xa[i])) < dif) {
			ns=i;
			dif=dift;
		}
		c[i]=ya[i];
		d[i]=ya[i];
	}
	*y=ya[ns--];
	for (m=1;m<n;m++) {
		for (i=1;i<=n-m;i++) {
			ho=xa[i]-x;
			hp=xa[i+m]-x;
			w=c[i+1]-d[i];
			if ( (den=ho-hp) == 0.0) nrerror("Error in routine polint");
			den=w/den;
			d[i]=hp*den;
			c[i]=ho*den;
		}
		*y += (*dy=(2*ns < (n-m) ? c[ns+1] : d[ns--]));
	}
	free_vector(d,1,n);
	free_vector(c,1,n);
}


void polin2(float x1a[], float x2a[], float **ya, int m, int n, float x1,float x2, float *y, float *dy)
{
	int j;
	float *ymtmp;
	ymtmp=vector(1,m);
	for (j=1;j<=m;j++) {
		polint(x2a,ya[j],n,x2,&ymtmp[j],dy);
	}
	polint(x1a,ymtmp,m,x1,y,dy);
	free_vector(ymtmp,1,m);
}

void polin3(float x1a[], float x2a[], float x3a[], float ***yb, int d1, int d2, int d3, float x1, float x2, float x3, float *y, float *dy)
{

	float **squarevals,ytmp;
	int i;

	snew(squarevals,d1);
	for (i=0;i<d1;i++)
	{
		snew(squarevals[i],d2);
	}

	for (i=1;i<=d1;i++)
	{
		for (j=1;j<=d1;j++)
		{
			// We have chosen a given set of (x,y) values; now we interpolate along the z direction.
			polint(x3a,yb[i][j],d3,x3,squarevals[i-1][j-1],dy);
		}
	}

	// Now, given a square that contains some values, we can just do 2d interpolation on it.
	polin2(x1a,x2a,squarevals,d1,d2,x1,x2,&y,&dy);

}