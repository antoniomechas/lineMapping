#include "Trazo.h"

void Trazo::setup ( int fromIndex, int toIndex, ofPoint from, ofPoint to, float totalTime )
{
	this->fromIndex = fromIndex;
	this->toIndex = toIndex;
	this->from = from;
	this->to = to;
	this->totalTime = totalTime;
	
	startTime = ofGetElapsedTimeMillis();
	posActual = from;

}

void Trazo::update ( )
{
	ofVec2f v = to - from;
	float dist = v.length();
	v.normalize();
	float d = ofMap(ofGetElapsedTimeMillis() - startTime, 0, totalTime, 0, dist, true);
	posActual = from  + v * d;
}

void Trazo::draw ( )
{
	ofCircle(posActual,5);
}

bool Trazo::isDone ()
{
	
	if ( (ofGetElapsedTimeMillis() - startTime) >= totalTime)
		return true;
	
	return false;
}