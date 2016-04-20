#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <vector>

class App {
	public:
		App() : index(-1), comp(-1), stor(-1), band(-1), num_servers(-1), num_replicas(-1) {}
		App( int, int, int, int, int, int num_r=0, bool reps[]=NULL );
		
		int getIndex();
		int getComp();
		int getStor();
		int getBand();
		int getImageSize();
		int getNumServers();
		int getNumReplicas();
		bool getReplica( int );
		
		void setAll( int, int, int, int, int, int, bool [] );
		void setIndex( int );
		void setComp( int );
		void setStor( int );
		void setBand( int );
		void setImageSize( int );
		void setNumServers( int );
		void setNumReplicas( int );
		void setReplica( int, bool );

	private:
		int index;
		int comp;
		int stor;
		int band;

		int num_servers;
		int num_replicas;
		std::vector<bool> replicas;
};

#endif
