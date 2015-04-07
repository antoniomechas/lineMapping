#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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

		struct VERTICE
		{
			ofPoint		centro;			// vertice
			vector<int> conexiones;		// índices dentro de vector<vertices>
		};
		
		vector<VERTICE>		vertices;

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
		ofxColorSlider		paramBackGroundColor;
		ofxColorSlider		paramLineColor;
		ofxFloatSlider		paramLineWidth;
};
