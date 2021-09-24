#include <bits/stdc++.h>
#include <queue>
#include <limits.h>

using namespace std;

typedef struct st
{
    int vertex;
    int key;
} st;

struct comparator
{
    bool operator()(st arg1, st arg2)
    {
        return arg1.key > arg2.key;
    }
};

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
        adj_matrix[vertex2][vertex1] = cost != 0 ? cost : 1;
    }

    bool is_element_in_pq(priority_queue<st, vector<st>, comparator> pq, int element)
    {
        priority_queue<st, vector<st>, comparator> g = pq;
        while (!g.empty())
        {
            if (g.top().vertex == element)
                return true;
            g.pop();
        }
        return false;
    }
    priority_queue<st, vector<st>, comparator> update_queue(priority_queue<st, vector<st>, comparator> pq, int vertex, int key)
    {
        priority_queue<st, vector<st>, comparator> new_pq, g = pq;

        while (!g.empty())
        {
            st element = g.top();
            g.pop();
            if (element.vertex == vertex)
            {
                element.key = key;
            }
            new_pq.push(element);
        }
        return new_pq;
    }

    int *prim(int begin)
    {
        priority_queue<st, vector<st>, comparator> pq;
        int key[nvertex + 1], visited[nvertex + 1];
        int *pred = (int *)malloc(sizeof(int) * (nvertex + 1));

        for (int i = 1; i < nvertex + 1; i++)
        {
            st element;
            key[i] = i == begin ? 0 : INT_MAX;
            pred[i] = -1;
            element.key = i == begin ? 0 : INT_MAX;
            element.vertex = i;
            pq.push(element);
        }

        pred[begin] = -1;

        while (!pq.empty())
        {
            st u = pq.top();
            pq.pop();

            for (int i = 1; i < nvertex + 1; i++)
            {
                if (adj_matrix[u.vertex][i] != 0)
                {
                    int adj_cost = adj_matrix[u.vertex][i];
                    if (is_element_in_pq(pq, i) && adj_cost < key[i])
                    {
                        pred[i] = u.vertex;
                        key[i] = adj_cost;
                        pq = update_queue(pq, i, adj_cost);
                    }
                }
            }
        }

        return pred;
    }
};

void make_tree(int *pred, graph graph, int nvertex, FILE *fw)
{
    for (int i = 1; i < nvertex + 1; i++)
    {
        for (int j = 1; j < nvertex + 1; j++)
        {

            if (pred[j] == i)
            {
                if (fw == NULL)
                {
                    printf("(%d,%d) ", i, j);
                }
                else
                {
                    fprintf(fw, "(%d,%d) ", i, j);
                }
            }
        }
    }
}

void tree_size(int *pred, graph graph, int nvertex, FILE *fw)
{
    int size = 0;

    for (int i = 1; i < nvertex + 1; i++)
    {
        if (pred[i] != -1)
        {
            size += graph.adj_matrix[pred[i]][i];
        }
    }
    if (fw == NULL)
    {
        cout << size << endl;
    }
    else
    {
        fprintf(fw, "%d", size);
    }
}
void print_menu()
{
    cout << "-h : mostra o help" << endl;
    cout << "-o <arquivo> : redireciona a saida para o ‘‘arquivo’’" << endl;
    cout << "-f <arquivo> : indica o ‘‘arquivo’’ que contém o grafo de entrada" << endl;
    cout << "-s mostra a solução em ordem crescente" << endl;
    cout << "-i : vértice inicial (dependendo do algoritmo)" << endl;
}

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

int main(int argc, char *argv[])
{
    int vertex, edges, answer;
    string input;
    int begin = -1;
    bool order = false;
    FILE *fr = NULL, *fw = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strcasecmp(argv[i], "-i") == 0)
        {
            if (i + 1 >= argc)
            {
                cout << "Entrada inválida!" << endl;
                print_menu();
                return 0;
            }
            begin = atoi(argv[i + 1]);

            if (begin == 0 && strcasecmp(argv[i + 1], "0") != 0)
            {
                cout << "Entrada inválida!" << endl;
                print_menu();
                return 0;
            }
        }
        else if (strcasecmp(argv[i], "-f") == 0)
        {
            if (i + 1 >= argc)
            {
                cout << "Entrada inválida!" << endl;
                print_menu();
                return 0;
            }

            fr = fopen(argv[i + 1], "r");
            if (fr == NULL)
            {
                cout << "Arquivo não encontrado!" << endl;
                return 0;
            }
        }
        else if (strcasecmp(argv[i], "-o") == 0)
        {
            if (i + 1 >= argc)
            {
                cout << "Entrada inválida!" << endl;
                print_menu();
                return 0;
            }
            cout << argv[i + 1] << endl;
            fw = fopen(argv[i + 1], "w+");
        }
        else if (strcasecmp(argv[i], "-h") == 0)
        {
            print_menu();
            return 0;
        }
        else if (strcasecmp(argv[i], "-s") == 0)
        {
            order = true;
        }
    }

    if (begin < 0)
    {
        cout << "Você precisa informar o vértice inicial!" << endl;
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

        int *pred = graph.prim(begin);
        if (order)
        {
            make_tree(pred, graph, vertex, fw);
        }
        else
        {
            tree_size(pred, graph, vertex, fw);
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
        int *pred = graph.prim(begin);
        if (order)
        {
            make_tree(pred, graph, vertex, fw);
        }
        else
        {
            tree_size(pred, graph, vertex, fw);
        }
    }
}