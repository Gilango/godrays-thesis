#ifndef __BUNDLE_H_
#define __BUNDLE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <math.h>
#include "Vector.h"
#include "Color.h"

#define EPSILON 0.000001

using namespace std;

namespace gRays
{

	class Bundle
	{
		public:
		
			Bundle(int count, double thres, double nPlane):nBig(count), threshold(thres), _nearPlane(nPlane)
			{}
		
			void storeVertices(Vector* vertices){ _vertices = vertices; }
			void storeNormals(Vector* normals){ _normals = normals; }
			void storeEdges(Vector* edges){ _edges = edges; }
			void storeRRays(Vector* rays){ _refractedRays = rays; }
			void setCamPos(Vector pos){ _pos = pos; }
			void setPixelRay(Vector pixelPath){ _path = pixelPath; }
			void CalcAlphaHat(void);
			void setSunColor(Color sColor){ _sun = sColor;}
			void setB(Vector val){ _b = val;}
			void calcTopTriArea(void);
			void calcBottomTriArea(double, int);
			double calcColorComp(int, double);
			double getFinalColor(int,int,int,int);
			void saveL1andL2(Color L1, Color L2){_L1 = L1; _L2 = L2;}
			Color getL1(){return _L1;}
			Color getL2(){return _L2;}
			void swap(double, double, double);
			void saveRST(double, double, double);
			int getIntersectionCase(){return _colorVal;}
			double getBottomTriArea(){return _bottomTriArea;}
			double getdividedArea(int i){return _divArea[i];}
			double getTopArea(int i){return _topArea[i];}
			double getBottomArea(int i){return _bottomArea[i];}
			
			void resetValues()
			{ 
				_iVal = 0;
				_colorVal = 0;
				for(int i=0; i<2; i++)
					_r[i] = _s[i] = _t[i] = 0.0f;
			}
			
			double getR(int i)
			{	
				if(_r[i] > EPSILON)
					return _r[i]; 
				else
					return 0.0;
			}
			double getS(int i)
			{
				 if(_s[i] > EPSILON)
					return _s[i]; 
				else
					return 0.0;
			}
			double getT(int i)
			{
				 if(_t[i] > EPSILON)
					return _t[i]; 
				
				else
					return 0.0;	
			}
			
			double getAlpha()
			{
				return 1.0;	
			}	
			
			bool eval(int, int, int);
			double findRoot(int);
			Vector calcY(double, int);
			Vector calcDT(double, int);
			double evalR(double, int);
			double evalS(double, int);
			double evalF(double, int);
			int sign(double);
			bool AreSame(double, double);
			void print();
			double triangleIntersection(void);
			double SafeAcos(double);
			bool calcCamTriangle(void);
			
		/*	~Bundle()
			{
				delete[] _vertices; 
				delete[] _normals;  
				delete[] _edges;   
				delete[] _refractedRays; 
			}	*/
				
		private:
					
			Vector* _vertices; 
			Vector* _normals;  
			Vector* _edges;   
			Vector* _refractedRays; 	
			Vector _pos, _path, _alphaHat[3], _crossProduct[3], _dotProduct[3], _b;
			double _r[2], _s[2], _t[2], _alpha[3];  //This alpha is the angle between 2 vectors
			int nBig, _iVal, _colorVal;
			double threshold, _topTriArea, _bottomTriArea, _dividedArea, _nearPlane;
			double _divArea[2], _topArea[2], _bottomArea[2];
			double alpha;  //this alpha is for the image.
			Color _sun, _L1, _L2;
	};		

}
#endif
