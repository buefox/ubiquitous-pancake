#ifndef _GRAPH_H
#define _GRAPH_H
#include <vector>
#include "Server.h"
#include "Edge.h"
#include "Application.h"
#include "User.h"

class Graph {
	public:
		Graph() : total_servers(-1), total_edges(-1), total_users(-1), total_apps(-1) {}
		Graph( int, Server [], int, Edge [], int, User [], int, App [] );
		
		int getTotalServers();
		Server *getServer( int );
		
		int getTotalEdges();
		Edge *getEdge( int );
		
		int getTotalUsers();
		User *getUser( int );
		
		int getTotalApps();
		App *getApp( int );

		void setTotalServers( int );
		void setServer( int, Server );
		
		void setTotalEdges( int );
		void setEdge( int, Edge );
		
		void setTotalUsers( int );
		void setUser( int, User );
		
		void setTotalApps( int );
		void setApp( int, App );
		
		void readAll( const char *, const char *, const char *, const char * );
		void readApps( const char * );
		void readUsers( const char * );
		void readGraph( const char *, const char * );
		void readEdges( const char * );
		void readServers( const char * );
		// void test();
		void showAll();
		void showGraph();
		void showServers();
		void showEdges();
		void showUsers();
		void showApps();
		
		void usersAction();
		
	private:
		int total_servers;
		std::vector<Server> servers;
		
		int total_edges;
		std::vector<Edge> edges;
		
		int total_users;
		std::vector<User> users;
		
		int total_apps;
		std::vector<App> apps;
		
		std::vector< std::vector<int> > distribution;
};

#endif