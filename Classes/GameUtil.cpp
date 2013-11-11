#include "GameUtil.h"
#include "math.h"

double MathUtil::lineSpace( double x1, double y1,double x2, double y2 )
{
	double lineLength = 0;
	lineLength = sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
	return lineLength;
}  

double MathUtil::pointToLine(double x1, double y1,  double x2, double y2,  double x0, double y0, int& out)    
{    
	double space = 0;    
	double a, b, c;    
	a = lineSpace(x1, y1, x2, y2);
	b = lineSpace(x1, y1, x0, y0);
	c = lineSpace(x2, y2, x0, y0);

	if (c <= 0.000001 || b <= 0.000001)
	{
		space = 0;    
		return space;    
	}    

	if (a <= 0.000001)
	{
		space = b;    
		return space;    
	}    
	if (c * c >= a * a + b * b)
	{
		out = 1;
		space = b;    
		return space;    
	}
	if (b * b >= a * a + c * c)
	{
		out = 2;
		space = c;    
		return space;    
	}

	double p = (a + b + c) / 2;
	double s = sqrt(p * (p - a) * (p - b) * (p - c));
	space = 2 * s / a;

	return space;    
}   

int MathUtil::pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
	int i, j, c = 0;
	for (i = 0, j = nvert-1; i < nvert; j = i++) 
	{
		if ( ((verty[i]>testy) != (verty[j]>testy)) &&
			(testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
			c = !c;
	}
	return c;
}
