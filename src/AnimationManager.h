
#pragma once

#include "ofMain.h"
#include "Trazo.h"

struct VERTICE
{
	ofPoint		centro;			// vertice
	vector<int> conexiones;		// índices dentro de vector<vertices>
};

class AnimationManager
{
	
	public:
		
							AnimationManager	( );
		void				initRuta			( );
		void				update				( );
		void				draw				( );

		vector<VERTICE>		vertices;
		bool				bIsDirty;

	protected:

		void				addTrazosForIndex(int index);
		void				getIndexConnections(int index, vector<int> &connections);
		bool				isIndexDone(int index);

		//vector<int>			ruta;
		vector<int>			doneIndexes;
		
		vector<Trazo> trazos;
};