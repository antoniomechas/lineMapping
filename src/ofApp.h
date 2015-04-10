#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "AnimationManager.h"
#include "ofxPostProcessing.h"
#include "ofxBloom.h"
#include "ofxGlow.h"
#include "ofxSpout.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	protected:

		int		selectIndex(int x, int y);
		void	drawVertices();
		void	saveModel();
		void	loadModel();
		void	deleteIndex(int index);
		void	setupGui();
		void	loadSettings (int p );
		void	saveSettings (int p );
		void	drawEdit();
		void	drawAnimation();
		void	drawBallRute();
		void	createRute();

		AnimationManager	animationManager;

		bool				bMousePressed;
		int					indexSelected;

		ofPoint				lineFrom;
		ofPoint				lineTo;

		int					indexHover;

		bool				bMovingVertex;
		bool				bHasDraggedVertex;

		int					preset;
		ofxPanel			gui;
		bool				bGuiVisible;
		ofxToggle			paramSpout;
		ofxColorSlider		paramBackGroundColor;
		ofxColorSlider		paramLineColor;
		ofxFloatSlider		paramLineWidth;
		ofxToggle			paramEdit;

		ofxToggle			paramFxOn;
		ofxIntSlider		paramFxRadius;
		ofxIntSlider		paramFxPasses;

		vector<int>			ruta;
		bool				bRutaInited;

		ofxPostProcessing	post1;
		ofxPostProcessing	post2;
		ofFbo				fbo;
		ofxBloom			bloom;
		ofxGlow				glow;

		int					loopCount;

};
