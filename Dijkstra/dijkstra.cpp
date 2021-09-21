#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
#include <vector>
#include<queue>
struct dist {
    int vertex, dist_vertex;
};
struct compare
{
    bool operator()(const dist & a, const dist & b) {
        return a.dist_vertex > b.dist_vertex;
    }
};
priority_queue<dist, vector<dist>, compare> generate_priority_queue(int length, vector<int> dist_in_vertex) {
    priority_queue<dist, vector<dist>, compare> priority;
    dist dist1;
    for(int i = 0; i < length; i++) {
        dist1.vertex = i;
        dist1.dist_vertex = dist_in_vertex[i];
        priority.push(dist1);
    }
    return priority;
}

priority_queue<dist, vector<dist>, compare> update_priority_queue(vector<int> list_vertex, vector<int> dist_in_vertex) {
    priority_queue<dist, vector<dist>, compare> priority;
    dist dist1;
    for(int i = 0; i < list_vertex.size(); i++) {
        dist1.vertex = list_vertex[i];
        dist1.dist_vertex = dist_in_vertex[list_vertex[i]];
        priority.push(dist1);
    }
    return priority;
}
vector<int> get_list_vertex( priority_queue<dist, vector<dist>, compare> priority) {
    vector<int> list_index(priority.size());
    while(!priority.empty()) {
        dist i = priority.top();
        priority.pop();
        list_index.push_back(i.vertex);
    }
    return list_index;
}
vector<int> dijkstra(int length,int matrix[][1000], int initial_vertex) {
    vector<int> prev(length, -1);
    vector<int> dist_in_vertex(length, 10000000);
    dist_in_vertex[initial_vertex] = 0;
    priority_queue<dist, vector<dist>, compare> priority = generate_priority_queue(length, dist_in_vertex);
    
    while(!priority.empty()) {
        dist min_priority = priority.top();
        priority.pop();
        for(int i = 0; i < length; i++) {
            if(matrix[min_priority.vertex][i] != -1) {
                if(dist_in_vertex[i] > dist_in_vertex[min_priority.vertex] + matrix[min_priority.vertex][i]) {
                    dist_in_vertex[i] = dist_in_vertex[min_priority.vertex] + matrix[min_priority.vertex][i];
                    prev[i] = min_priority.vertex;
                    priority = update_priority_queue(get_list_vertex(priority), dist_in_vertex);
                }
            }
        }
    }
    
    return dist_in_vertex;
}
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
void help() {
    cout << "-o <arquivo> Redireciona a saída para um arquivo específico" << endl;
    cout << "-f <arquivo> Indica o arquivo que contém o grafo de entrada" << endl;
    cout << "-s Mostra a solução (em ordem crescente)" << endl;
    cout << "-i Vértice inicial" << endl;
    cout << "-l Vértice final" << endl;
    return;
}
int main(int argc,char **argv) {
    int v, e, v1, v2, weight, initial, last = -1;
    string write;
    ifstream my_input;
    for (int i = 0; i < argc; i++) {
        string arg = (string) argv[i];
        if(arg.find("-h") != string::npos) {
            help();
        } 
        if(arg.find("-o") != string::npos) {
            write = (string) argv[++i];
        }
        if(arg.find("-l") != string::npos) {
            last = atoi(argv[++i]);
        }
        if(arg.find("-i") != string::npos) {
            initial = atoi(argv[++i]);
        }
        if(arg.find("-f") != string::npos) {
            string first_line;
            my_input.open(argv[++i]);
            getline(my_input, first_line);
            int *first = read_adjacency(first_line);
            v = first[0];
            e = first[1];
        }
    }
    string input;

    int adj_matrix[v+1][1000];
    
    for(int i = 0; i < v+1; i++) {
        for(int j = 0; j < v+1; j++) {
            adj_matrix[i][j] = -1;
        }
    }
    for(int i = 0; i < e; i++) {
        getline(my_input, input);
        int * adjacency = read_adjacency(input);
    
        adj_matrix[adjacency[0]][adjacency[1]] = adjacency[2] != 0 ? adjacency[2] : 1;
        free(adjacency);
    }
  
    vector<int> dist = dijkstra(v+1, adj_matrix, initial);
    ofstream outdata;
    if(write.length() > 0) {
        outdata.open(write);
        if(!outdata) {
            cerr << "Erro: Não possível abrir o arquivo" << endl;
            return 0;
        }
    }
    if(last == -1) {
        cout << "Calculando a distância do vértice " << initial << " para os demais..." << endl;
        for(int i = 1; i < dist.size(); i++) {
            cout << "Vértice: " << i << " Distância: " << dist[i] << endl;
            outdata << "Vértice: " << i << " Distância: " << dist[i] << endl;
        }
    } else {
        cout << "Calculando a distância do vértice " << initial << " para o vértice " << last << endl;
        cout << dist[last] << endl;
        outdata << dist[last] << endl;
    }
}