#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdlib.h>
#include <limits.h>

#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>

#include "Server.h"
#include "Edge.h"
#include "Application.h"
#include "User.h"

typedef struct Vertex {
	int index;
	int distance;
	int parent;
	int cost;
} vertex;

/*   below are helper data structure of implementing mcmf */
struct _edge{
	int to;
	int cap;
	int cost;
	_edge* reverse_edge;
	_edge(int d, int cap, int cost) : to(d), cap(cap), cost(cost), reverse_edge(NULL) {}
};
struct Ans
{
	int cost;
	int cap;
	Ans(int cost, int cap) : cost(cost), cap(cap) {};
};

typedef std::vector< std::vector<_edge*> > _edges;

struct nodeinfo{
	int cost;
	int cap;
	bool inq;
	_edge* income_edge;
	nodeinfo(int cost, int cap, bool inq, _edge* e) : cost(cost), cap(cap), inq(inq), income_edge(e) {}
};
/*   above are helper data structure of implementing mcmf */

class Graph {
	public:
		Graph() : time_window(-1), power_threshold(-1), total_servers(-1), total_edges(-1), total_users(-1), total_apps(-1) {}
		Graph( int, int, int, Server [], int, Edge [], int, User [], int, App [] );
		
		int getTimeWindow();
		int getPowerThreshold();
		double getPower();
		
		int getTotalServers();
		Server *getServer( int );
		
		int getTotalEdges();
		Edge *getEdge( int );
		
		int getTotalUsers();
		User *getUser( int );
		
		int getTotalApps();
		App *getApp( int );

		void setTimeWindow( int );
		void setPowerThreshold( int );
		void setPower( double p=0 );
		
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

		void showAll();
		void showGraph();
		void showServers();
		void showEdges();
		void showUsers();
		void showApps();
		
		void userMovement( int );
		// time slot
		void genRequests( int );
		void cleanRequests( int );
		// time window
		void genDistribution();
		void showDistribution( int t=0 );
		void cleanDistribution();
		// algorithm
		void reboot();
		void newAlgo(); // tradeoff between replication and transmission cost
		void algorithm();
		
		
		bool feasibility( int, int, int, int ); // check for server capacity, power threshold and (max_requests)
		// helper function of costCal
		Ans mcmf(_edges&, int, int, std::vector< std::vector<int> >&, int);
		
		int costCal(int, int, int, int, std::vector< std::vector<int> >&);
		int costCalReplication(int, int, int, std::vector< std::vector<bool> >&, std::vector< std::vector<int> >&, int& );
		
	private:
		int time_window;
		int power_threshold;
		double power;
	
		int total_servers;
		std::vector<Server> servers;
		
		int total_edges;
		std::vector<Edge> edges;
		
		int total_users;
		std::vector<User> users;
		
		int total_apps;
		std::vector<App> apps;
		
		std::vector< std::vector< std::vector<int> > > distribution; // servers * apps * time_window
};

#endif