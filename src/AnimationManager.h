
#pragma once

#include "ofMain.h"
#include "Trazo.h"
#include "ofxGui.h"

#define ANIMATION_MODE_BOLAS	0
#define ANIMATION_MODE_LINEAS	1
#define ANIMATION_MODE_MAX		2

struct VERTICE
{
	ofPoint		centro;			// vertice
	vector<int> conexiones;		// �ndices dentro de vector<vertices>
	ofColor		color;
	float		lineWidth;
	bool		done;
	bool		enabled;
};

class AnimationManager
{
	
	public:
		
							AnimationManager	( );
		void				initRuta			( );
		void				update				( );
		void				draw				( );
		bool				allIndexesDone		( );
		void 				resetIndexesDone	( );

		vector<VERTICE>		vertices;
		bool				bIsDirty;

		ofxIntSlider		paramAnimationMode;
		ofxToggle			paramDrawBolas;
		ofxToggle			paramDrawLineas;
		ofxFloatSlider		paramBolasMinSize;
		ofxFloatSlider		paramBolasMaxSize;
		ofxFloatSlider		paramTrazoMinSpeed;			//Velocidad de las bolas
		ofxFloatSlider		paramTrazoMaxSpeed;
		ofxFloatSlider		paramVelocidadVertices;		//Velocidad de rotaci�n de los vertices	
		ofxFloatSlider		paramVerticesMaxDistance;		//Velocidad de rotaci�n de los vertices	
		ofxFloatSlider		paramVelocidad;
		ofxToggle			paramAnimateWidth;
		ofxFloatSlider		paramLineWidthMin;
		ofxFloatSlider		paramLineWidthMax;
		ofxToggle			paramAnimateColor;

	protected:

		void				drawBolas();
		void				drawLineas();
		void				addTrazosForIndex		( int index, vector<Trazo> &trazosNew);
		void				getIndexConnections		( int index, vector<int> &connections);
		bool				isIndexDone				( int index);
		bool				isTrazoActivo			( int from, int to);
		void				addDoneIndex			( int index );

		struct LINEA
		{
			int from;
			int to;
		};
		vector<LINEA>		doneLines;
		vector<int>			doneIndexes;

		vector<Trazo>		trazos;
		ofMesh				mesh;
};