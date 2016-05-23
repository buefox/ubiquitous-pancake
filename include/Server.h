#ifndef _SERVER_H
#define _SERVER_H
#include <vector>
// #include "Replica.h"

class Server {
	public:
		// Replica		
		Server() : index(-1), comp(-1), stor(-1), power_idle(-1), power_peak(-1), total_servers(-1), num_connections(-1), total_edges(-1), num_edges(-1), total_users(-1), num_users(-1), total_apps(-1), num_apps(-1) {}
		Server( int, int, int, int, int, int, int, bool [], int, int, bool [], int, int, bool [], int, int, bool[], int[] );
		
		int getIndex();
		int getComp();
		int getStor();
		int getPowerIdle();
		int getPowerPeak();
		
		int getTotalServers();
		int getNumConnections();
		bool getConnection( int );
		
		int getTotalEdges();
		int getNumEdges();
		bool getEdge( int );
		
		int getTotalUsers();
		int getNumUsers();
		bool getUser( int );
		
		int getTotalApps();
		int getNumApps();
		bool getApp( int );
		int getServing( int );
		
		int getUsedComp();
		int getUsedStor();
		double getUtilization(); // CPU
		double getPower();
		
		void setAll( int, int, int, int, int, bool [], int, int, bool [], int, int, bool [], int, int, bool [], int [] );
		void setIndex( int );
		void setComp( int );
		void setStor( int );
		
		void setTotalServers( int );
		void setNumConnections( int );
		void setConnection( int, bool );
		
		void setTotalEdges( int );
		void setNumEdges( int );
		void setEdge( int, bool );
		
		void setTotalUsers( int );
		void setNumUsers( int );
		void setUser( int, bool );
		
		void setTotalApps( int );
		void setNumApps( int );
		void setApp( int, bool );
		void setServing( int, int );
		
		void setUsedComp( int used=0 );
		void setUsedStor( int used=0 );
		void setUtilization( double u=0 ); // CPU
		void setPower( double p=0 );

	private:
		int index; // Server number
		int comp;  // CPU
		int stor;  // Storage
		
		int power_idle;
		int power_peak;
		
		int total_servers;
		int num_connections; // labeling total number of servers that connect to this server
		std::vector<bool> connections;  // labeling which servers are connected th this server
		
		int total_edges; // total links
		int num_edges;  
		std::vector<bool> edges;  // labeling which links are connected to this server
		
		int total_users;
		int num_users;
		std::vector<bool> users;
		
		int total_apps;
		int num_apps;
		std::vector<bool> apps;
		std::vector<int> serving; // the replica served app k from this server i
		
		int used_comp;
		int used_stor;
		double utilization;
		double power;
};

#endif
