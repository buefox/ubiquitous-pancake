#ifndef _SERVER_H
#define _SERVER_H
#include <vector>
// #include "Replica.h"

class Server {
	public:
		// Replica
		Server() : index(-1), comp(-1), stor(-1), total_servers(-1), num_connections(-1), total_edges(-1), num_edges(-1), total_users(-1), num_users(-1), total_apps(-1), num_apps(-1) {}
		Server( int, int, int, int, int, bool [], int, int, bool [], int, int, bool [], int, int );
		
		int getIndex();
		int getComp();
		int getStor();
		
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
		// Replica getReplica( int );
		
		void setAll( int, int, int, int, int, bool [], int, int, bool [], int, int, bool [], int, int );
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
		// void setReplica( int, Replica );

	private:
		int index;
		int comp;
		int stor;
		
		int total_servers;
		int num_connections;
		std::vector<bool> connections;
		
		int total_edges;
		int num_edges;
		std::vector<bool> edges;
		
		int total_users;
		int num_users;
		std::vector<bool> users;
		
		int total_apps;
		int num_apps;
		std::vector<bool> apps;
		// std::vector<Replica> replicas;
};

#endif
