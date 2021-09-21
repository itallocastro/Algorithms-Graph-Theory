#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
#include <vector>
#include<queue>
#include <list>

struct dist {
    int vertex, weigth;
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
void help() {
    cout << "-o <arquivo> Redireciona a saída para um arquivo específico" << endl;
    cout << "-f <arquivo> Indica o arquivo que contém o grafo de entrada" << endl;
    cout << "-s Mostra a solução (em ordem crescente)" << endl;
    cout << "-i Vértice inicial" << endl;
    cout << "-l Vértice final" << endl;
    return;
}
int* bellman_ford(int length,list<dist> adj_list[], int initial_vertex) {
    int *distance = (int *) calloc(length, sizeof(int));
    for(int i = 0; i < length; i++) {
        distance[i] = INT_MAX;
    }
    distance[initial_vertex] = 0;
    
    for(int l = 1; l < length-1; l++) {
        for(int k = 1; k < length; k++) {
            vector<dist> neighbor(adj_list[k].begin(), adj_list[k].end());
            for(int j=0; j<adj_list[k].size(); j++) {
                if(distance[k]!= INT_MAX && distance[k] + neighbor[j].weigth < distance[neighbor[j].vertex]) {
                    distance[neighbor[j].vertex] = distance[k] + neighbor[j].weigth;
                }
            }
        }
    }

    for(int k = 1; k < length; k++) {
            vector<dist> neighbor(adj_list[k].begin(), adj_list[k].end());
            for(int j=0; j<adj_list[k].size(); j++) {
                if(distance[k]!= INT_MAX && distance[k] + neighbor[j].weigth < distance[neighbor[j].vertex]) {
                    return NULL;
                }
            }
        }
    
    return distance;
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

    list<dist> adj_list[v+1];
    int adj_matrix[v+1][1000];
    
    for(int i = 0; i < v+1; i++) {
        for(int j = 0; j < v+1; j++) {
            adj_matrix[i][j] = -1;
        }
    }
    for(int i = 0; i < e; i++) {
        getline(my_input, input);
        int * adjacency = read_adjacency(input);
        dist target;
        target.vertex = adjacency[1];
        target.weigth = adjacency[2] != 0 ? adjacency[2] : 1;
        adj_list[adjacency[0]].push_back(target);
        free(adjacency);
    }
    
    int* distance = bellman_ford(v+1, adj_list, initial);
   
    if (distance == NULL) {
        cout << "O grafo possui um ciclo negativo!" << endl;
        return 0;
    }
    
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
        for(int i = 1; i < v+1; i++) {
            cout << "Vértice: " << i << " Distância: " << distance[i] << endl;
            outdata << "Vértice: " << i << " Distância: " << distance[i] << endl;
        }
    } else {
        cout << "Calculando a distância do vértice " << initial << " para o vértice " << last << endl;
        cout << distance[last] << endl;
        outdata << distance[last] << endl;
    }

    free(distance);
}