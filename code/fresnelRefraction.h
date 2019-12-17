#ifndef __FRESNEL_REFRACTION_H_
#define __FRESNEL_REFRACTION_H_

#include "Vector.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

namespace gRays
{
	class Fresnel
	{
		public:
				Fresnel(float _ior):
					ior(_ior)
					{
						//rays = new Vector[3];
					}
					
				Vector evalRefraction(Vector incidentRay, Vector normal, Vector hitPosition);	
				
				float SafeAcos(float);
				
				void createRays(int size){refractedRays = new Vector[size];}
				
				void saveRay(Vector refractedRay, int vertexIndex)
				{
					refractedRays[vertexIndex] = refractedRay;
					
				}
				
				void getRefractedRays(int* vertexIndices, Vector rays[]);
				
		~Fresnel()
		{
			delete[] refractedRays;
			//delete[] rays;
		}				
					
		private:
				float ior; //Index of refraction	
				Vector *refractedRays;//, *rays;	
	};	

}
#endif
