OFILES = Camera.o OIIOFiles.o Image.o objReader.o fresnelRefraction.o Bundle.o DeepImage.o

AFILES = $(OFILES) 

ROOTDIR = .
LIB = $(ROOTDIR)/libVR.a 

CXX = g++ -Wall -g -O2 -std=c++0x -D_THREAD_SAFE -pthread -fopenmp

LINKS =  -L/group/dpa/lib  -L/usr/local/lib -L$(HOME)/projects/OpenImageIO/dist/linux64/lib -lOpenImageIO -lHalf 

INCLUDES = -I /group/dpa/include  -I $(HOME)/projects/OpenImageIO/dist/linux64/include  

.C.o:
	$(CXX) -c $(INCLUDES) $<
	
all: $(AFILES)
	ar rv $(LIB) $?	

godrays: test.C
	$(CXX) test.C $(INCLUDES) $(LIB) $(LINKS) -o godrays
	
		
	
clean: 
	rm *.o *.a godrays
	
	
