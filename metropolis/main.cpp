#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <locale> 
#include <cmath>

std::vector<int> c;
std::vector<int> k;
std::vector<float> p;

// repace dot with comma for further calculations
struct comma_separator : std::numpunct<char> {
    virtual char do_decimal_point() const override { return ','; }
};


void countFreq(int  *arr, int N){
    std::vector<int> visited(N, false);

    for (int i = 0; i < N; i++) {
 
        if (visited[i] == true)
            continue;
  
        // claculate counts
        int count = 1;
        for (int j = i + 1; j <N; j++) {
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

int* getDegree(int *matrix, int N){
    int* degrees = new int[N]{};
    
    for(int k = 0; k < N; k++){
        for(int j = 0; j < N; j++)
            degrees[k] += matrix[k*N + j];        
    }

    return degrees;
}

// Make adjacency matrix - ER graph
void makeMatrix(int* matrix, float p, int N){

    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(0.0, 1.0);

    for(int i = 0; i < N; i++){

        for(int j = i + 1; j < N; j++){
                       
            float r = distr(gen);
            if(i == j)
                continue; 
            if(r < p){
                matrix[i * N + j] = 1;
                matrix[j * N + i] = 1; 
            }
        }
    }
    countFreq(getDegree(matrix, N), N);
}

void makeMCmatrix(int *matrix, int N, float p, int freq, int max){

    std::ofstream out;
    std::string pwd_path = getenv("PWD");
    std::string path = pwd_path  + "/a_degree.dat";
    out.open(path);
    out.imbue(std::locale(std::cout.getloc(), new comma_separator));

    int E = 0; // number of edges in the graph
    int i = 0;


    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr0(0, (N * N) - 1); 

    std::uniform_real_distribution<> distr(0.0, 1.0);

    bool theta_greater = true;
    if(log(p /( 1 - p)) > 0)
        theta_greater = true;
    else
        theta_greater = false;

    std::cout << "theta_greater = " << theta_greater << std::endl;

    while(true){
        i++;
        if(i > max)
            break;
        if(i % freq == 0)
            out << i << "\t" <<  static_cast<float>(2*E) / static_cast<float>(N) << std::endl;
         
         // get random element from adjajency matrix 
        int id = distr0(gen);
    
        // if we randomly get element form main diagonal then do nothing
        if(id % (N + 1) == 0)
            continue;
        

        int k = id / N;
        int j = id % N;


        if(matrix[id] == 0){
            
            if(true == theta_greater){
                
                matrix[k * N + j] = 1;
                matrix[j * N + k] = 1;
                E++;
            }else{
                float r = distr(gen);

                float e =  (p /  static_cast<float>(1.0 - p));
                if(r < e){
                    matrix[k * N + j] = 1;
                    matrix[j * N + k] = 1;
                    E++; 
                }
            }


        }else{
            if(true == theta_greater){
                float r = distr(gen);

                float e = 1.0 / p - 1;
                if(r < e){
                    matrix[k * N + j] = 0;
                    matrix[j * N + k] = 0;
                    E--; 
                }
            }else{
                
                matrix[k * N + j] = 0;
                matrix[j * N + k] = 0;
                E--; 
            }
      
        }
   
    }
    out.close();

    countFreq(getDegree(matrix, N), N);
}

int main(){ 

    const int N = 500;
    int *matrix = new int[N * N]{};

    float p_graph = 0.1;
    //makeMatrix(matrix, p_graph, N);
    makeMCmatrix(matrix, N, p_graph, 1000, 1000000);


    std::ofstream out;
    std::string pwd_path = getenv("PWD");
    std::string path = pwd_path  + "/out.dat";
    out.open(path);
    out.imbue(std::locale(std::cout.getloc(), new comma_separator));

    for(int i = 0; i < c.size(); i++)
        out << k[i] << "\t" << c[i] << "\t" << p[i] << std::endl;
    
    out.close();
 
    return 0;
}