#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
typedef struct
{
	int stor, comp, bandwidth, repli, max_r;
}Apps;
typedef struct 
{
	int stor, comp;
}Servers;
void makeServerInput(int time_window, int num_server, int num_link, int num_user, int num_apps, bool is_ranged, int server_comp, int server_stor){
	FILE* s = fopen("../input/Servers.txt", "w");
	FILE* u = fopen("../input/Users.txt", "w");
	FILE* a = fopen("../input/Applications.txt", "w");
	fprintf(s, "%d %d %d %d %d\n\n", time_window, num_server, num_link, num_user, num_apps);
	fprintf(u, "%d %d %d %d %d\n\n", time_window, num_server, num_link, num_user, num_apps);
	fprintf(a, "%d %d %d %d %d\n\n", time_window, num_server, num_link, num_user, num_apps);
  
	std::vector<int> users;
	std::vector<int> apps;
	std::vector<int> loc; // user location
	std::vector< std::vector<bool> > app_dis;
	int res[3] = {1,2,4}; // tend to change
	std::vector< std::vector<int> > user_dis;
	std::vector<Apps> applications;
	std::vector<Servers> servers;

	users.resize(num_user);
	loc.resize(num_user);
	apps.resize(num_apps);
	user_dis.resize(num_server);
	app_dis.resize(num_server);
	applications.resize(num_apps);
	servers.resize(num_server);
	
	std::vector< std::vector<int> > serving; // which server j serving for application k from server i
	serving.resize( num_server, std::vector<int>( num_apps, -1 ) );
	
	for(int i = 0;i < num_server;++i){
		app_dis[i].resize(num_apps);
		servers[i].stor = server_stor;
		servers[i].comp = server_comp;
	}
	for(int i = 0;i < num_server;++i){
		for(int j = 0;j < num_apps;++j){
			app_dis[i][j] = false;
		}
	}

	for(int i = 0;i < num_user;++i){
		users[i] = i;
		loc[i] = -1;
	}

	for(int i = 0;i < num_apps;++i){
		apps[i] = i;
		applications[i].stor = res[rand()%3];
		applications[i].comp = res[rand()%3];
		applications[i].bandwidth = rand()%50+50;
		applications[i].repli = 0;
		applications[i].max_r = 10;
	}

	srand(time(NULL));
	for (int i = 0;i < num_user;++i) {
			int r = rand() % num_user ;
			int t = users[i]; 
			users[i] = users[r]; 
			users[r] = t;
	}
	for (int i = 0;i < num_apps;++i) {
			int r = rand() % num_apps;
			int t = apps[i]; 
			apps[i] = apps[r]; 
			apps[r] = t;
	}

	// put 1 copy of each app to random server.
	for(int i = 0;i < num_apps;++i){
		while(1){
			int r = rand() % num_server;
			if(servers[r].stor >= applications[i].stor && servers[r].comp >= applications[i].comp){
				app_dis[r][i] = true;
				applications[i].repli += 1;
				servers[r].stor -= applications[i].stor;
				servers[r].comp -= applications[i].comp;
				
				// serving: fix to find the nearest replica of application k from each server i
				for ( int j=0; j<num_server; j++ ) serving[j][i] = r;
				break;
			}
		}
	}

	for(int i = 0;i < num_user;++i){
		int r = rand() % num_server;
		user_dis[r].push_back(users[i]);
		loc[i] = r;
	}
	int i;

	for(i = 0;i < num_server;++i){
		
		for (int j = 0;j < num_apps;++j) {
			int r = rand() % num_apps;
			int t = apps[j]; 
			apps[j] = apps[r]; 
			apps[r] = t;
		}
		
		// need consider the comp , storage and power of the server 
		for(int j = 0;j < num_apps;++j){
			int r = rand() % 2;
			if(r == 0) continue;
			if(!app_dis[i][apps[j]] && servers[i].comp >= applications[apps[j]].comp && servers[i].stor >= applications[apps[j]].stor){
				applications[apps[j]].repli += 1;
				app_dis[i][apps[j]] = true;
				servers[i].comp -= applications[apps[j]].comp;
				servers[i].stor -= applications[apps[j]].stor;
			}
		}

		int r_a = 0;
		for(int j = 0;j < num_apps;++j){
			if(app_dis[i][j])
				r_a++;
		}
		if(is_ranged){  // reserved for further modification
			fprintf(s, "%d %d %d %d\n", server_comp, server_stor, r_a, (int)user_dis[i].size());
		}
		else{
			fprintf(s, "%d %d %d %d\n", server_comp, server_stor, r_a, (int)user_dis[i].size());
		}
		for(int j = 0;j < num_apps;++j){
			if(app_dis[i][j])
				fprintf(s, "%d ", j);
		}
		fprintf(s, "\n");

		// if((int)user_dis[i].size() == 0){
		// 	fprintf(s, "\n");
		// }

		for(int j = 0;j < (int)user_dis[i].size();++j){
			fprintf(s, "%d ", user_dis[i][j]);
		}
		fprintf(s, "\n");
		for ( int j=0; j<num_apps; j++ ) fprintf( s, "%d ", serving[i][j] );
		fprintf( s, "\n" );

	}
	fprintf(s, "\n[TIME_WINDOW] [NUM_SERVER] [NUM_LINK] [NUM_USER] [NUM_APPS]\n[COMP] [STOR] [USER] [APP]\n[APP_INDEX]\n[USER_INDEX]\n[SERVING]\n");
	fclose(s);

	for(i = 0;i < num_user;++i){
		int n = 0;
		while(n == 0){
			n = rand() % num_apps;
		}
		//printf("%d\n", n);
		for(int j = 0;j < num_apps;++j) {
			int r = rand() % num_apps;
			int t = apps[j]; 
			apps[j] = apps[r]; 
			apps[r] = t;
		}
		//printf("%d\n", loc[i]);
		fprintf(u, "%d %d\n", loc[i], n);
		
		for(int k = 0;k < n;++k){
			fprintf(u, "%d ", apps[k]);
			// printf("%d ", apps[k]);
		}
		fprintf(u, "\n\n");
	}

	fprintf(u, "[TIME_WINDOW] [NUM_SERVER] [NUM_LINK] [NUM_USER] [NUM_APPS]\n[LOCATION] [NUM_APPS]\n[APP_INDEX]\n");
	fclose(u);
	
	for(int i = 0;i < num_apps;++i){
		fprintf(a, "%d %d %d %d ", applications[i].comp, applications[i].stor, applications[i].bandwidth, applications[i].repli);
		// rand()%50+50 means the bandwidth req of app is now 50-100, tend to chage
		// fprintf(a, "[ ");
		for(int j = 0;j < num_server;++j)
			if(app_dis[j][i])
				fprintf(a, "%d ", j);
		fprintf(a, "%d\n", applications[i].max_r );
	}

	fprintf(a, "\n[TIME_WINDOW] [TOTAL_SERVERS] [TOTAL_EDGES] [TOTAL_USERS] [TOTAL_APPS]\n[COMP_REQ] [MEMO_REQ] [BAND_REQ] [NUM_REPLICA] [ARR_EXIST_SERVER] [MAX_REQUESTS]\n");
}
void makeEdgeInput(int time_window, int num_server, int num_link, int num_user, int num_apps, bool weighted, int minw, int maxw){
	// make a random connected graph with given node egde and weighted rand (if)
	int maxEdges, nodeA, nodeB, numEdges, temp;
	// int permute[num_server+1];
	std::vector<int> permute;
	// bool adjcent[num_server+1][num_server+1];
	std::vector< std::vector<bool> > adjcent;
	// int nodei[num_link+1], nodej[num_link+1];
	std::vector<int> nodei;
	std::vector<int> nodej;
	// int weight[num_link+1];
	std::vector<int> weight;

	// initialize 
	permute.resize(num_server+1);
	adjcent.resize(num_server+1);
	nodei.resize(num_link+1);
	nodej.resize(num_link+1);
	weight.resize(num_link+1);
	for(nodeA = 0;nodeA < num_server+1;++nodeA){
		adjcent[nodeA].resize(num_server+1);
		for(nodeB = 0;nodeB < num_server+1;++nodeB){
			adjcent[nodeA][nodeB] = false;
		}
	}
	// printf("!!!\n");

	for(int i = 0;i < num_server+1;++i){
		permute[i] = i;
	}
	for (int i = 1;i < num_server+1;++i) {
			int r = rand() % num_server + 1;
			int t = permute[i]; 
			permute[i] = permute[r]; 
			permute[r] = t;
	}
	for(int k = 0;k < num_link+1;++k){
		nodei[k] = -1;
		nodej[k] = -1;
		weight[k] = -1;
	}
	numEdges = 0;
	// generate a random spanning tree by the greedy method
	for(nodeA = 2;nodeA <= num_server;++nodeA){
		numEdges++;
		nodeB = rand() % (nodeA - 1) + 1;

		nodei[numEdges] = permute[nodeA];
		nodej[numEdges] = permute[nodeB];
		adjcent[permute[nodeA]][permute[nodeB]] = true;
		adjcent[permute[nodeB]][permute[nodeA]] = true;
		if(weighted){
			weight[numEdges] = minw + rand() % (maxw - minw + 1);
		}
	}
	
	// add remaining edges
	while(numEdges <= num_link){
		nodeA = rand() % num_server + 1;
		nodeB = rand() % num_server + 1;
		if(nodeA == nodeB) continue;
		if(nodeA > nodeB){
			temp = nodeA;
			nodeA = nodeB;
			nodeB = temp;
		}
		if(!(adjcent[nodeA][nodeB])){
			numEdges++;
			nodei[numEdges] = nodeA;
			nodej[numEdges] = nodeB;
			adjcent[nodeA][nodeB] = true;
			if(weighted){
				weight[numEdges] = minw + rand() % (maxw - minw + 1);
			}
		}
	}

	// write to file
	FILE* f = fopen("../input/Edges.txt", "w");
	fprintf(f, "%d %d %d %d %d\n", time_window, num_server, num_link, num_user, num_apps);
	// for(int k = 1; k <= num_link;++k){
	//     printf("%d %d %d\n", weight[k], nodei[k]-1, nodej[k]-1);
	// }
	for(int k = 1; k <= num_link;++k){
		fprintf(f, "%d %d %d\n", weight[k], nodei[k]-1, nodej[k]-1);
	}
	fprintf(f, "\n[TIME_WINDOW] [NUM_SERVER] [NUM_LINK] [NUM_USER] [NUM_APPS]\n[BANDWIDTH] [NODEA] [NODEB]\n");
	fclose(f);
}
int main(int args, char* argv[]){
	// usage ./inputGenerator num_server num_link num_user num_apps is_weighted \
	//                        min_link max_link server_comp server_stor
	// num_server: number of nodes on the graph
	// num_link:   number of edges on the graph
	//             n-1 <= num_link <= n*(n-1)/2
	// num_user:   number of users
	// num_apps:   number of application
	// is_weighted:1 if the graph is weighted, 0 otherwise
	// min_link, max_link: the minimum and the maximum of the weight


	// arguments check
	if(args < 10){
		printf("usage ./inputGenerator time_window num_server num_link num_user num_apps is_weighted min_link max_link server_comp server_stor\n");
		return -1;
	}
	
	int time_window, num_server, num_link, num_user, num_apps, server_comp, server_stor;
	int minw, maxw;
	bool weighted = false;
	
	time_window = atoi(argv[1]);
	num_server = atoi(argv[2]);
	num_link = atoi(argv[3]);
	num_user = atoi(argv[4]);
	num_apps = atoi(argv[5]);
	
	weighted = ( atoi( argv[6] ) ? true : false );
	minw = ( atoi( argv[6] ) ? atoi( argv[7] ) : 0 );
	maxw = ( atoi( argv[6] ) ? atoi( argv[8] ) : 0 );
	
	server_comp = atoi(argv[9]);
	server_stor = atoi(argv[10]);
	// printf("!\n");
	// check valid input of num_server and num_link
	if(num_link < (num_server - 1)){
		printf("%d %d\n", num_server, num_link);
		printf("invalid input of num_link\n");
		return -1;
	}
	if((long long)num_link > (long long)((long long)num_server * ((long long)num_server - 1) / 2)){
		printf("!%d %d\n", num_server, num_link);
		printf("invalid input of num_link\n");
		return -1;
	}
	srand(time(NULL));

	makeEdgeInput(time_window, num_server, num_link, num_user, num_apps, weighted, minw, maxw);
	// printf("!!\n");
	makeServerInput(time_window, num_server, num_link, num_user, num_apps, false, server_comp, server_stor);
}