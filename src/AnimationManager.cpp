#include "AnimationManager.h"

AnimationManager::AnimationManager()
{
	bIsDirty = false;
}

void AnimationManager::initRuta()
{
	doneIndexes.clear();
	trazos.clear();

	if (vertices.size() == 0)
		return;

	addTrazosForIndex(0);
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
		for (int c = 0 ; c < vertices[i].conexiones.size() ; c++)
		{
			int ind = vertices[i].conexiones[c];
			if ( i != index && ind == index && !isIndexDone(ind) )
			{
				connections.push_back(ind);
				cout << "DESPUES: index " << index << " -> " << ind << endl;
			}

		}

	for (int i = 0 ; i < connections.size() ; i++)
	{
		cout << "index " << index << " -> " << connections[i] << endl;
	}
}

//
// Ya hemos pasado por el indice
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
	for (int i = 0 ; i < trazos.size() ; i++)
	{
		trazos[i].update();
		if (trazos[i].isDone())
			addTrazosForIndex(trazos[i].toIndex);
	}

	for (int i = 0 ; i < trazos.size() ; i++)
	{
		if (trazos[i].isDone())
		{
			trazos.erase(trazos.begin() + i);
			if (i > 0) i--;
		}
	}
}

void AnimationManager::draw()
{
	for (int i = 0 ; i < trazos.size() ; i++)
	{
		trazos[i].draw();
	}
}

void AnimationManager::addTrazosForIndex(int index)
{
	vector<int> conn;
	getIndexConnections(index, conn);
	for (int i = 0 ; i < conn.size() ; i++)
	{
		Trazo trazo;
		trazo.setup(conn[i], vertices[index].centro, vertices[conn[i]].centro, 1000);
		trazos.push_back(trazo);
		doneIndexes.push_back(conn[i]);
	}
	doneIndexes.push_back(index);
}