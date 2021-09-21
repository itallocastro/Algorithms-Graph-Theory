#include <bits/stdc++.h>
#include <queue>
#include <limits.h>

using namespace std;

class graph {
    public:
        int ** adj_matrix;
        int nvertex;
        queue<int> q;


        graph(int vertex, int edges) {
            nvertex = vertex;
            adj_matrix = (int **) malloc((vertex + 1) * sizeof(int *));
            for (int i = 0; i < vertex + 1; i++) {
                adj_matrix[i] = (int *) malloc((vertex + 1) * sizeof(int));
                for (int j = 0; j < vertex + 1; j++) {
                    adj_matrix[i][j] = 0;
                }
            }
        }

        void print_adjacency() {
            for (int i = 0; i < nvertex + 1; i++) {
                for (int j = 0; j < nvertex + 1; j++) {
                    cout << adj_matrix[i][j] << " ";
                }
                cout << endl;
            }
        }

        void create_edge(int vertex1, int vertex2, int cost) {
            adj_matrix[vertex1][vertex2] = cost != 0 ? cost : 1;
        }

        bool bfs(int source, int end, int parent[], int * rgraph){
            bool visited[nvertex+1];
            memset(visited, 0, sizeof(visited));

            visited[source] = true;
            q.push(source);
            parent[source] = -1;

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int i = 0; i < nvertex + 1; i++) {
                    if (!visited[i] && rgraph[(u * (nvertex+1)) + i] != 0) {
                        parent[i] = u;
                        if (i == end) {
                            return true;
                        }
                        visited[i] = 1;
                        q.push(i);

                    }
                }
            }
            return false;
        }
        int ford_fulkerson(int s, int t) {
            int residual_graph[nvertex+1][nvertex+1];
            int u, v;

            for (u = 0; u < nvertex+1; u++) {
                for (v = 0; v < nvertex+1; v++) {
                    residual_graph[u][v] = adj_matrix[u][v];
                }
            }

            int parent[nvertex+1];
            int max_flow = 0;

            while (bfs(s, t, parent, &residual_graph[0][0])) {
                int path_flow = INT_MAX;

                for (v = t; v != s; v = parent[v]) {
                    u = parent[v];
                    path_flow = min(path_flow, residual_graph[u][v]);
                }

                for (v = t; v != s; v = parent[v]) {
                    u = parent[v];
                    residual_graph[u][v] -= path_flow;
                    residual_graph[v][u] += path_flow;
                }

                max_flow += path_flow;
            }
            return max_flow;
        }
};


int * read_adjacency(string input) {
    int * adjacency = (int *) calloc(3, sizeof(int));

    char number[100];

    int j = 0, w = 0;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] != ' ') {
            number[j] = input[i];
            j++;
        } else {
            number[j] = '\0';
            adjacency[w] = atoi(number);
            w++;
            j = 0;  
        }
    }
    adjacency[w] = atoi(number);

    return adjacency;
}

int main() {
    int vertex, edges;
    string input;
    cin >> vertex;
    cin >> edges;
    
    getchar();

    graph graph(vertex, edges);

    for (int i = 0; i < edges; i++) {
        getline(cin, input);
        cout << input << endl;
        int * adjacency = read_adjacency(input);
        graph.create_edge(adjacency[0], adjacency[1], adjacency[2]);
        free(adjacency);
    }
    cout << graph.ford_fulkerson(0, 5);
}