#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <mutex>
#include "objReader.h"
#include "fresnelRefraction.h"
#include "Camera.h"
#include "CmdLineFind.h"
#include "Bundle.h"
#include "Color.h"
#include "OIIOFiles.h"
#include "ProgressMeter.h"
#include "omp.h"
#include "DeepImage.h"

#define DBL_MIN         2.2250738585072014e-308

// modified normal value 0.28730 0.95783 0.000000
std::mutex mtx;
using namespace std;
using namespace gRays;

int main(int argc, char** argv)
{
	CmdLineFind clf( argc, argv );
	ObjReader obj;
	
	double threshold = clf.find("-threshold", 0.00001, "Threshold value");
	int nBig = clf.find("-nbig", 20, "nBig value");
	//Bundle bundle(nBig, threshold);  //nBig and threshold
	Camera cam;
	float ior = clf.find("-ior", 1.33f, "Index of refraction for water");
	int imageWidth = clf.find("-NX", 1920, "Image width");
	int imageHeight = clf.find("-NY", 817, "Image height");
	//Vector eyePos = clf.find("-eye" , Vector(30.003, -8.747, 0.548), "Camera position.");
	Vector eyePos = clf.find("-eye" , Vector(0.43405311533120816, -10.097053768473206, 7.3869697418154168), "Camera position.");
	//Vector eyeAim = clf.find("-aim" , Vector(6.244, -4.214, 0.216), "Camera aim.");
	Vector eyeAim = clf.find("-aim" , Vector(0.43405311533120816, -1.748531168720719, 0.26381023005465476), "Camera aim.");
	//Vector up = clf.find("-up", Vector(0.0, 1.0, 0.0), "camera up vector.");
	Vector up = clf.find("-up", Vector(0.0, 0.6490711242259618, 0.76072772770291874), "camera up vector.");
	cout<<"cam pos: "<<eyePos.X()<<" "<<eyePos.Y()<<" "<<eyePos.Z()<<endl;
	cout<<"cam aim: "<<eyeAim.X()<<" "<<eyeAim.Y()<<" "<<eyeAim.Z()<<endl;
	//Vector view = Vector(6.244, -4.214, 0.216) - Vector(30.003, -8.747, 0.548);
	Vector view = eyeAim - eyePos;
	//cam.setEyeViewUp(Vector(30.003, -8.747, 0.548), view, Vector(0.0, 1.0, 0.0));
	cam.setEyeViewUp(eyePos, view, up);
	cam.setFov(48.46);
	cam.setAspectRatio((imageWidth*1.0)/imageHeight);
	Vector lightPos = clf.find("-lpos" , Vector(0.0, 5.5, -5.9), "Light position.");
	Vector lightAim = clf.find("-laim" , Vector(0.0, 0.0, 0.0), "Light aim.");
	Vector lightDist = lightAim - lightPos;
	Vector iRay = lightAim - lightPos;
	//cam.setAspectRatio();
	
	
	string objName = clf.find("-objf", "oceanSurface.0001.obj", "OBJ mesh for the input triangles");
	//string objName = clf.find("-objf", "triangle.0003.obj", "OBJ mesh for the input triangles");
	string objPath = clf.find("-objpath", "/DPA/moosefs/PeanutButterJelly/prod/share/assets/oceanSurfaceObj/0019/256x256/obj/", "OBJ mesh path for the input triangles");
	//string objPath = clf.find("-objpath", "/home/gilango/Thesis/svnCode/", "OBJ mesh path for the input triangles");
	//string objPath = clf.find("-objpath", "/home/gilango/projects/godrays/objFiles/", "OBJ mesh path for the input triangles");
	objName = objPath + objName;
	
	//cout<<objName<<endl;
	
	//string imageFile = clf.find("-name", "image.exr", "Name of output image file");
	int currentFrame = clf.find("-CF", 1, "Current frame");
	int totalFrames = clf.find("-TF", 120, "Total frames");
	int turnTable = clf.find("-turn", 0, "Tutn table on");
	int startFrame = clf.find("-SF", 1, "Start frame");
	int endFrame = clf.find("-EF", 1, "End frame");
	int renderMultipleFrames = clf.find("-RMF", 0, "Render multiple frames");
	float radius = clf.find("-r", lightDist.magnitude(), "Light radius");
	int interval = clf.find("-interval", 0, "Range to render the deep images");
	int deepImage = clf.find("-deep", 1, "Deep image or 2D image");
	float nearPlane = clf.find("-nplane", 457.2, "Value for near plane");
	float farPlane = clf.find("-fplane", 6096.0, "Value for far plane");
	int face = clf.find("-face", 90, "Triangle face value");
	
	int divide = clf.find("-divide", 0, "To split the deep image");	
	int divisor;	
	
	if(divide == 0)
		divisor = 1;
	else
		divisor = 10;
		
	Vector tmpRGB = Vector(1.0, 1.0, 1.0);
	//Vector bVal(0.00912, 0.00088, 0.000419); // Diffuse attentuation coefficient
	Vector bVal(0.0758, 0.02197, 0.0025); // Diffuse attentuation coefficient
	Vector sunRGB = clf.find("-rgb" , tmpRGB, "RGB values for the sun.");
	Vector b = clf.find("-b", bVal, "B value for attenuation.");
	Color sunColor(sunRGB.X(), sunRGB.Y(), sunRGB.Z(), 1.0); 
	//Color sunColor(100000.0, 100000.0, 100000.0, 1.0); 
	string imageBaseName = clf.find("-name", "gilango_godrays_deepImage", "Name of output image file");
	//string filePath = clf.find("-path", "/DPA/moosefs/PeanutButterJelly/prod/share/assets/godrays/0006/1920x817/exr/", "folder path");
	//string filePath = clf.find("-path", "/newscratch/gilango/godraysDeepImage/0001/frames/", "folder path");
	string filePath = clf.find("-path", "/home/gilango/Thesis/testRenders/", "folder path");
	string imageFile, padframe;
		
	
	Fresnel fresnel(ior);
	
	
	bool a = obj.read(objName);
	int vertexIndex, normalIndex;
	int verticesCount = obj.vertexCount();
	cout<<"Vertices: "<<verticesCount<<endl;
	cout<<"Normals: "<<obj.normalCount()<<endl;
	int totalFaces = obj.facesCount();
	Vector refractedRay;
	fresnel.createRays(verticesCount+1);
	
	Vector camPos = cam.eye();
	
	//cout<<endl<<"Incident light ray: "<<iRay;
	
	int loopCount = 1;
	if(turnTable == 1)
		loopCount = totalFrames-1;
	
	if(renderMultipleFrames == 1)
		loopCount = endFrame - startFrame + 1;		
	
	for( int tt=1; tt<=loopCount;	tt++)
	{
		if(turnTable == 1)
			currentFrame = tt;
			
		if(renderMultipleFrames == 1)
			currentFrame = startFrame + tt - 1;		
		
		//float radian = (float(currentFrame)/float(totalFrames)) * M_PI;
		//Vector lightPos = Vector(0.0, radius * sin(radian), radius * cos(radian));
		//cout<<"Light pos: "<<lightPos;
		//iRay = lightAim - lightPos;
		iRay.normalize();
		//camera.setEyeViewUp( camPos, camView, Vector(0,1,0) );
	
		ProgressMeter meter( imageHeight, "God rays" );
		
		stringstream ss;
		ss << currentFrame;
		padframe = ss.str();
		
		if (currentFrame < 1000)
			padframe = "0" + padframe;
		if (currentFrame < 100)
			padframe = "0" + padframe;
		if (currentFrame < 10)
			padframe = "0" + padframe;	
		padframe = "." + padframe;
		
		imageFile = filePath + imageBaseName + padframe + ".exr";
		
		//imageFile = "/home/gilango/Thesis/testRenders/" + imageBaseName + padframe + ".exr";
		
		
		for(int f=0; f<totalFaces; f++)   //This loop is to save the refracted rays for every vertices in the obj file with respect to the light ray
		{
			for(int pos=0; pos<3; pos++)
			{
				vertexIndex = obj.getVertexIndex(f, pos);
				normalIndex = obj.getNormalIndex(f, pos);
				refractedRay = fresnel.evalRefraction(iRay, obj.getNormal(normalIndex), obj.getVertex(vertexIndex));
				//cout<<endl<<refractedRay;
				fresnel.saveRay(refractedRay, vertexIndex);
			}	
			
		}	
		
		
		Image image;
		image.reset( imageWidth, imageHeight );	
		
		DeepImage deepimage;
		deepimage.reset(imageWidth, imageHeight);
		double depth_front = 0.0, depth_back = 0.0;	
		//omp_set_num_threads(8);
		#pragma omp parallel for
		for(int j=0; j<imageHeight; j++)
		{									
			
			for(int i=0; i<imageWidth; i++)
			{
					depth_front = 0.0;
					depth_back = 0.0;
					DeepPixelBuffer buffer;
					
					double red = 0.0, green = 0.0, blue = 0.0, alpha = 0.0;
					int count = 0;
					
					for(int curFace=0; curFace<totalFaces; curFace++)
					//for(int curFace=45000; curFace<53000; curFace++)
					{
						
						Bundle bundle(nBig, threshold, nearPlane);
						bundle.setSunColor(sunColor);
						bundle.setB(b);
						bundle.setCamPos(camPos);
						Vector local_verts[3];
						Vector local_norms[3];
						int local_vs[3];
						int local_ns[3];
						obj.getVertices(curFace, local_verts);
						obj.getNormals(curFace, local_norms);
						bundle.storeVertices(local_verts);
						bundle.storeNormals(local_norms);
						bundle.storeEdges(obj.getAllEdges(curFace));
						obj.getVertexIndices(curFace, local_vs); 
						
						Vector refractedRays[3];
						fresnel.getRefractedRays(local_vs, refractedRays); 
						bundle.storeRRays(refractedRays);
						bundle.CalcAlphaHat();
						//bundle.print();
						Vector pixelRay = cam.view( double(i)/imageWidth, double(j)/imageHeight );
						pixelRay.normalize();
						bundle.setPixelRay(pixelRay);
						bundle.resetValues(); //set r, s, t and _iVal values to 0.0f
						bool intersection = bundle.eval(curFace, i, j);
						
						if(curFace % divisor == interval)
						{
							if(intersection)
							{
								DeepPixel deeppixel1, deeppixel2;
								//cout<<"Intersection"<<endl;
								double s = bundle.getS(0);
								bundle.calcTopTriArea();
								bundle.calcBottomTriArea(s, 0);
								float redVal = bundle.calcColorComp(0, s);
								float greenVal = bundle.calcColorComp(1, s);
								float blueVal = bundle.calcColorComp(2, s);
								Color L1, L2;
						//		cout<<"L1 red value: "<<redVal<<endl;
								L1.set(redVal, greenVal, blueVal, 1.0);
						//		cout<<"L1: ("<<L1.X()<<", "<<L1.Y()<<", "<<L1.Z()<<endl;
								
								s = bundle.getS(1);
								
								if(s == 0.0)
								{
									//cout<<"Triangle intersection"<<endl;
									L2.set(sunColor);
								}	
									//L2.set(sunColor);
								else
								{	
									bundle.calcBottomTriArea(s, 1);
									double redVal = bundle.calcColorComp(0, s);
									double greenVal = bundle.calcColorComp(1, s);
									double blueVal = bundle.calcColorComp(2, s);
						//			cout<<"L2 red value: "<<redVal<<endl;
									L2.set(redVal, greenVal, blueVal, 1.0);
								}
								/*if(curFace == 90 and i == 982 and j == 775)
									cout<<"L2: "<<L2.X()<<", "<<L2.Y()<<", "<<L2.Z()<<endl;*/
								
								bundle.saveL1andL2(L1, L2);
								
								double r = bundle.getR(0); 
								
								if(deepImage == 0 and r >= nearPlane and r <= farPlane)// and curFace == face)
								{	
									/*int val = bundle.getIntersectionCase();
									if(i == 33 and j == 459)
										cout<<"intersection val: "<<val<<endl;
									double r, g, b;
									if(val == 1) //Only one intersection point on the triangle
									{
										r = 1.0;
										g = b = 0.0;
									}	
									if(val == 2) //only one intersection point on the light shaft
									{
										g = 1.0;
										r = b = 0.0;
									}
									if(val == 3) //One intersection point on the triangle, one on the shaft
									{
										/*cout<<"L1: "<<L1.X()<<", "<<L1.Y()<<", "<<L1.Z()<<endl;
										cout<<"L2: "<<L2.X()<<", "<<L2.Y()<<", "<<L2.Z()<<endl;
										cout<<"s0: "<<bundle.getS(0)<<endl;
										cout<<"s1: "<<bundle.getS(1)<<endl;
										cout<<"Top area for 1st pt: "<<bundle.getTopArea(0)<<endl;
										cout<<"Bottom area for 1st pt: "<<bundle.getBottomArea(0)<<endl;
										cout<<"Top area for 2nd pt: "<<bundle.getTopArea(1)<<endl;
										cout<<"Bottom area for 2nd pt: "<<bundle.getBottomArea(1)<<endl;
										cout<<"Refracted rays 0: "<<refractedRays[0]<<endl;
										cout<<"Refracted rays 1: "<<refractedRays[1]<<endl;
										cout<<"Refracted rays 2: "<<refractedRays[2]<<endl;
										b = 1.0;
										r = g = 0.0;
									}
									if(val == 4) //Two intersection points on the shaft
									{
										//cout<<"It's working"<<endl;
										b = g = 1.0;
										r = 0.0;
									}
									/*if(i == 200 and j == 445)
									{
										cout<<"Red: "<<r<<" Green: "<<g<<" Blue: "<<b<<endl;
										cout<<"intersection val: "<<val<<endl;
									}
									image.value(i,j,0) = r;
									image.value(i,j,1) = g;
									image.value(i,j,2) = b;
									image.value(i,j,3) = 1.0;*/
									
									image.value(i,j,0) += bundle.getFinalColor(0, i, j, curFace);
									image.value(i,j,1) += bundle.getFinalColor(1, i, j, curFace);
									image.value(i,j,2) += bundle.getFinalColor(2, i, j, curFace);
									image.value(i,j,3) = 1.0;
								}
								
								if(deepImage == 1 and r >= nearPlane and r <= farPlane)
								{
									red = bundle.getFinalColor(0, i, j, curFace);
									green = bundle.getFinalColor(1, i, j, curFace);
									blue = bundle.getFinalColor(2, i, j, curFace);
									alpha = 0.0001;
									
									deeppixel1.color.set(red, green, blue, alpha);
									deeppixel1.depth_front =  bundle.getR(0);
									deeppixel1.depth_back = bundle.getR(1);
									buffer.push_back(deeppixel1);
								}	
								
								
							}
							
						}
						
					}
				if(deepImage == 1)
				{	
					buffer.sort(Tidy());
					setPixelDeep(deepimage,i,j,buffer);	
				}	
			}
			meter.update();			
			
			
		}
		
		string tmp = meter.totalTimeTaken();
			
		string totalTime = clf.find("-time", tmp, "Total time taken");
		
		if(deepImage == 0)
			writeOIIOImage( imageFile.c_str(), image, clf.mapFinds() );	
			
		if(deepImage == 1)
			writeOIIOImage(imageFile.c_str(), deepimage);

	
	}	
	
	
		
	
}
