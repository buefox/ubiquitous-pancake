#ifndef _USER_H
#define _USER_H
#include <vector>

class User {
	public:
		User() : index(-1), location(-1), num_apps(-1) {}
		User( int, int, int, bool [], bool reqs[]=NULL );
		
		int getIndex();
		int getLocation();
		int getNumApps();
		bool getApplication( int );
		bool getRequest( int );

		void setAll( int, int, int, bool [], bool [] );
		void setIndex( int );
		void setLocation( int );
		void setNumApps( int );
		void setApplication( int, bool );
		void setRequest( int, bool );

		void launchRequests(); // all the possible applications

	private:
		int index;
		int location;
		
		/* total_apps */
		int num_apps;
		std::vector<bool> applications; // used or not
		// overtime
		std::vector<bool> requests; // launched or not
};

#endif
