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
	bGuiVisible = false;
	preset = 1;
	paramEdit = false;

	setupGui();
	loadSettings(preset);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	animationManager.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (paramEdit)
		drawEdit();
	else
		drawAnimation();

	if (bGuiVisible)
	{
		ofSetColor(255,255,255);
		gui.draw();
	}
}

//
//Dibuja el modo edici�n del modelo
//
void ofApp::drawEdit()
{
	ofBackground(paramBackGroundColor);
	//ofSetBackgroundColor(paramBackGroundColor);
	ofSetLineWidth(paramLineWidth);
	ofSetColor(paramLineColor);

	drawVertices();

	if (bMousePressed)
	{
		if (indexSelected > -1)
		{
			//ofSetColor(255,255,255);
			ofFill();
			ofCircle(animationManager.vertices[indexSelected].centro, 5 + ofNoise(ofGetElapsedTimef()) * 20);
		}
		ofLine(lineFrom, lineTo);
	}

	if (indexHover > -1)
	{
		cout << indexHover << endl;
		ofSetColor(255,0,255);
		//ofCircle(vertices[indexHover].centro, 5 + ofNoise(100. + ofGetElapsedTimef()) * 20);
		ofPoint centro(animationManager.vertices[indexHover].centro);
		ofLine(centro - ofPoint(10,0), centro + ofPoint(10,0));
		ofLine(centro - ofPoint(0,10), centro + ofPoint(0,10));
	}
}

void ofApp::drawAnimation()
{
	ofBackground(0,0,0);
	ofSetColor(255,255,255);
	drawEdit();
	ofSetColor(255,0,0);
	animationManager.draw();
}

void ofApp::drawVertices()
{
	ofSetColor(paramLineColor);
	for (int i = 0 ; i < animationManager.vertices.size() ; i++)
	{
		for (int v = 0 ; v < animationManager.vertices[i].conexiones.size(); v++)
		{
			int index = animationManager.vertices[i].conexiones[v];
			ofLine(animationManager.vertices[i].centro, animationManager.vertices[index].centro);
		}
		ofFill();
		ofCircle(animationManager.vertices[i].centro, 5 + paramLineWidth * .3);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	switch (key)
	{
		case 'l':
			loadModel();
			break;

		case OF_KEY_DEL:
			if (indexSelected > -1)
				deleteIndex(indexSelected);
			bMousePressed = false;
			break;

		case ' ':
			bGuiVisible = !bGuiVisible;
			break;

		case 's':
			saveSettings(preset);
			break;

		case 'i':
			animationManager.initRuta();
			break;

	}
	if (key > '0' && key < '9')
	{
		preset = key - '0';
		loadSettings(preset);
	}	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	if (!paramEdit)
		return;

	lineTo.set(x,y);

	indexHover = selectIndex(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//if (bMovingVertex)

	if (!paramEdit)
		return;

	if (indexSelected > -1)
	{
		animationManager.vertices[indexSelected].centro.set(x,y);
		bHasDraggedVertex = true;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//cout << button << endl;
	//Compruebo si es el segundo click
	
	if (!paramEdit)
		return;

	if (bMousePressed)
	{
		if (button == OF_MOUSE_BUTTON_3) //Boton derecho, anular
		{
			bMousePressed = false;
			indexSelected = -1;
		}
		if (button == OF_MOUSE_BUTTON_1) //Boton izquierdo, a�adir conexi�n
		{
			int index1 = 0;
			if (indexHover == -1)
			{
				VERTICE vert2;
				vert2.centro = lineTo;
				animationManager.vertices.push_back(vert2);
				index1 = animationManager.vertices.size() - 1;
			}
			else
				index1 = indexHover;
			
			if (indexSelected > -1)
				animationManager.vertices[indexSelected].conexiones.push_back(index1);
			else 
			{
				//Linea que no tiene un vertice origen (la primera linea de todas)
				VERTICE vert1;
				vert1.centro = lineFrom;
				vert1.conexiones.push_back(index1);
				animationManager.vertices.push_back(vert1);
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
			lineFrom = animationManager.vertices[indexSelected].centro;
			//if(ofGetKeyPressed(OF_KEY_CONTROL))
				bMovingVertex = true;
		}
		else
			lineFrom.set(x,y);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	if (!paramEdit)
		return;
	
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
	for (int i = 0 ; i < animationManager.vertices.size(); i++)
	{
		if ( animationManager.vertices[i].centro.distance(ofVec3f(x,y,0)) < 20 )
			return i;
	}
	return -1;
}

void ofApp::saveModel()
{
	ofBuffer dataBuffer;
	stringstream t;
	for (int i = 0 ; i < animationManager.vertices.size() ; i++)
	{
		if (animationManager.vertices[i].conexiones.size() == 0)
			t << i << "," << animationManager.vertices[i].centro.x << "," << animationManager.vertices[i].centro.y << "," << -1 << endl;
		else
		{
			for (int c = 0 ; c < animationManager.vertices[i].conexiones.size() ; c++)
			{
				t << i << "," << animationManager.vertices[i].centro.x << "," << animationManager.vertices[i].centro.y << "," << animationManager.vertices[i].conexiones[c] << endl;
			}
		}
	}
	// fill the buffer with something important
	dataBuffer.append(t.str());
	bool fileWritten = ofBufferToFile("vertices.txt", dataBuffer);
}

void ofApp::loadModel()
{
	ofBuffer dataBuffer = ofBufferFromFile("vertices.txt");
	animationManager.vertices.clear();
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
			if (animationManager.vertices.size() == index)
			{
				// es nuevo
				animationManager.vertices.push_back(v);
			}
			else
			{
				animationManager.vertices[index].conexiones.push_back(num[3]);
			}
		}
		if (dataBuffer.isLastLine())
			break;
		t = dataBuffer.getNextLine();
	}
	
}

void ofApp::deleteIndex (int index)
{
	for (int i = 0 ; i < animationManager.vertices.size() ; i++)
		for (int c = 0 ; c < animationManager.vertices[i].conexiones.size() ; c++) 
			if (animationManager.vertices[i].conexiones[c] == index)
				animationManager.vertices[i].conexiones.erase(animationManager.vertices[i].conexiones.begin() + c);

	animationManager.vertices[index].centro.set(-1,-1);
	animationManager.vertices[index].conexiones.clear();

}

void ofApp::setupGui()
{
	//--------------------------------------------------------------
	bGuiVisible = false;
    
	gui.setup("lineMapping");
    
    gui.add(paramEdit.setup("Edit", false));
	gui.add(paramBackGroundColor.setup("BackGround", ofColor(0,0,0),ofColor(0,0),ofColor(255,255)));
    gui.add(paramLineColor.setup("Line Color", ofColor(0,0,0),ofColor(0,0),ofColor(255,255)));
    gui.add(paramLineWidth.setup("Line Width", 1,1,20));

}

void ofApp::loadSettings(int p)
{
	stringstream filename;
	filename << "settings_" << p << ".xml";
	gui.loadFromFile(filename.str()); 
	paramEdit = false;
}

void ofApp::saveSettings(int p)
{
	stringstream filename;
	filename << "settings_" << p << ".xml";
	gui.saveToFile(filename.str()); 
	saveModel();
}