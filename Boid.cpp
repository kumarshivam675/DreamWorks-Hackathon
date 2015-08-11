#include "Boid.h"
#include<algorithm>

/* Define the functions below */

Boid::Boid(int x, int y, int xbound, int ybound,
        int     mboundaryPadding    ,
        float   mmaxSpeed           ,
        float   mmaxForce           ,
        float   mflockSepWeight     ,
        float   mflockAliWeight     ,
        float   mflockCohWeight     ,
        float   mflockSepRadius     ,
        float   mflockAliRadius     ,
        float   mflockCohRadius     ,
		Scene*  scene               )
{ 

	boundaryPadding = mboundaryPadding;
	maxSpeed = mmaxSpeed;
	maxForce = mmaxForce;
    boundaryPadding = mboundaryPadding;
    flockSepWeight = mflockSepWeight;
    flockAliWeight = mflockAliWeight;
    flockCohWeight = mflockCohWeight;
    flockSepRadius = mflockSepRadius;
    flockAliRadius = mflockAliRadius;
    flockCohRadius = mflockCohRadius;
    endCorner.setval(xbound,ybound);
	tri.setval(mScene->getEndPosition().x, mScene->getEndPosition().y);
	mScene = scene;
    loc.setval(x,y);
	float speedx = mScene->getEndPosition().x - mScene->getStartPosition().x;
	float speedy = mScene->getEndPosition().y - mScene->getStartPosition().y;
    vel.setval(speedx,speedy);
	vel.normalize();
	acc.setval(0,0);

}

// Method to update location
void Boid::update(vector<Boid> &boids) {
	flock(boids);
	vel += acc;
	vel.limit(maxSpeed);
	tri.setval(mScene->getEndPosition().x, mScene->getEndPosition().y);
	bool status = avoid(tri,10);
	loc += vel;
	
	acc *= 0;
	

}

void Boid::seek(Vec2f target,float weight) {
	/*Vec2f desired = target - loc;
	desired.normalize();
	desired *= maxSpeed;
	
	vel = desired - vel;
	vel.limit(maxForce);
	//return steer;*/
}

bool Boid::avoid(Vec2f target,float weight) {
	Vec2f sum;
	sum.setval(0,0);
	float** sdf= mScene->getSDFhandle();
	float sdf_val = sdf[(int)loc.x][(int)loc.y];
	//std::cout << sdf_val << endl;
	if(sdf_val < 5){
		float sdf1 = sdf[(int)loc.x+1][(int)loc.y+1];
		float sdf2 = sdf[(int)loc.x-1][(int)loc.y-1];
		float sdf3 = sdf[(int)loc.x+1][(int)loc.y-1];
		float sdf4 = sdf[(int)loc.x-1][(int)loc.y+1];
		if(vel.x >= 0 && vel.y >= 0)
			sdf2 = 0;
		else if(vel.x >= 0 && vel.y <= 0)
			sdf4 = 0;
		else if(vel.x <= 0 && vel.y <= 0)
			sdf1 = 0;
		else 
			sdf3 = 0;
		float maximum = max(max(sdf1,sdf2),max(sdf3,sdf4));
		if(sdf1 == maximum){
			sum.x = 1;
			sum.y = 1;
			sum.normalize();
		}
		else if(sdf2 == maximum){
			sum.x = -1;
			sum.y = -1;
			sum.normalize();
		}
		else if(sdf3 == maximum){
			sum.x = 1;
			sum.y = -1;
			sum.normalize();
		}
		else if(sdf4 == maximum){
			sum.x = -1;
			sum.y = 1;
			sum.normalize();
		}
		vel = sum;
		//sum.limit(maxSpeed);
		return true;
	}
	return false;
}

void Boid::boundCheck(int padding) {

  


}


// A method that calculates a steering vector towards a target
Vec2f Boid::steer(Vec2f target) {
	Vec2f desired = target - loc;
	desired.normalize();
	desired *= maxSpeed;
	
	Vec2f steer = desired - vel;
	steer.limit(maxForce);
	return steer;
   
}


void Boid::flock(vector<Boid> &boids) {
	Vec2f sep = separate(boids);
	Vec2f alg = align(boids);
	Vec2f coh = cohesion(boids);
	sep *= flockSepWeight;
	alg *= flockAliWeight;
	coh *= flockCohWeight;
	//Vec2f tri;
  //std::cout<<mScene->getEndPosition().x<<" "<<endl;

	tri.setval(mScene->getEndPosition().x, mScene->getEndPosition().y);
	tri -=loc;
	tri.normalize();
	acc += sep;
	acc += alg;
	acc += coh;
	acc += tri;
	
}

bool Boid::isHit(int x, int y, int radius) {
    
}

// Separation
// Method checks for nearby boids and steers away
Vec2f Boid::separate(vector<Boid> &boids) {
	Vec2f v,steer;
	steer.setval(0,0);
	//v.setval(0,0);
	int neighbourCount = 0;
	for(int i = 0; i < boids.size(); i++){
		float d = boids[i].loc.distance(loc);
		if(d>0 && d < flockSepRadius){
			v = loc - boids[i].loc;
			v.normalize();
			v = v/d;
			steer += v;
			neighbourCount++;
		}
	}
	if(neighbourCount > 0)
		steer = steer/neighbourCount;
	if(steer.length() > 0){
		steer.normalize();
		steer *= maxSpeed;
		steer -= vel;
		steer.limit(maxForce);
//		cout << "separating "<<steer.x<<" "<<steer.y<<"\n";
	}
	return steer;
    
}

// Alignment
// For every nearby boid in the system, calculate the average velocity
Vec2f Boid::align(vector<Boid> &boids) {
	Vec2f sum,v;
	Vec2f steer;
	v.setval(0,0);	
	sum.setval(0,0);
	int neighbourCount = 0;
	for(int i = 0; i < boids.size(); i++){

		float d = boids[i].loc.distance(loc);
		if(d>0 && d < flockAliRadius){
			sum += boids[i].vel;
			neighbourCount++;
		}
		
			
	}
	if(neighbourCount > 0){
		sum /= neighbourCount;
		sum.normalize();
		sum *= maxSpeed;
		steer = sum - vel;
		steer.limit(maxForce);
		//cout << "aligning "<<steer.x<<" "<<steer.y<< "\n";
		return steer;
	}
	else{
		return v;
	}
    
}

// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
Vec2f Boid::cohesion(vector<Boid> &boids) {
	Vec2f v,sum;
	v.setval(0,0);
	sum.setval(0,0);
	int neighbourCount = 0;
	for(int i = 0; i < boids.size(); i++){
		float d = boids[i].loc.distance(loc);
		if(d>0 && d < flockCohRadius){
			sum += boids[i].loc;
			neighbourCount++;
		}
	}
	if(neighbourCount > 0){
		sum /= neighbourCount;
		//cout << "cohesing"<<steer(sum).x<<" "<<steer(sum).y<<"\n";
		return 	steer(sum);
	}
	else{
		return v;
	}   
}


float Boid::dist(Vec2f v1,Vec2f v2)
{

}

float Boid::clamp(float val, float minval, float maxval)
{
    
}
