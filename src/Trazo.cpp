#include "Trazo.h"

void Trazo::setup ( int fromIndex, int toIndex, ofPoint from, ofPoint to, float totalTime, 
					float *width, ofColor *colorFrom, ofColor *colorTo )
{
	this->fromIndex = fromIndex;
	this->toIndex = toIndex;
	this->from = from;
	this->to = to;
	this->totalTime = totalTime;
	this->width = width;
	this->colorFrom = colorFrom;
	this->colorTo = colorTo;

	drawBola = true;
	radius = 5;

	startTime = ofGetElapsedTimeMillis();
	posActual = from;
	bIsDone = false;
	//bEnabled = true;

}

void Trazo::update ( )
{
	if (bIsDone)
		return;

	ofVec2f v = to - from;
	float dist = v.length();
	v.normalize();
	float d = ofMap(ofGetElapsedTimeMillis() - startTime, 0, totalTime, 0, dist, true);
	posActual = from  + v * d;
	
	if ( (ofGetElapsedTimeMillis() - startTime) >= totalTime)
		bIsDone = true;
}

void Trazo::draw ( )
{
	if (drawLine)
	{
		ofMesh mesh;
		mesh.addVertex(from);
		mesh.addVertex(posActual);
		mesh.addColor(*colorFrom);
		mesh.addColor(*colorTo);
		//mesh.addVertex(from);
		//mesh.addVertex(posActual);
		//mesh.addVertex(posActual);
		//mesh.addVertex(to);
		//mesh.addColor(ofColor(0));
		//mesh.addColor(ofColor(255));
		//mesh.addColor(ofColor(255));
		//mesh.addColor(ofColor(0));
		mesh.setMode(OF_PRIMITIVE_LINES);
		ofSetLineWidth(*width);
		mesh.drawWireframe();
	}
	//ofLine(from, posActual);
	if (drawBola)
	{
		ofSetColor(*colorFrom);
		ofCircle(posActual, radius);
	}

}

bool Trazo::isDone ()
{
	return bIsDone;
}