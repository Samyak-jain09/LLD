#include <bits/stdc++.h>
using namespace std;

vector<int> dijkstra(int V, vector<vector<int>> edges, int S) {
        vector<vector<pair<int,int>>> adj(V);
        for(int i = 0 ; i<edges.size() ; ++i){
            adj[edges[i][0]].push_back({edges[i][1],edges[i][2]});
            adj[edges[i][1]].push_back({edges[i][0],edges[i][2]});
        }
        priority_queue<pair<int,int> , vector<pair<int,int>> , greater<pair<int,int>> > pq;
        pq.push({0,S});
        vector<int> dist(V,1e9);
        dist[S] = 0;
        while(!pq.empty()){
            int node = pq.top().second;
            int dis = pq.top().first;
            pq.pop();
            for(auto n : adj[node]){
                if(dis+n.second<dist[n.first]){
                    dist[n.first] = n.second + dis;
                    pq.push({dist[n.first],n.first});
                }
            }
        }
        return dist;
    }

     vector<int> bellman_ford(int V, vector<vector<int>>& edges, int S) {
        const int INF = 1e9;

        vector<int> dist(V, INF);
        dist[S] = 0;

        for (int i = 0; i < V - 1; ++i) {
            bool changed = false;

            for (auto &edge : edges) {
                int u = edge[0];
                int v = edge[1];
                int wt = edge[2];

                if (dist[u] != INF && dist[u] + wt < dist[v]) {
                    dist[v] = dist[u] + wt;
                    changed = true;
                }
            }

            if (!changed)
                break;
        }

        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int wt = edge[2];

            if (dist[u] != INF && dist[u] + wt < dist[v]) {
                return {-1};
            }
        }

        return dist;
    }

    void floydWarshall(vector<vector<int>>& matrix) {
        int n = matrix.size();
        const int INF = 1e9;

        // Convert -1 to INF
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == -1)
                    matrix[i][j] = INF;
            }
            matrix[i][i] = 0;
        }

        // Floyd-Warshall
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (matrix[i][k] != INF && matrix[k][j] != INF) {
                        matrix[i][j] = min(
                            matrix[i][j],
                            matrix[i][k] + matrix[k][j]
                        );
                    }
                }
            }
        }

        // Convert INF back to -1
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == INF)
                    matrix[i][j] = -1;
            }
        }
    }

    int prim_algorithm(int V, vector<vector<int>> adj[]) {
       priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>> > pq;
       pq.push({0,0});
       int sm = 0;
       vector<int> visited(V,0);
       while(!pq.empty()){
        int node = pq.top().second;
        int dist = pq.top().first;
        pq.pop();
        if(visited[node] == 1){
            continue;
        }
        visited[node] = 1;
        sm+=dist;
        for(auto it : adj[node]){
            int n = it[0];
            int wt = it[1];
            if(!visited[n]){
                pq.push({wt,n});
            }
        }
       }
       return sm;
    }