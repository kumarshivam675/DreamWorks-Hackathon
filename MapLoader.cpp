#include "MapLoader.h"
#include "Vector.h"
#include <cstring>
#include <openvdb/openvdb.h>
#include<stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

/************************ This loads the map from openVDB.
                          You can use the native viewer once you complete this.
                          There are comments for each snippet of code.
                          Hint: http://www.openvdb.org/documentation/doxygen/codeExamples.html **************/



bool**
MapLoader::loadVDBMap(char* filename)
{
	
    //Initialize the OpenVDB library.
	openvdb::initialize();
    // Create a VDB file object.
	
	openvdb::io::File file(filename);
	cout << filename << endl;
    // Open the file. This reads the file header, but not any grids.
	file.open();
    //Loop over all grids in the file and retrieve a shared pointer to "SDF". Close file
	openvdb::GridBase::Ptr baseGrid;
	for (openvdb::io::File::NameIterator nameIter = file.beginName();
    	nameIter != file.endName(); ++nameIter)
	{
    	// Read in only the grid we are interested in.
    	if (nameIter.gridName() == "SDF") {
        	baseGrid = file.readGrid(nameIter.gridName());
    	} else {
        	std::cout << "skipping grid " << nameIter.gridName() << std::endl;
    		}
	}
	file.close();

    //cast the generic grid pointer to a FloatGrid pointer.
	openvdb::FloatGrid::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);
    //Get metadata into strings, see .meta file 
    //Convert strings to numbers
	y_boundary = stoi(grid->metaValue<std::string>("boundy"));
	x_boundary = stoi(grid->metaValue<std::string>("boundx"));
	startRadius = stof(grid->metaValue<std::string>("start_radius"));
	endRadius = stof(grid->metaValue<std::string>("end_radius"));
	 
	float startx = stof(grid->metaValue<std::string>("startx"));
	float starty = stof(grid->metaValue<std::string>("starty"));
	float endx = stof(grid->metaValue<std::string>("endx"));
	float endy = stof(grid->metaValue<std::string>("endy"));
	//std::cout<<endx<<" "<<endy<<endl;
	posStart = Vec2f(startx,starty);
	posEnd = Vec2f(endx,endy);

    // Create the map data - 2D map and SDF arrays. 
    //Allocate memory
	grid_array = new float*[x_boundary];
	for (int i=0; i<x_boundary; i++)
		grid_array[i] = new float[y_boundary];

	bool ** mapdata;
	mapdata = new bool*[x_boundary];
	for (int i=0; i<x_boundary; i++)
	{
		mapdata[i] = new bool[y_boundary];
		for (int j=0; j<y_boundary; j++)
			mapdata[i][j] = false;
	}
		
	/*for (openvdb::FloatGrid::ValueOnCIter iter = grid->cbeginValueOn(); iter; ++iter) 
	{
		if (iter.getCoord()[2] == 0)
		{
			grid_array[iter.getCoord()[0]][iter.getCoord()[1]] = *iter;
			mapdata[iter.getCoord()[0]][iter.getCoord()[1]] = true;
		}
	}*/

	openvdb::FloatGrid::Accessor accessor = grid->getAccessor();

    for(int i=0;i<x_boundary;i++)
    {
        for(int j=0;j<y_boundary;j++)
        {
		
            openvdb::Coord temp(i, j, 0);
		//cout<<accessor.getValue(temp)<<endl;
		grid_array[i][j]=(float)accessor.getValue(temp);
            if(accessor.getValue(temp)<0)
			{				
				mapdata[i][j]=true;
				
			}
            else
                mapdata[i][j]=false;
			//cout<< mapData[i][j] << "," ;
        }
    }
    //Get an accessor for coordinate-based access to voxels.
    	
    /* Iterate through SDF array using the accessor to get SDF values. 
       Add true or false to map data according to the SDF value. */


    //Get values of Vectors posStart and posEnd

    //Change "loaded" flag
	loaded = true;
    //Return map data array
	
	return mapdata;
}

float**
MapLoader::getSDF(){
	return grid_array;
}

MapLoader::MapLoader() {

}

Vec2f
MapLoader::getStartPosition() {
	return posStart;
}

Vec2f
MapLoader::getEndPosition() {
	return posEnd;
}

unsigned int
MapLoader::gety_boundary() {
	return y_boundary;
}

unsigned int
MapLoader::getx_boundary() {
	return x_boundary;
}

unsigned int
MapLoader::getStartRadius() {
	return startRadius;
}

unsigned int
MapLoader::getEndRadius() {
	return endRadius;
}
