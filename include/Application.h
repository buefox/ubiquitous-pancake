#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <limits.h>
#include <vector>

class App {
	public:
		App() : index(-1), comp(-1), stor(-1), band(-1), num_servers(-1), num_replicas(-1), max_requests(-1) {}
		App( int, int, int, int, int, int num_r=0, bool reps[]=NULL, int max_r=INT_MAX , int time_window=INT_MAX);
		
		int getIndex();
		int getComp();
		int getStor();
		int getBand();
		
		int getNumServers();
		int getNumReplicas();
		bool getReplica( int );
		
		int getMaxRequests();
		int getNumRequests( int, int );
		
		void setAll( int, int, int, int, int, int, bool [] );
		void setIndex( int );
		void setComp( int );
		void setStor( int );
		void setBand( int );
		//void setImageSize( int );
		void setNumServers( int );
		void setNumReplicas( int );
		void setReplica( int, bool );
		
		void setMaxRequests( int );
		void setNumRequests( int, int, int );

	private:
		int index;
		int comp;
		int stor;
		int band;	
		int num_servers;
		int num_replicas;
		std::vector<bool> replicas; //legth = num_servers
		
		int max_requests;
		std::vector< std::vector<int> >   num_requests;
};

#endif
