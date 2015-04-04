#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	bMousePressed = false;
	indexSelected = -1;
	indexHover = -1;
	lineFrom.set(0,0);
	lineTo.set(0,0);
	bMovingVertex = false;
	bHasDraggedVertex = false;

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	drawVertices();

	if (bMousePressed)
	{
		if (indexSelected > -1)
		{
			ofSetColor(255,255,255);
			ofFill();
			ofCircle(vertices[indexSelected].centro, 5 + ofNoise(ofGetElapsedTimef()) * 20);
		}
		ofLine(lineFrom, lineTo);
	}

	if (indexHover > -1)
	{
		cout << indexHover << endl;
		ofSetColor(255,0,255);
		//ofCircle(vertices[indexHover].centro, 5 + ofNoise(100. + ofGetElapsedTimef()) * 20);
		ofPoint centro(vertices[indexHover].centro);
		ofLine(centro - ofPoint(10,0), centro + ofPoint(10,0));
		ofLine(centro - ofPoint(0,10), centro + ofPoint(0,10));
	}
}

void ofApp::drawVertices()
{
	for (int i = 0 ; i < vertices.size() ; i++)
	{
		for (int v = 0 ; v < vertices[i].conexiones.size(); v++)
		{
			int index = vertices[i].conexiones[v];
			ofSetColor(255,255,255);
			ofLine(vertices[i].centro, vertices[index].centro);
		}
		ofSetColor(255,255,255);
		ofFill();
		ofCircle(vertices[i].centro, 5);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	if (key == 's')
		save();

	if (key == 'l')
		load();

	if (key == OF_KEY_DEL)
	{
		if (indexSelected > -1)
			deleteIndex(indexSelected);
		bMousePressed = false;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	lineTo.set(x,y);

	indexHover = selectIndex(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//if (bMovingVertex)
	if (indexSelected > -1)
	{
		vertices[indexSelected].centro.set(x,y);
		bHasDraggedVertex = true;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//cout << button << endl;
	//Compruebo si es el segundo click
	if (bMousePressed)
	{
		if (button == OF_MOUSE_BUTTON_3) //Boton derecho, anular
		{
			bMousePressed = false;
			indexSelected = -1;
		}
		if (button == OF_MOUSE_BUTTON_1) //Boton izquierdo, añadir conexión
		{
			int index1 = 0;
			if (indexHover == -1)
			{
				VERTICE vert2;
				vert2.centro = lineTo;
				vertices.push_back(vert2);
				index1 = vertices.size() - 1;
			}
			else
				index1 = indexHover;
			
			if (indexSelected > -1)
				vertices[indexSelected].conexiones.push_back(index1);
			else 
			{
				//Linea que no tiene un vertice origen (la primera linea de todas)
				VERTICE vert1;
				vert1.centro = lineFrom;
				vert1.conexiones.push_back(index1);
				vertices.push_back(vert1);
			}
			bMousePressed = false;
			indexSelected = -1;
		}
	}
	else
	{
		bMousePressed = true;
		indexSelected = selectIndex(x,y);
		if (indexSelected > -1)
		{
			lineFrom = vertices[indexSelected].centro;
			//if(ofGetKeyPressed(OF_KEY_CONTROL))
				bMovingVertex = true;
		}
		else
			lineFrom.set(x,y);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
	if (bMovingVertex && bHasDraggedVertex)
		bMousePressed = false;
	
	bMovingVertex = false;
	bHasDraggedVertex = false;
	//indexSelected = -1;

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

int ofApp::selectIndex(int x, int y)
{
	for (int i = 0 ; i < vertices.size(); i++)
	{
		if ( vertices[i].centro.distance(ofVec3f(x,y,0)) < 20 )
			return i;
	}
	return -1;
}

void ofApp::save()
{
	ofBuffer dataBuffer;
	stringstream t;
	for (int i = 0 ; i < vertices.size() ; i++)
	{
		if (vertices[i].conexiones.size() == 0)
			t << i << "," << vertices[i].centro.x << "," << vertices[i].centro.y << "," << -1 << endl;
		else
		{
			for (int c = 0 ; c < vertices[i].conexiones.size() ; c++)
			{
				t << i << "," << vertices[i].centro.x << "," << vertices[i].centro.y << "," << vertices[i].conexiones[c] << endl;
			}
		}
	}
	// fill the buffer with something important
	dataBuffer.append(t.str());
	bool fileWritten = ofBufferToFile("vertices.txt", dataBuffer);
}

void ofApp::load()
{
	ofBuffer dataBuffer = ofBufferFromFile("vertices.txt");
	vertices.clear();
	string t = dataBuffer.getFirstLine();
	vector<int> num;
	while ( true )
	{
		cout << t << endl;
		num.clear();
		const char *str = t.c_str();
		do
		{
			const char *begin = str;

			while(*str != ',' && *str)
				str++;

			num.push_back(ofToInt(string(begin, str)));
		} while (0 != *str++);
		// tiene que haber 4 numeros, vertice indice, centro.x, centro.y, conexion
		if (num.size() != 4)
			cout << "error leyendo archivo" << endl;
		else
		{
			VERTICE v;
			v.centro.x = num[1];
			v.centro.y = num[2];
			if (num[3] != -1)
				v.conexiones.push_back(num[3]);
			int index = num[0];
			if (vertices.size() == index)
			{
				// es nuevo
				vertices.push_back(v);
			}
			else
			{
				vertices[index].conexiones.push_back(num[3]);
			}
		}
		if (dataBuffer.isLastLine())
			break;
		t = dataBuffer.getNextLine();
	}
	
}

void ofApp::deleteIndex (int index)
{
	for (int i = 0 ; i < vertices.size() ; i++)
		for (int c = 0 ; c < vertices[i].conexiones.size() ; c++) 
			if (vertices[i].conexiones[c] == index)
				vertices[i].conexiones.erase(vertices[i].conexiones.begin() + c);

	vertices[index].centro.set(-1,-1);
	vertices[index].conexiones.clear();

}