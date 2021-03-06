#include <bits/stdc++.h>
#include <queue>
#include <limits.h>

using namespace std;

class graph
{
public:
    int **adj_matrix;
    int nvertex;
    queue<int> q;

    graph(int vertex, int edges)
    {
        nvertex = vertex;
        adj_matrix = (int **)malloc((vertex + 1) * sizeof(int *));
        for (int i = 0; i < vertex + 1; i++)
        {
            adj_matrix[i] = (int *)malloc((vertex + 1) * sizeof(int));
            for (int j = 0; j < vertex + 1; j++)
            {
                adj_matrix[i][j] = 0;
            }
        }
    }

    void print_adjacency()
    {
        for (int i = 0; i < nvertex + 1; i++)
        {
            for (int j = 0; j < nvertex + 1; j++)
            {
                cout << adj_matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    void create_edge(int vertex1, int vertex2, int cost)
    {
        adj_matrix[vertex1][vertex2] = cost != 0 ? cost : 1;
    }

    bool bfs(int source, int end, int parent[], int *rgraph)
    {
        bool visited[nvertex + 1];
        memset(visited, 0, sizeof(visited));

        visited[source] = true;
        q.push(source);
        parent[source] = -1;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            for (int i = 0; i < nvertex + 1; i++)
            {
                if (!visited[i] && rgraph[(u * (nvertex + 1)) + i] != 0)
                {
                    parent[i] = u;
                    if (i == end)
                    {
                        return true;
                    }
                    visited[i] = 1;
                    q.push(i);
                }
            }
        }
        return false;
    }
    int ford_fulkerson(int s, int t)
    {
        int residual_graph[nvertex + 1][nvertex + 1];
        int u, v;

        for (u = 0; u < nvertex + 1; u++)
        {
            for (v = 0; v < nvertex + 1; v++)
            {
                residual_graph[u][v] = adj_matrix[u][v];
            }
        }

        int parent[nvertex + 1];
        int max_flow = 0;

        while (bfs(s, t, parent, &residual_graph[0][0]))
        {
            int path_flow = INT_MAX;

            for (v = t; v != s; v = parent[v])
            {
                u = parent[v];
                path_flow = min(path_flow, residual_graph[u][v]);
            }

            for (v = t; v != s; v = parent[v])
            {
                u = parent[v];
                residual_graph[u][v] -= path_flow;
                residual_graph[v][u] += path_flow;
            }

            max_flow += path_flow;
        }
        return max_flow;
    }
};

int *read_adjacency(string input)
{
    int *adjacency = (int *)calloc(3, sizeof(int));

    char number[100];

    int j = 0, w = 0;
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] != ' ')
        {
            number[j] = input[i];
            j++;
        }
        else
        {
            number[j] = '\0';
            adjacency[w] = atoi(number);
            w++;
            j = 0;
        }
    }
    adjacency[w] = atoi(number);

    return adjacency;
}

void print_menu()
{
    cout << "-h : mostra o help" << endl;
    cout << "-o <arquivo> : redireciona a saida para o ??????arquivo??????" << endl;
    cout << "-f <arquivo> : indica o ??????arquivo?????? que cont??m o grafo de entrada" << endl;
    cout << "-i : v??rtice inicial (dependendo do algoritmo)" << endl;
    cout << "-l : v??rtice final (dependendo do algoritmo)" << endl;
}
int main(int argc, char *argv[])
{
    int vertex, edges, answer;
    string input, file_name;
    int begin = -1, end = -1;
    FILE *fr = NULL, *fw = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strcasecmp(argv[i], "-i") == 0)
        {
            if (i + 1 >= argc)
            {
                cout << "Entrada inv??lida!" << endl;
                print_menu();
                return 0;
            }
            begin = atoi(argv[i + 1]);

            if (begin == 0 && strcasecmp(argv[i + 1], "0") != 0)
            {
                cout << "Entrada inv??lida!" << endl;
                print_menu();
                return 0;
            }
        }
        else if (strcasecmp(argv[i], "-l") == 0)
        {
            if (i + 1 >= argc)
            {
                cout << "Entrada inv??lida!" << endl;
                print_menu();
                return 0;
            }
            end = atoi(argv[i + 1]);

            if (end == 0 && strcasecmp(argv[i + 1], "0") != 0)
            {
                cout << "Entrada inv??lida!" << endl;
                print_menu();
                return 0;
            }
        }
        else if (strcasecmp(argv[i], "-f") == 0)
        {
            if (i + 1 >= argc)
            {
                cout << "Entrada inv??lida!" << endl;
                print_menu();
                return 0;
            }

            fr = fopen(argv[i + 1], "r");
            if (fr == NULL)
            {
                cout << "Arquivo n??o encontrado!" << endl;
                return 0;
            }
        }
        else if (strcasecmp(argv[i], "-o") == 0)
        {
            if (i + 1 >= argc)
            {
                cout << "Entrada inv??lida!" << endl;
                print_menu();
                return 0;
            }
            fw = fopen(argv[i + 1], "w+");
        }
        else if (strcasecmp(argv[i], "-h") == 0)
        {
            print_menu();
            return 0;
        }
    }

    if (begin < 0 || end < 0)
    {
        cout << "Voc?? precisa informar os v??rtices de in??cio e fim!" << endl;
        return 0;
    }

    if (fr == NULL)
    {
        cin >> vertex;
        cin >> edges;

        getchar();

        graph graph(vertex, edges);

        for (int i = 0; i < edges; i++)
        {
            getline(cin, input);
            int *adjacency = read_adjacency(input);
            graph.create_edge(adjacency[0], adjacency[1], adjacency[2]);
            free(adjacency);
        }

        answer = graph.ford_fulkerson(begin, end);
        if (fw != NULL)
        {
            fprintf(fw, "%d", answer);
            fclose(fw);
        }
        else
        {
            cout << answer;
        }
    }
    else
    {
        char input[255];
        fscanf(fr, "%d", &vertex);
        fscanf(fr, "%d ", &edges);
        graph graph(vertex, edges);

        while (fscanf(fr, "%[^\n]%*c", input) != EOF)
        {
            int *adjacency = read_adjacency(input);
            graph.create_edge(adjacency[0], adjacency[1], adjacency[2]);
            free(adjacency);
        }
        fclose(fr);
        answer = graph.ford_fulkerson(begin, end);

        if (fw != NULL)
        {
            fprintf(fw, "%d", answer);
            fclose(fw);
        }
        else
        {
            cout << answer;
        }
    }
}