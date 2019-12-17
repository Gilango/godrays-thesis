#include "Vector.h"
#include "fresnelRefraction.h"

using namespace std;
using namespace gRays;

float Fresnel :: SafeAcos (float x)
{
	if (x < -1.0) x = -1.0 ;
	else if (x > 1.0) x = 1.0 ;
	return acos (x) ;
}
  

Vector Fresnel :: evalRefraction(Vector iRay, Vector n, Vector hitPosition)
{
	Vector normal = n.unitvector();
	
	float cosThetaIncident = iRay * normal;
	//cout<<"cos :"<<cosThetaIncident<<endl;
	float thetaIncident = SafeAcos( fabs(cosThetaIncident) );
	//cout<<"Theta: "<<thetaIncident<<endl;
    float sinThetaTransmit = sin( thetaIncident );
	float ndexref = ior;
	
	if( cosThetaIncident  < 0 )
	{
		ndexref = 1.0/ior;
	}
	sinThetaTransmit *= ndexref;

	if( sinThetaTransmit < 1.0 )
	{
		   float thetaTransmit = asin( sinThetaTransmit );
		   float gammaRadical = 1.0 - ndexref*ndexref*pow((iRay^normal).magnitude(),2.0 );
		   float gamma = -ndexref * cosThetaIncident;
		   if( gamma < 0 )
		   { 
				 gamma += sqrt( gammaRadical ); 
		   }
		   else
		   {
				 gamma -= sqrt( gammaRadical );
		   }
		   Vector transmittedDir = iRay*ndexref + normal*gamma; 
		   if(transmittedDir.magnitude() != 0.0)
				transmittedDir.normalize();
			return transmittedDir; //Return refracted direction
			

	}
	else
	{
		   cout<<endl<<"Sin is > 1.0"<<endl;
		   cout<<"Normal: "<<n[0]<<" "<<n[1]<<" "<<n[2]<<endl;
		   return Vector(0.0, 0.0, 0.0);
	}	   
}

void Fresnel :: getRefractedRays(int* vIndices, Vector rays[])
{
	rays[0] = refractedRays[vIndices[0]];
	rays[1] = refractedRays[vIndices[1]];
	rays[2] = refractedRays[vIndices[2]];
	
	//return rays;
	
}	

