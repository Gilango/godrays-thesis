#include "objReader.h"
#include "Vector.h"

using namespace gRays;

void ObjReader::calculateEdges()
{
	for(int i=0; i<facesnum; i++)
	{
		Vector v0 = vertices[faces[i][0][0]];
		Vector v1 = vertices[faces[i][1][0]];
		Vector v2 = vertices[faces[i][2][0]];
		edges[i][1] = v1 - v0;
		edges[i][2] = v2 - v1;
		edges[i][0] = v0 - v2;		
	}	 
}	

void ObjReader::getVertices(int index, Vector localvertices[])
{
	
	localvertices[0] = getVertex(faces[index][0][0]);
	localvertices[1] = getVertex(faces[index][1][0]);
	localvertices[2] = getVertex(faces[index][2][0]);
	
	//return localvertices;
	
}

void ObjReader::getNormals(int index, Vector localnormals[])
{
	localnormals[0] = getNormal(faces[index][0][1]);
	localnormals[1] = getNormal(faces[index][1][1]);
	localnormals[2] = getNormal(faces[index][2][1]);
	
	//return localnormals;
	
}

void ObjReader::getVertexIndices(int index, int vIndices[])
{
	vIndices[0] = getVertexIndex(index, 0);
	vIndices[1] = getVertexIndex(index, 1);
	vIndices[2] = getVertexIndex(index, 2);
	
	//return vIndices;
}	

void ObjReader::getNormalIndices(int index, int nIndices[])
{
	nIndices[0] = getNormalIndex(index, 0);
	nIndices[1] = getNormalIndex(index, 1);
	nIndices[2] = getNormalIndex(index, 2);
	
	//return nIndices;
}
	
bool ObjReader::read(const string filename)
{
	FILE *fp;
	fpos_t position;
	char line[500], *word;
	int i,j,k=0;
	const char* foo = filename.c_str();
	fp = fopen(foo, "r");
		
	if(fp == NULL)
	{
		cout<<"\nError opening the file.\n";
		return false;
	}
	cout<<"\nFile successfully opened\nProcessing the file...\n";
	
	fgetpos (fp, &position);
	
	
	while(fgets(line, sizeof(line), fp) != NULL)
	{
		word = strtok(line, " ");

		if(strcmp(word, "v") == 0)
		{
			vertexnum++;
		}
		else if(strcmp(word, "vn") == 0)
		{
			normalnum++;
		}
		else if(strcmp(word, "vt") == 0)
		{
			tcoordnum++;
		}
		else if(strcmp(word, "f") == 0)
		{
			facesnum++;
		}
					
	}
	edgesnum = facesnum;
	
	vertices = new Vector[vertexnum+1];
	normals = new Vector[normalnum+1];
	//tcoord = new Vector2[tcoordnum+1];
	faces = new int**[facesnum];
	edges = new Vector*[edgesnum];
	
	for(i=0;i<facesnum;i++)
	{
		faces[i] = new int*[3];
		for(j=0;j<3;j++)
		{
			faces[i][j] = new  int[2];
		}
	}
	
	for(i=0; i<edgesnum; i++)
	{
		edges[i] = new Vector[3];	
	}	

	fsetpos (fp, &position);
		
	
	while(fgets(line, sizeof(line), fp) != NULL)
	{
			
		if(line[0] != '#') 
		{		
			word = strtok(line," ");
			while(word != NULL)
			{
				if(strcmp(word, "v") == 0)
				{
					word = strtok(NULL, " ");
					vertices[vcount][0] = atof(word);
					word = strtok(NULL, " ");
					vertices[vcount][1] = atof(word);
					word = strtok(NULL, " ");
					vertices[vcount][2] = atof(word);
					vcount++;
				}
				if(strcmp(word, "vt") == 0)
				{
					word = strtok(NULL," ");
				//	tcoord[texcount][0] = atof(word);
					word = strtok(NULL," ");
				//	tcoord[texcount][1] = atof(word);
					texcount++;
				}
				if(strcmp(word, "vn") == 0)
				{
					word = strtok(NULL, " ");
					normals[ncount][0] = atof(word);
					word = strtok(NULL, " ");
					normals[ncount][1] = atof(word);
					word = strtok(NULL, " ");
					normals[ncount][2] = atof(word);
					ncount++;
				}
				if(strcmp(word, "f") == 0)
				{
					for(k=0;k<3;k++)
					{
					word = strtok(NULL, " /");
					faces[fcount][k][0] = atoi(word);
					if(tcoordnum != 0)
					{
						word = strtok(NULL, " /");
					//	faces[fcount][k][1] = atoi(word);
					}
					word = strtok(NULL, " /");
					faces[fcount][k][1] = atoi(word);
					}
					fcount++;
				}
			word = strtok(NULL, " ");
			}
		}
	}
	
	calculateEdges();
	fclose(fp);
	return true;	

}	
