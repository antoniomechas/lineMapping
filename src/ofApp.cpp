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

	post2.init(ofGetWidth(), ofGetHeight());
    post2.createPass<BloomPass>()->setEnabled(true);
    post2.createPass<BloomPass>()->setEnabled(true);
    post2.createPass<BloomPass>()->setEnabled(true);
    post2.createPass<BloomPass>()->setEnabled(true);
    post2.createPass<FxaaPass>()->setEnabled(true);
	bloom.allocate(ofGetWidth(), ofGetHeight());
	glow.allocate(ofGetWidth(), ofGetHeight());

	setupGui();
	loadSettings(preset);

	ofxSpout::init("lineMapping", ofGetWidth(), ofGetHeight(), true);	
	loopCount = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	
	animationManager.update();
	if (animationManager.allIndexesDone())
	{
		animationManager.resetIndexesDone();
		animationManager.initRuta();
		loopCount++;	
		//cout << "loop: " << loopCount << endl;
		if (loopCount > 3)
		{
			loopCount = 0;
			animationManager.initRuta();
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	// init sender if it's not already initialized
	ofxSpout::initSender();
	//cout << "spout: " << ofxSpout::_isSpoutTextureShared << endl;
	if (paramEdit)
		drawEdit();
	else
		drawAnimation();
	
	// send screen to Spout
	ofxSpout::sendTexture();
	
	if (bGuiVisible)
	{
		ofSetColor(255,255,255);
		gui.draw();
	}
}

//
//Dibuja el modo edición del modelo
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
			ofSetColor(255,0,255);
			//ofCircle(vertices[indexHover].centro, 5 + ofNoise(100. + ofGetElapsedTimef()) * 20);
			ofPoint centro(animationManager.vertices[indexSelected].centro);
			ofLine(centro - ofPoint(10,0), centro + ofPoint(10,0));
			ofLine(centro - ofPoint(0,10), centro + ofPoint(0,10));
		}
		ofSetColor(paramLineColor);
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
	//glow.setRadius( 1 + ofNoise(ofGetElapsedTimef() * paramVelocidad) * 3);
	//bloom.setRadius( 1 );

	//glow.draw();
	//drawEdit();
	//ofSetColor(255,0,0);
	if (paramFxOn)
	{
		//bloom.passes = paramFxPasses;
		//bloom.radius = paramFxRadius;
		//bloom.begin();
		for (int t = 0 ; t < 3 ; t++)
			post2[t]->setEnabled(false);

		if (paramFxPasses > 1)
			post2[0]->setEnabled(true);
		if (paramFxPasses > 2)
			post2[1]->setEnabled(true);
		if (paramFxPasses > 3)
			post2[2]->setEnabled(true);
		if (paramFxPasses > 4)
			post2[3]->setEnabled(true);
		post2.begin();
	}

	//ofBackground(0,0,0);
	ofSetColor(paramLineColor);
	animationManager.draw();
	
	if (paramFxOn)
	{	
		post2.end();
		//bloom.end();
		//bloom.update();
		//ofSetColor(255,255,255);
		//bloom.draw();
	}
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

		case OF_KEY_LEFT:
			if (bMousePressed && indexSelected > -1)
				animationManager.vertices[indexSelected].centro.x--;
			break;
		case OF_KEY_RIGHT:
			if (bMousePressed && indexSelected > -1)
				animationManager.vertices[indexSelected].centro.x++;
			break;
		case OF_KEY_UP:
			if (bMousePressed && indexSelected > -1)
				animationManager.vertices[indexSelected].centro.y--;
			break;
		case OF_KEY_DOWN:
			if (bMousePressed && indexSelected > -1)
				animationManager.vertices[indexSelected].centro.y++;
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
		if (button == OF_MOUSE_BUTTON_1) //Boton izquierdo, añadir conexión
		{
			int index1 = 0;
			if (indexHover == -1)
			{
				VERTICE vert2;
				vert2.centro = lineTo;
				vert2.enabled = true;
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
				vert1.enabled = true;
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
			
			if (v.centro.x == -1 && v.centro.y == -1)
				v.enabled = false;
			else
				v.enabled = true;

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

	animationManager.vertices[index].enabled = false;
	animationManager.vertices[index].centro.set(-1,-1);
	animationManager.vertices[index].conexiones.clear();

}

void ofApp::setupGui()
{
	//--------------------------------------------------------------
	bGuiVisible = false;
    
	gui.setup("lineMapping");
    
    gui.add(paramSpout.setup("Spout", false));
    gui.add(paramEdit.setup("Edit", false));
	gui.add(paramBackGroundColor.setup("BackGround", ofColor(0,0,0),ofColor(0,0),ofColor(255,255)));
    gui.add(paramLineColor.setup("Line Color", ofColor(0,0,0),ofColor(0,0),ofColor(255,255)));
    gui.add(paramLineWidth.setup("Line Width", 1,1,20));

	gui.add(animationManager.paramAnimationMode.setup("Animation Mode", 0, 0, ANIMATION_MODE_MAX -1 ));
	gui.add(animationManager.paramVelocidad.setup("Velocidad", 1,.1,10));

	gui.add(animationManager.paramDrawBolas.setup("Draw Bolas", false));
	gui.add(animationManager.paramDrawLineas.setup("Draw Lines", false));
	gui.add(animationManager.paramBolasMinSize.setup("Bolas min size", 1,1,40));
	gui.add(animationManager.paramBolasMaxSize.setup("Bolas max size", 1,1,40));
	gui.add(animationManager.paramVelocidadVertices.setup("Vel Vertices", 0, 0, 10));
	gui.add(animationManager.paramVerticesMaxDistance.setup("Vertices Max Dst", 1, 1, 20));

	gui.add(animationManager.paramAnimateWidth.setup("Trazo Animate Width", false));
    gui.add(animationManager.paramLineWidthMin.setup("Line Width Min", 1,1,20));
    gui.add(animationManager.paramLineWidthMax.setup("Line Width Max", 1,1,20));

	gui.add(animationManager.paramAnimateColor.setup("Animate Color", false));

	gui.add(animationManager.paramTrazoMinSpeed.setup("Trazo Min Speed", 100,100,10000));
    gui.add(animationManager.paramTrazoMaxSpeed.setup("Trazo Max Speed", 100,100,10000));

	gui.add(paramFxOn.setup("Fx On", false));
	gui.add(paramFxPasses.setup("Fx Passes", 1,1,15));
	gui.add(paramFxRadius.setup("Fx Radius", 1,1,15));

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

void ofApp::exit()
{
	// exit spout
	ofxSpout::exit();
}