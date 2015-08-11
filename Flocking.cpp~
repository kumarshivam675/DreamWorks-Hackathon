#include "Flocking.h"

/* Define the functions below */

int Flocking::update()
{
	//std::cout << "Flocking wala update\n";
    for(int i = 0; i < boids.size(); i++){
    	boids[i].update(boids);
    }
}
/*void Flocking::addBoid()
{
}*/

void Flocking::addBoid(int x, int y)
{
	
	if(sceneMap==NULL)
		cout<<"Simulation null"<<endl;	
	for(int i = 0; i < 10; i++){
	   	Boid b(x+ 2*i, y + i, x_bound, y_bound, boundaryPadding,maxSpeed, maxForce, flockSepWeight,
	   			flockAliWeight, flockCohWeight, flockSepRadius, flockAliRadius, flockCohRadius,sceneMap);
	   	boids.push_back(b);
		
	}
}

void Flocking::removeBoid(int x, int y, int radius)
{
   
}

int Flocking::flockSize()
{
}


void Flocking::setBounds(int x, int y)
{
	x_bound = x;
	y_bound = y;
}


void Flocking::setDestination(int x, int y,float area)
{
	destination.x = x;
	destination.y = y;
	destinationArea = area;
}


void Flocking::setDestination(Vec2f dest,float area)
{
	destination = dest;
	destinationArea = area;
}


void Flocking::setSceneMap(Scene* scene)
{
	sceneMap = scene;
}

void Flocking::useCollisionSDF(bool val)
{

}

vector<Boid> Flocking::getBoids()
{
	return boids;
}

Vec2f** Flocking::calculatePartialDerivaties()
{
	
		
}



void Flocking::setSimulationParameters(
    		int 	mboundaryPadding 	,
            	float 	mmaxSpeed 		,
            	float 	mmaxForce 		,
          	float 	mflockSepWeight 	,
          	float 	mflockAliWeight 	,
          	float 	mflockCohWeight 	,
          	float 	mcollisionWeight 	,
          	float 	mflockSepRadius 	,
          	float 	mflockAliRadius 	,
          	float 	mflockCohRadius 	,
          	float 	mdestWeight 		)
{
	boundaryPadding = mboundaryPadding;
	maxSpeed = mmaxSpeed;
	maxForce = mmaxForce;
	flockSepWeight = mflockSepWeight;
	flockAliWeight = mflockAliWeight;
	flockCohWeight = mflockCohWeight;
	collisionWeight = mcollisionWeight;
	flockSepRadius = mflockSepRadius;
	flockAliRadius = mflockAliRadius;
	flockCohRadius = mflockCohRadius;
	destWeight = mdestWeight;
}
