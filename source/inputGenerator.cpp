#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
void makeServerInput(int num_server, int num_link, int num_user, int num_apps, bool is_ranged, int server_comp, int server_stor){
	FILE* s = fopen("../input/Server.txt", "w");
    FILE* u = fopen("../input/Users.txt", "w");
    FILE* a = fopen("../input/Applications.txt", "w");
    fprintf(s, "%d %d %d %d\n\n", num_server, num_link, num_user, num_apps);
    fprintf(u, "%d %d %d %d\n\n", num_server, num_link, num_user, num_apps);
    fprintf(a, "%d %d %d %d\n\n", num_server, num_link, num_user, num_apps);

    // int users[num_user];
    std::vector<int> users;
    // int loc[num_user];
    std::vector<int> loc;
    // int repli[num_apps];
    std::vector<int> repli;
    // bool rep_ar[num_server][num_apps]; 
    std::vector< std::vector<bool> > rep_ar;
    // int apps[num_apps];
    std::vector<int> apps;
    int res[3] = {1,2,4};
    std::vector< std::vector<int> > user_dis;

    users.resize(num_user);
    loc.resize(num_user);
    apps.resize(num_apps);
    repli.resize(num_apps);
    user_dis.resize(num_server);
    rep_ar.resize(num_server);
    for(int i = 0;i < num_server;++i){
        rep_ar[i].resize(num_apps);
    }

    for(int i = 0;i < num_user;++i){
    	users[i] = i;
        loc[i] = -1;
    }

    for(int i = 0;i < num_apps;++i){
    	apps[i] = i;
        repli[i] = 0;
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

    for(int i = 0;i < num_server;++i){
        for(int j = 0;j < num_apps;++j){
            rep_ar[i][j] = false;
        }
    }

    for(int i = 0;i < num_apps;++i){
        int r = rand() % num_server;
        rep_ar[r][i] = true;
        repli[i] += 1;
    }

    for(int i = 0;i < num_user;++i){
        int r = rand() % num_server;
        user_dis[r].push_back(users[i]);
        loc[i] = r;
    }
    int i;
    // for(int i = 0;i < num_server;++i){
    //     for(int j = 0;j < user_dis[i].size();++j)
    //         printf("%d ", user_dis[i][j]);
    //     printf("\n");
    // }
    // scanf("%d", &i);
	for(i = 0;i < num_server;++i){
		int r_a = rand() % num_apps;
		if(is_ranged){  // reserved for further modification
			fprintf(s, "%d %d %d %d\n", server_comp , server_stor, (int)user_dis[i].size(), r_a);
		}
		else{
			fprintf(s, "%d %d %d %d\n", server_comp, server_stor, (int)user_dis[i].size(), r_a);
		}
		
        for (int j = 0;j < num_apps;++j) {
            int r = rand() % num_apps;
            int t = apps[j]; 
            apps[j] = apps[r]; 
            apps[r] = t;
        }
        if(r_a == 0){
            fprintf(s, "\n");
        }
        for(int j = 0;j < r_a;++j){
            fprintf(s, "%d ", apps[j]);
            if(!rep_ar[i][apps[j]]){
                repli[apps[j]] += 1;
                rep_ar[i][apps[j]] = true;
            }
            if(j == r_a-1){ 
               fprintf(s, "\n");
            }
        }
        if((int)user_dis[i].size() == 0){
            fprintf(s, "\n");
        }

    	for(int j = 0;j < (int)user_dis[i].size();++j){
    		fprintf(s, "%d ", user_dis[i][j]);
            if(j == (int)user_dis[i].size()-1)
                fprintf(s, "\n\n");
        }
	}
    fprintf(s, "[NUM_SERVER] [NUM_LINK] [NUM_USER] [NUM_APPS]\n[APP_INDEX]\n[USER_INDEX]\n");
	fclose(s);

    for(i = 0;i < num_user;++i){
        int n = 0;
        while(n == 0){
            n = rand() % num_apps;
        }
        //printf("%d\n", n);
        for(int j = 1;j < num_apps;++j) {
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

    fprintf(u, "[NUM_SERVER] [NUM_LINK] [NUM_USER] [NUM_APPS]\n[LOCATION] [NUM_APPS]\n[APP_INDEX]\n");
    fclose(u);
    for(int i = 0;i < num_apps;++i){
        int r = rand() % 3;
        fprintf(a, "%d %d %d %d ", res[r], res[r], rand()%50+50, repli[i]);
        fprintf(a, "[ ");
        for(int j = 0;j < num_server;++j)
            if(rep_ar[j][i])
                fprintf(a, "%d ", j);
        fprintf(a, "]\n");    

    }

    fprintf(a, "\n[TOTAL_SERVERS] [TOTAL_EDGES] [TOTAL_USERS] [TOTAL_APPS]\n[COMP_REQ] [MEMO_REQ] [BAND_REQ] [NUM_REPLICA] [ARR_EXIST_SERVER]\n");
}
void makeEdgeInput(int num_server, int num_link, int num_user, int num_apps, bool weighted, int minw, int maxw){
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
    FILE* f = fopen("../input/Edge.txt", "w");
    fprintf(f, "%d %d %d %d\n", num_server, num_link, num_user, num_apps);
    // for(int k = 1; k <= num_link;++k){
    //     printf("%d %d %d\n", weight[k], nodei[k]-1, nodej[k]-1);
    // }
    for(int k = 1; k <= num_link;++k){
    	fprintf(f, "%d %d %d\n", weight[k], nodei[k]-1, nodej[k]-1);
    }
    fprintf(f, "\n[NUM_SERVER] [NUM_LINK] [NUM_USER] [NUM_APPS]\n[BANDWIDTH] [NODEA] [NODEB]\n");
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
	if(args < 9){
		printf("usage ./inputGenerator num_server num_link num_user num_apps is_weighted min_link max_link server_comp server_stor\n");
		return -1;
	}
	
	int num_server, num_link, num_user, num_apps, server_comp, server_stor;
	int minw = 0, maxw = 0;
	bool weighted = false;
	if(atoi(argv[5]) == 1){
		weighted = true;
		minw = atoi(argv[6]);
		maxw = atoi(argv[7]);
	}
	num_server = atoi(argv[1]);
	num_link = atoi(argv[2]);
	num_user = atoi(argv[3]);
	num_apps = atoi(argv[4]);
	server_comp = atoi(argv[8]);
	server_stor = atoi(argv[9]);
    printf("!\n");
	// check valid input of num_server and num_link
	if(num_link < (num_server - 1) || num_link > (num_server * (num_server - 1) / 2)){
		printf("invalid input of m\n");
		return -1;
	}
	srand(time(NULL));

	makeEdgeInput(num_server, num_link, num_user, num_apps, weighted, minw, maxw);
    printf("!!\n");
	makeServerInput(num_server, num_link, num_user, num_apps, false, server_comp, server_stor);


}