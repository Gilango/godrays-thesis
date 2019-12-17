#ifndef __OBJ_READER_H_
#define __OBJ_READER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <math.h>
#include "Vector.h"

using namespace std;

namespace gRays
{
	class ObjReader
	{
		public:
			ObjReader()
			{
				vcount=1;
				ncount=1;
				texcount=1;
				fcount=0;	
				vertexnum = normalnum = tcoordnum = facesnum = edgesnum = 0;
				localvertices = new Vector[3];
				localnormals = new Vector[3];
				vIndices = new int[3];
				nIndices = new int[3];
				
			}	

			bool read(const string filename);
			void calculateEdges(void);
			int getVertexIndex(int faceNumber, int vertexPos){return faces[faceNumber][vertexPos][0];} /*faceNumber is which line in the obj file and the vertexPos is which vertex in that triangle face. 
																											vertexPos = 0, 1, 2*/
			int getNormalIndex(int faceNumber, int vertexPos){return faces[faceNumber][vertexPos][1];}
			Vector getNormal(int index){return normals[index];}
			Vector getVertex(int index){return vertices[index];}
			Vector* getAllEdges(int faceIndex){return edges[faceIndex];}
			Vector getOneEdge(int faceIndex, int edgeIndex){return edges[faceIndex][edgeIndex];}
			
			void getVertices(int, Vector[]);
			
			void getNormals(int, Vector[]);
			
			void getVertexIndices(int, int[]);
			
			void getNormalIndices(int, int[]);
			
			int vertexCount(){return vertexnum;}
			int normalCount(){return normalnum;}
			int facesCount(){return facesnum;}
			int edgesCount(){return edgesnum;} //This has to be multiplied by 3. as it stores the 3 edges of a face in a single element. So, to loop through edges, use facesnum
			
			
		
			~ObjReader()
			{
				delete[] vertices;
				delete[] normals;
				delete[] localvertices;
				delete[] localnormals;
				delete[] vIndices;
				delete[] nIndices;
				
				for(int i = 0; i < edgesnum; i++)
					delete[] edges[i];
				delete[] edges;
							
				for(int i = 0; i < facesnum ; i++)
				{
					for(int j = 0; j < 3; j++)
					{
						delete[] faces[i][j];
					}
					delete[] faces[i];

				}
				delete[] faces;
			}
			
		private:
				int vertexnum, normalnum, tcoordnum, facesnum, edgesnum;
				int vcount, ncount, texcount, fcount;
				Vector *vertices, *normals, *localvertices, *localnormals;
				int *vIndices, *nIndices;
			//	Vector2 *tcoord;
				int ***faces;
				Vector **edges;
	};
	
	
	

}
#endif	

