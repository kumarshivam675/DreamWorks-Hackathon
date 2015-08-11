#include "Simulation.h"
#include <unistd.h>     // Header File for sleeping.


using namespace math;

/***************** Define some functions *******************/

void
Simulation::loadScene(char* mapFile)
{
	auto startTime = std::chrono::steady_clock::now();
	mStartTime = std::chrono::steady_clock::now();

    bool **data = ml.loadVDBMap(mapFile);   //Loading VDB files now

	startPosition = ml.getStartPosition();
	endPosition = ml.getEndPosition();
	
    x_bound = ml.getx_boundary();
    y_bound = ml.gety_boundary();

    printf("The grid is %d*%d\n", x_bound, y_bound);

	bool* passData = new bool[x_bound * y_bound];

	for(int y=0;y<y_bound;y++) {
        for(int x=0;x<x_bound;x++) {
            passData[y*x_bound + x] = data[x][y];
            if(passData[y*x_bound + x]) {
                //printf(".");
            } else {
                //printf("#");
            }
        }
        //printf("\n");
    }

	Grid<bool> mapData(x_bound, y_bound, passData);
	mScene = new Scene(startPosition, endPosition, mapData , ml.getStartRadius(), ml.getEndRadius());
   	mScene->setSDFhandle(ml.getSDF());
	flock.setSceneMap(mScene);
	flock.addBoid(startPosition.x,startPosition.y);
	flock.setDestination(endPosition,1.0);

	auto endTime = std::chrono::steady_clock::now();
	std::cout << "Map Loading Time (ms) : " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;

}

bool
Simulation::updateLoop()
{
	//std::cout << "update loop in simulation\n";
	flock.update();
	return true;
}



void
Simulation::init(long 	msleepTime 		,
              	 int 	 mfishCount 		,
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
   		float 	mdestWeight 		,
   		int 	mrandSeed 		)

{
	auto startTime = std::chrono::steady_clock::now();
	auto endTime = std::chrono::steady_clock::now();
	sleepTime = msleepTime;
	fishCount = mfishCount;
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
	randSeed = mrandSeed;
	flock.setSimulationParameters(boundaryPadding,maxSpeed, maxForce,flockSepWeight, flockAliWeight, flockCohWeight, collisionWeight,
					flockSepRadius, flockAliRadius, flockCohRadius, destWeight);
  //Initialize variables    
}



void
Simulation::run()
{
	//std::cout << "in simulation wala run\n";
	bool continueRunning = true;
	static long long simTime = 0;
	
  while (continueRunning) {
		auto startTime = std::chrono::steady_clock::now();

		continueRunning = updateLoop();
		std::this_thread::sleep_for (std::chrono::milliseconds(30));
		auto endTime = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

		simTime+=duration;

	}

	std::cout << "Flocking simulation time (ms) : "<<simTime<<std::endl;
	mEndTime = std::chrono::steady_clock::now();
}


long long
Simulation::totalTime()
{
	return std::chrono::duration_cast<std::chrono::seconds>(mEndTime - mStartTime).count();
}


Flocking* Simulation::getFlockHandle()
{
        return &flock;
}

Scene* Simulation::getSceneHandle()
{
        return mScene;
}
