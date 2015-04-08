#include "AnimationManager.h"

AnimationManager::AnimationManager()
{
	bIsDirty = false;
}

void AnimationManager::initRuta()
{
	doneIndexes.clear();
	doneLines.clear();
	trazos.clear();

	if (vertices.size() == 0)
		return;

	addTrazosForIndex(0, trazos);
}

//
//Devuelve todas las conexiones que tiene el vertice especificado 
//pueden ser desde o hacia
//
void AnimationManager::getIndexConnections(int index, vector<int> &connections)
{
	connections.clear();
	
	if (vertices[index].conexiones.size() > 0)
	{
		for (int c = 0 ; c < vertices[index].conexiones.size() ; c++)
		{
			if (!isIndexDone(vertices[index].conexiones[c]))
				connections.push_back(vertices[index].conexiones[c]);
		}
	}

	for (int i = 0 ; i < vertices.size() ; i++)
	{
		for (int c = 0 ; c < vertices[i].conexiones.size() ; c++)
		{
			int ind = vertices[i].conexiones[c];
			if ( i != index && ind == index && !isIndexDone(i) )
			{
				connections.push_back(i);
				//cout << "PARA: " << index << " DESDE " << i << endl;
			}
		}
	}
	//for (int i = 0 ; i < connections.size() ; i++)
	//{
	//	cout << "TODOS: index " << index << " -> " << connections[i] << endl;
	//}
}

//
// Si ya hemos pasado por el indice
//
bool AnimationManager::isIndexDone(int index)
{
	bool r = false;

	for (int i = 0 ; i < doneIndexes.size() ; i++)
		if (doneIndexes[i] == index)
			r = true;

	return r;
}

void AnimationManager::update()
{
	vector<Trazo> trazosNew;
	vector<Trazo>::iterator it = trazos.begin();
	while ( it != trazos.end() )
	{
		it->update();
		if (it->isDone())
		{
			addTrazosForIndex(it->toIndex, trazosNew);
			it = trazos.erase(it);
		}
		else
		{
			++it;
		}
	}
	
	//Elimina duplicados
	for (int i = 0 ; i < trazosNew.size() ; i++)
	{
		for (int k = 0 ; k < trazosNew.size() ; k++)
		{
			if (i != k)
			{
				if (trazosNew[i].fromIndex == trazosNew[k].fromIndex && trazosNew[i].toIndex == trazosNew[k].toIndex)
				{
					trazosNew.erase(trazosNew.begin() + k);
					k--; 
				}
			}
		}
		
	}

	trazos.insert(trazos.end(),trazosNew.begin(), trazosNew.end());
}

void AnimationManager::draw()
{
	switch (paramAnimationMode)
	{
		case ANIMATION_MODE_BOLAS:
			drawBolas();
			break;

		case ANIMATION_MODE_LINEAS:
			drawLineas();
			break;

	}

}

void AnimationManager::drawBolas()
{
	for (int i = 0 ; i < trazos.size() ; i++)
	{
		trazos[i].draw();
	}
}

void AnimationManager::drawLineas()
{

	ofSetLineWidth(paramLineWidthMin);
	for (int i = 0 ; i < vertices.size() ; i++)
	{
		for (int v = 0 ; v < vertices[i].conexiones.size(); v++)
		{
			int index = vertices[i].conexiones[v];
			ofVec2f offset(0,0);
			ofVec2f offset2(0,0);
			if (paramVelocidadVertices > 0)
			{
				float x = ofSignedNoise(ofGetElapsedTimef() * paramVelocidadVertices, 0) * 3;
				float y = ofSignedNoise(0, ofGetElapsedTimef() * paramVelocidadVertices) * 3;
				offset.set(x,y);
				x = ofSignedNoise(100.+ ofGetElapsedTimef() * paramVelocidadVertices, 0) * 3;
				y = ofSignedNoise(0, 100. + ofGetElapsedTimef() * paramVelocidadVertices) * 3;
				offset2.set(x,y);
			}
			ofLine(vertices[i].centro + offset, vertices[index].centro + offset2);
		}
		ofFill();
		ofCircle(vertices[i].centro, 5 + paramLineWidthMin * .3);
	}
}



void AnimationManager::addTrazosForIndex(int index, vector<Trazo> &trazosNew)
{
	vector<int> conn;
	getIndexConnections(index, conn);
	for (int i = 0 ; i < conn.size() ; i++)
	{
		if (!isTrazoActivo(index, conn[i]))
		{
			Trazo trazo;
			trazo.setup(index, conn[i], vertices[index].centro, vertices[conn[i]].centro, ofRandom(paramTrazoMinSpeed, paramTrazoMaxSpeed));
			trazosNew.push_back(trazo);
			//doneIndexes.push_back(conn[i]);
		}
	}
	//doneIndexes.push_back(index);
}

bool AnimationManager::isTrazoActivo(int from, int to)
{
	
	for (int i = 0 ; i < trazos.size() ; i++)
	{
		if (trazos[i].fromIndex == from && trazos[i].toIndex == to)
			return true;
	}
	
	return false;

}