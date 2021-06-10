#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>

using namespace std;

int N = 1000;
std::vector<int> existed;
std::vector<int> available(N);
int *s = new int [N]{};

std::vector<int> c;
std::vector<int> x;
std::vector<float> p;

struct comma_separator : std::numpunct<char> {
    virtual char do_decimal_point() const override { return ','; }
};


void printMatrix(int *&weights, int *&degrees, int N){

    for(int i = 0; i < N*N; i++){
      
        cout  << weights[i] << " ";
        if((i + 1)% N == 0)
            cout << endl;
    }
    cout << endl;

    cout <<"d: " << endl;
    for(int i = 0; i < N; i++){
        cout << degrees[i] << " ";
    }
    cout << endl;

}

void calculateS(int *&weights, int N){

    for(int i = 0; i < N * N; i++){
        int col = i % N;
        int row = i / N;
        s[col] += weights[i];
    }
}


void countFreq(int *&arr, int n){
    vector<bool> visited(n, false);
    c.clear();
    p.clear();
    x.clear();

    for (int i = 0; i < n; i++) {
 
        if (visited[i] == true)
            continue;
  
        // zlicz czestotliwosc
        int count = 1;
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                visited[j] = true;
                count++;                
            }
        }

        c.push_back(count);
        x.push_back(arr[i]);
        p.push_back(static_cast<float>(count)/static_cast<float>(N));
    }
}

void saveToFile(string filename){
    std::ofstream out;
    std::string pwd_path = getenv("PWD");
    std::string path = pwd_path  + "/" + filename +".dat";
    out.open(path);
    out.imbue(std::locale(std::cout.getloc(), new comma_separator));

    for(int i = 0; i < p.size(); i++){
        out << c[i] << "\t" << x[i] << "\t" << p[i] << endl;
    }
    out.close();
}

void connectCluster(int w0, int m0, int *&weights, int *&degrees, int N){

    std::iota (std::begin(available), std::end(available), 0); 
   
    //choose m0 elements from N
    std::vector<int> out;
    size_t nelems = m0;
    std::sample(
        available.begin(),
        available.end(),
        std::back_inserter(out),
        nelems,
        std::mt19937{std::random_device{}()}
    );
    
 
    
    for(int i = 0; i < nelems; i++)
        existed.push_back(out[i]);

    for(int i = 0; i < nelems; i++){

        std::vector<int>::iterator it = std::find(available.begin(), available.end(), out[i]);
        int index = -6;
        if (it != available.end()){
            //std::cout << "Element Found" << std::endl;
            index = std::distance(available.begin(), it);
        }
        else{
            //std::cout << "Element Not Found" << std::endl;
        }
        
        available.erase(available.begin() + index);
    }

    for(int i = 0; i < N*N; i++){
      

        // iterate through colums
        for(int j = 0; j < nelems; j++){
            // choose one index from vector v
            //
            int col = out[j];
            
            for(int k = 0; k < nelems; k++){
                
                if(k == j)
                    continue;
                int row = out[k];
                int id = N * row + col;
                weights[id] = w0;

            }        
        }
        
    }

    for(int i = 0; i < nelems; i++){
        degrees[out[i]] = nelems - 1;
    }


}

void addVertex(int index, int m, int w0, int delta, int *&weights, int *&degrees, int N){
 
    // choose m elements from vector existed
    std::vector<int> out;
    size_t nelems = m;
    std::sample(
        existed.begin(),
        existed.end(),
        std::back_inserter(out),
        nelems,
        std::mt19937{std::random_device{}()}
    );

    // cretae m connections
    for(int i = 0; i < m; i++){
        int col = index;
        int row = out[i];
        weights[N * col + row] += w0 + delta;
        weights[N * row + col] += w0 + delta;
        degrees[row] += 1;
    }
    // add new index to existing ones
    existed.push_back(index);
    degrees[index] += 1;

    //printMatrix(weights, degrees, N);
    
}


void createNetwork(int m0, int m, int w0, int delta, int *&weights, int *&degrees, int N){

    for( int i = 0; i < available.size(); i++)
        addVertex(available[i],  m,  w0,  delta,  weights, degrees,  N);    

}


int main(){
   
    // m0 - number of nodes at the begining
    int m0 = 3;
    // m - number of nodes to add at each time step
    int m = 2;
    if(m > m0){
        cout << "Too large m" << endl;
        m = m0;
    }
        
    // wo - starting weight of each edge
    int w0 = 1;
    int delta = 1.5;
    int *weights = new int [N * N]{};
    int *degrees = new int [N]{};
    connectCluster(w0, m0, weights, degrees,  N);
    createNetwork(m0, m, w0, delta, weights, degrees, N);
    calculateS(weights,N);
    countFreq(s,N);   
    saveToFile("s");
    countFreq(degrees,N);   
    saveToFile("degrees");

    return 0;
}

