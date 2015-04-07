#include "ofMain.h"

class Trazo
{

	public:
		
		void		setup	( int toIndex, ofPoint from, ofPoint to, float totalTime );
		void		update	( );
		void		draw	( );
		bool		isDone	( );

		int			toIndex;

	protected:

		ofPoint		from;
		ofPoint		to;
		float		totalTime;
		float		startTime;
		ofPoint		posActual;
};