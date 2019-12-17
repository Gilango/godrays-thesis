#include "Bundle.h"
#include "Vector.h"

using namespace std;
using namespace gRays;

void Bundle :: print()
{
	for(int i=0; i<3; i++)
	{
		cout<<endl<<"Vertex "<<i<<": "<<_vertices[i];
		cout<<endl<<"Normal "<<i<<": "<<_normals[i];
		cout<<endl<<"Edge "<<i<<": "<<_edges[i];
		cout<<endl<<"RRay "<<i<<": "<<_refractedRays[i];
		
	}	
}	

void Bundle :: swap(double r, double s, double t)
{
	_r[1] = _r[0];
	_s[1] = _s[0];
	_t[1] = _t[0];
	
	_r[0] = r;
	_s[0] = s;
	_t[0] = t;
}	

void Bundle :: saveRST(double r, double s, double t)
{
	if(_iVal == 0)
	{
		_r[0] = r;
		_s[0] = s;
		_t[0] = t;
		_iVal++;
	}
	
	else
	{
		if(_r[0] > r)
			swap(r, s, t);
			
		else
		{
			_r[1] = r;
			_s[1] = s;
			_t[1] = t;
		}		
	}	
		
}	

/*bool Bundle :: eval()
{
	double r, s, t, intersection;
	int intersectionPoints = 0;
	
	
	for(int i=0; i<3; i++)
	{
		//result[i] = findRoot(i);
			
		t = findRoot(i);
		
		if(t >= 0.0 && t <= 1.0)
		{	
		//	cout<<"value of t: "<<t<<endl;
			s = evalS(t, i);
			
			if(s >= 0.0)
			{
				r = evalR(t, i);
				
				if(r >= 0.0)
				{
				//	cout<<"one valid intersection point."<<endl;
					if(intersectionPoints == 1)
					{
						if(_r[0] > r)
						{
							swap(r, s, t);
							
							
							intersectionPoints++;

						}
						else
						{
							_r[intersectionPoints] = r;
							_s[intersectionPoints] = s;
							_t[intersectionPoints] = t;
							intersectionPoints++;
						}
							
					}	
					else
					{
						_r[intersectionPoints] = r;
						_s[intersectionPoints] = s;
						_t[intersectionPoints] = t;
						intersectionPoints++;
					}	
					//return true;
				}	
			}	
		}	

	}
		
	if(intersectionPoints == 2)
		return true;
		//cout<<endl<<"Intersection points before triangle: "<<intersectionPoints;
		
	if(intersectionPoints == 1)
	{
		intersection = triangleIntersection();
		if(intersection == 1.0)
		{
			intersectionPoints++;
			return true;
		}	
	}
	
	return false;	
}*/

bool Bundle :: eval(int curFace, int width, int height)
{
	double r, s, t, intersection;
	int intersectionPoints = 0;
	bool oneIntersection;
	
	intersection = triangleIntersection();
	if(intersection == 1.0)
	{
		intersectionPoints++;		
		_colorVal++;
	}
	
	for(int i=0; i<3; i++)
	{
		if(intersectionPoints == 2)
			return true;
			
		t = findRoot(i);
				
		if(t >= 0.0 && t <= 1.0)
		{	
			s = evalS(t, i);
							
			if(s >= 0.0)
			{
				r = evalR(t, i);
				if(r >= 0.0)
				{
					saveRST(r, s, t);	
					intersectionPoints++;
					_colorVal += 2;					
				}	
			}	
		}

	}
	
	if(intersectionPoints == 2)
			return true;
	
	if(intersectionPoints == 1)
	{
		oneIntersection = calcCamTriangle();
		
		return true;
	}
	//if(width == 200 and height == 445 and curFace == 104)
	//	cout<<"Intersection points: "<<intersectionPoints<<endl;
	return false;	
}

double Bundle :: findRoot(int index)
{
	double tnMinus2 = 0.0, tnMinus1 = 1.0;
	
	double FtnMinus1;
	
	double FtnMinus2 = evalF(tnMinus2, index);
	
	

		
	for(int i=2; i<nBig; i++)
	{
		FtnMinus1 = evalF(tnMinus1, index);
		
		double t = ( tnMinus2 * FtnMinus1 - tnMinus1 * FtnMinus2 ) / (FtnMinus1 - FtnMinus2);
		
		
		double ft = evalF(t, index);
		
		if(ft < threshold )
		{	
			return t;
		}	
			
		tnMinus2 = tnMinus1;
		FtnMinus2 = FtnMinus1;
		tnMinus1 = t;					
	}	 
	
	return -1.0f;
}

int Bundle :: sign(double x)
{
	if (x > 0.0) return 1;
	if (x < 0.0) return -1;
	return 0;
}	

bool Bundle :: AreSame(double a, double b)
{
    return fabs(a - b) < EPSILON;
}

/*double Bundle :: findRoot(int index)
{
		
	double F0 = evalF(0.0, index);
	
	if (AreSame(F0, 0.0))
		return 0.0;
	
	double F1 = evalF(1.0, index);
		
	if (AreSame(F1, 0.0))
		return 1.0;
		
	if (F0 * F1 > 0.0)
		return -1.0;	
		
	double a = 0.0, b = 1.0;	
	
	int N = 0;
	
	while(N < nBig)
	{
		double c = (a + b) / 2.0;
		
		double fc = evalF(c, index);
		
		if (fc == 0.0)
			return c;
			
		if( ( (b - a)/2.0) < threshold)
			return c;
			
		N += 1;
		
		if(sign(c) == sign(a))
			a = c;
			
		else
			b = c;
	}	
	
	return -1.0f;
}*/

Vector Bundle :: calcY(double t, int index)
{
	int pIndex = index;
	int eIndex = index+1;
	
	if(eIndex > 2)
		eIndex = 0;
		
	Vector y = _vertices[pIndex] + t * _edges[eIndex];
	
	return y;
}	

Vector Bundle :: calcDT(double t, int index)
{
	if(_alpha[index] == 0.0)
		return _refractedRays[index];
		
	double cosAlphaT = cos(_alpha[index] * t);	
	
		
	Vector DT = _refractedRays[index] * cosAlphaT + _dotProduct[index]  * (1- cosAlphaT) - _crossProduct[index] * sin(_alpha[index] * t); // _dotProduct[i] = alphaHat * (alphaHat * _refractedRays[pIndex]) \
																																		_crossProduct[i] = (_refractedRays[pIndex] ^ alphaHat);
	
	if(DT.magnitude() != 0.0)
		DT.normalize();
	
	return DT;
}	

double Bundle :: evalR(double t, int index)
{
	Vector y = calcY(t, index);
	
	Vector DT = calcDT(t, index);
	
	double r = ( (_path * (y - _pos)) - ((_path * DT) * DT * (y - _pos)) ) / ( 1 - pow((_path * DT), 2) );
	
	return r;
}	

double Bundle :: evalS(double t, int index)
{
	Vector y = calcY(t, index);
	
	Vector DT = calcDT(t, index);
	
	double s = ( ( (_path * DT) * _path * (y - _pos) ) - ( DT * (y - _pos) ) ) / ( 1 - pow((_path * DT), 2) );
	
	return s;
}	

/*double Bundle :: evalF(double t, int index)
{
	Vector y = calcY(t, index);
	
	Vector DT = calcDT(t, index);
	
	double r = evalR(t, index);
	
	double s = evalS(t, index);
	
	
	Vector tmp = _pos + (_path * r) - y - (DT * s) ;
	
	double f = tmp * tmp;
	
	//double f = pow(tmp.magnitude(), 2);

	
	return f;
}*/

double Bundle :: evalF(double t, int index)
{
	Vector y = calcY(t, index);
	
	Vector DT = calcDT(t, index);
	
	double f = (y - _pos) * (_path ^ DT) ;
	
	return f;
}

double Bundle :: SafeAcos (double x)
{
	if (x < -1.0) x = -1.0 ;
	else if (x > 1.0) x = 1.0 ;
	return acos (x) ;
}
  
  
void Bundle :: CalcAlphaHat()
{
	for(int i = 0; i<3; i++)
	{
		int j = i + 1;
		if(j > 2)
			j = 0;
			
		_alphaHat[i] = _refractedRays[i] ^ _refractedRays[j];
		
		if(_alphaHat[i].magnitude() != 0.0)
			_alphaHat[i].normalize();	
			
		double dotP = _refractedRays[i] * _refractedRays[j];
		
		_alpha[i] = SafeAcos(dotP);	
		
		_dotProduct[i] = _alphaHat[i] * (_alphaHat[i] * _refractedRays[i]);
		
		_crossProduct[i] = (_refractedRays[i] ^ _alphaHat[i]);
	}		
	
	
}

	

double Bundle :: triangleIntersection()
{
	//Begin calculating determinant - also used to calculate u parameter
	Vector p = _path ^ -1.0 * _edges[0];
	//if determinant is near zero, ray lies in plane of triangle
	double det = _edges[1] * p;
	//NOT CULLING
	if(det > -EPSILON && det < EPSILON) return -1.0;
	double inv_det = 1.0 / det;
 
	//calculate distance from V0 to ray origin
	Vector T = _pos - _vertices[0];
 
	//Calculate u parameter and test bound
	double t = (T * p) * inv_det;                        //u coordinate of the intersection point
	//The intersection lies outside of the triangle
	if(t < 0.0 || t > 1.0) return -1.0;
 
	//Prepare to test v parameter
	Vector Q = T ^ _edges[1];
 
	//Calculate V parameter and test bound
	double s = (_path * Q) * inv_det;                         //v coordinate of the intersection point
	//The intersection lies outside of the triangle
	if(s < 0.0 || t + s  > 1.0) return -1.0;
 
	double r = (-1.0 * _edges[0] * Q) * inv_det;  //Distance from the cam position to intersection point
 
	if(r > EPSILON) 
	{ //ray intersection
		
		saveRST(r, 0.0, t);
		return 1.0f;
	}
 
  // No hit, no win
  return -1.f;
}			

bool Bundle :: calcCamTriangle() //This function calculates s where there is the camera and it has 3 cases
{
	double u, v, m, s, r;
	Vector y;
	
	double v_numerator =  (_pos - _vertices[0]) * (_edges[2] - ((_edges[1]*(_edges[1]*_edges[2])) / (_edges[1]*_edges[1])) );
	double v_denominator = (_edges[2]*_edges[2]) - ((pow((_edges[1]*_edges[2]),2)) / (_edges[1]*_edges[1]));
	
	v = v_numerator/v_denominator;
	
	if(v >= 0.0 && v <= 1.0)
	{
		u = ( ( (_pos - _vertices[0]) * _edges[1]) / (_edges[1]*_edges[1]) ) - ( v * ((_edges[1]*_edges[2]) / (_edges[1]*_edges[1])) );
		
		if (u >= 0.0 && u <= 1.0)
		{
			double uPlusv = u + v;
			
			if (uPlusv >= 0.0 && uPlusv <= 1.0)
			{
				 y = _pos - _vertices[0] - u * _edges[1] - v * _edges[2];
				 
				 r = 0.0;
				 if(_r[0] > _nearPlane)
					r = _nearPlane;
				saveRST(0.0, y.magnitude(), 0.0);
				
				return true;
			}	
		}	
		
	}
	
	int count = 0;
	double abs_y[3];
	
	for(int i=0; i<3; i++)
	{
		m = 0.0;
		m = ((_pos - _vertices[i]) * _edges[i]) / (_edges[i]*_edges[i]);
		
		if(m >= 0.0 && m <= 1.0)
		{
			y = _pos - _vertices[i] - m * _edges[i];
			abs_y[count] = y.magnitude();
			count++;									
		}	
	}
	
	if(count > 0)
	{
		
		if(count == 1)
			s = abs_y[0];
		if(count == 2)
			s = min(abs_y[0], abs_y[1]);
		if(count == 3)
			s = min(abs_y[0], min(abs_y[1], abs_y[2]));	
		 r = 0.0;
		 if(_r[0] > _nearPlane)
			r = _nearPlane;
		saveRST(0.0, s, 0.0);
				
		return true;
	}	
	
	double d[3];
	
	Vector dist;
	for(int i=0; i<3; i++)
	{
		dist = (_vertices[i] - _pos);	
		d[i] = dist.magnitude();
	}
	
	s = min(d[0], min(d[1], d[2]));
	
	r = 0.0;
	if(_r[0] > _nearPlane)
		r = _nearPlane;
		
	saveRST(0.0, s, 0.0);
	
	return true;	
		
}
	
void Bundle :: calcTopTriArea()
{
	Vector temp = _edges[1] ^ _edges[2] ;
	_topTriArea = fabs(temp.magnitude()) / 2.0;
	
}	

void Bundle :: calcBottomTriArea(double s, int index)
{
	
	Vector e1 = _edges[1] + s * ( _refractedRays[1] - _refractedRays[0] );
	Vector e2 = _edges[2] + s * ( _refractedRays[2] - _refractedRays[1] );
		
	Vector temp = e1 ^ e2;
		
	_bottomTriArea = fabs(temp.magnitude()) / 2.0;
		
	_dividedArea = double(_topTriArea / _bottomTriArea);
		
	_topArea[index] = _topTriArea;
	_bottomArea[index] = _bottomTriArea;
	_divArea[index] = _dividedArea;	
}

double Bundle :: calcColorComp(int colorIndex, double s)
{
	double sunColorComp, colorComponent;
	
	sunColorComp = _sun[colorIndex];
		
	colorComponent = sunColorComp * _dividedArea;

	return colorComponent;
}

double Bundle :: getFinalColor(int colorIndex, int width, int height, int curFace)
{
	double L1Comp, L2Comp, colorComponent, bComponent;
	
	L1Comp = _L1[colorIndex];
	L2Comp = _L2[colorIndex];  
	bComponent = _b[colorIndex];
	

	double val = bComponent * (_r[1] - _r[0] + _s[1] - _s[0]);
	
	double value = fabs(val);

	if(value < threshold)
		colorComponent = bComponent * (_r[1] - _r[0]) * exp(-1.0 * bComponent *(_r[0] + _s[0]) ) * L1Comp;
		
	else
	{
		double part1 = ((_r[1]-_r[0]) / (_r[1]-_r[0] + _s[1]-_s[0])) * exp(-1.0 * bComponent * (_r[0] + _s[0]));
		
		double part2a = ( L1Comp * (1.0 - exp(-1.0 * val)) );
		
		double part2b = ( (L2Comp - L1Comp) * exp(-1.0 * val) );
		
		double part2c = ( ((L2Comp - L1Comp)/val) * ( 1.0 - exp(-1.0 * val)) ) ;
		
		double part2 = part2a - part2b + part2c;
		
		colorComponent = part1 * part2;
		
	}	
	return colorComponent;
}	
