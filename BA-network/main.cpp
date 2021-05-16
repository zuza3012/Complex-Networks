#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <iostream>
#include <fstream>
using namespace std;

const int N = 10000; // maksimum size of network
int m0 = 100;
int m = 2;

struct Edge{
    int v1, v2;
};

std::array<int, N> verticies{};

std::vector<int> c;
std::vector<int> k;
std::vector<float> p;

void printEdges(){
    cout << "edges" << endl;
    for(int i = 0; i < edges.size(); i++){
        cout << "id: " << i << "(" << edges[i].v1 << ", " << edges[i].v2 << ")" << endl;
    }
    cout << endl;
}

// create network fully connected with m0 nodes 
void initializeBA(){
    for(int i = 0; i < m0; i++){
        for(int j = 0; j < m0; j++){
            Edge edge;
            edge.v1 = i;
            edge.v2 = j;

            if(i==j) break;
            
            // chack if an edge was written to edges 
            if(!edges.size() == 0){
                for(int k = 0; k < edges.size(); k++){
                    // if an edge does not exist in edges0 
                     if(!((edge.v1 == edges[k].v1 && edge.v2 == edges[k].v2) || (edge.v2 == edges[k].v1 && edge.v1 == edges[k].v2))){
                         edges.push_back(edge);
                         verticies[i]++;
                         verticies[j]++;
                         break;
                    }
                }
             }else{
                 edges.push_back(edge);
                 verticies[i]++;
                 verticies[j]++;
                
            }
     
        }
    }
}

void printInfo(){
    for(int i = 0; i < N; i++){
        cout << "Vertex[" << i << "]\t" << "k = " << verticies[i] << endl;
        
    }
    cout << endl;

    cout << "Edges Info" << endl;
    for(int j = 0; j < edges.size(); j++){
        cout << "i: " << "(" << edges[j].v1 << ", " << edges[j].v2 << ")" << endl;
    } 
}

void addVertex(int index){
    // add 1 vertex (1 time step)
    std::random_device rd; 
    std::mt19937 gen(rd()); 
   
    for(int i = 0; i < m; i++){
        // create an edge
        Edge edge;
        edge.v1 = index;
        
        // if we have one node then add to it new node 
        if(edges.size() == 0){
            edge.v2 = 0;
            edges.push_back(edge);
            break;
        }
        // randomly choose index of egde from e 
        std::uniform_int_distribution<> distr(0, edges.size() - 1); 
        int id = distr(gen); 
  
        // randomly choose one from 2 nodes (e[id].v1 or e[id].v2)
        std::uniform_real_distribution<> dis(0.0, 1.0);

        if(dis(gen) > 0.5)
            edge.v2 = edges[id].v1;
        else
            edge.v2 = edges[id].v2;
        
        // add an edge to vector od edges     
        edges.push_back(edge);
        
        // increase degree of nodes: node to which we add an edge and new node 
        verticies[edge.v1]++;
        verticies[edge.v2]++;
    }    
}

void createBA(){

    for( int i = m0; i < N; i++)
        addVertex(i);
}


void countFreq(std::array<int, N> &arr, int n){
    vector<bool> visited(n, false);

    for (int i = 0; i < n; i++) {
 
        if (visited[i] == true)
            continue;
  
        // count frequency
        int count = 1;
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                visited[j] = true;
                count++;                
            }
        }

        c.push_back(count);
        k.push_back(arr[i]);
        p.push_back(static_cast<float>(count)/static_cast<float>(N));
    }
}


int main(){

    initializeBA();
    createBA();
    countFreq(verticies, N);

    std::ofstream out;
    std::string pwd_path = getenv("PWD");
    std::string path = pwd_path  + "/out.dat";
    out.open(path);

    for(int i = 0; i < c.size(); i++){
        out << k[i] << "\t" << c[i] << "\t" << p[i] << std::endl;
    }

    out.close();

    // mean value of k (degree)
    float ksr = 0;
    for(int i = 0; i < k.size(); i++){
        ksr += k[i];
    }
    ksr = ksr / N;
    cout << "<k>" << ksr << endl;
    return 0;
}