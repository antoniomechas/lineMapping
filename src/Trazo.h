#include "ofMain.h"

class Trazo
{

	public:
		
		void		setup		( int fromIndex, int toIndex, ofPoint from, ofPoint to, float totalTime, 
									float *width, ofColor *colorFrom, ofColor *colorTo );
		void		update		( );
		void		draw		( );
		bool		isDone		( );
		//bool		isEnabled	( );

		int			fromIndex;
		int			toIndex;
		bool		drawBola;
		bool		drawLine;
		float		radius;
		//bool		bEnabled; //se ha terminado y ya solo hay que dibujarlo, no actualizarlo

	protected:

		ofPoint		from;
		ofPoint		to;
		float		totalTime;
		float		startTime;
		ofPoint		posActual;
		bool		bIsDone;
		float		*width;
		ofColor		*colorFrom;
		ofColor		*colorTo;
};