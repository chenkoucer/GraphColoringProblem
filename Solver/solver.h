#pragma once
#ifndef GRAPH_COLORING_H
#define GRAPH_COLORING_H


#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <ctime>

namespace ck {

#define NONE -1
#define kINF INT32_MAX
#define MAX_BUF_LEN 1000


class GraphColoring {
public:
    const clock_t kStartClock = clock();
    const time_t kStartTime = time(NULL);
    const std::string kAlgorithm = "TabuSearch";
    const std::string kInstanceName;
    const std::string OptimalSolName = "optima.txt";
    struct Configure {
        int perturb_range = kINF;
        int timeout_seconds = 300;
        int max_tabu_steps = kINF;
        unsigned int random_seed = 99995011 / (time(NULL) % 255) + 1;
        std::string ToString() {
            std::ostringstream res;
            res << "(" << perturb_range << "/"
                << timeout_seconds << "/"
                << max_tabu_steps << "/"
                << random_seed << ")";
            return res.str();
        }
    };

    struct MoveStruct {
        int vertex;
        int ki;
        int kj;
        int delt = 20000;
    };
public:
    explicit GraphColoring(const char* inst_name, const int color_num = 10) :kInstanceName(inst_name) {
        color_num_ = color_num;
        srand(cfg_.random_seed);
    }
    explicit GraphColoring(const char* inst_name, Configure &cfg, const int color_num = 10) : cfg_(cfg), kInstanceName(inst_name) {
        color_num_ = color_num;
        srand(cfg_.random_seed);
    }
    ~GraphColoring() {}
    void Solve();
    void Record(const char *log_path = "log.csv");

protected:
    void loadGraph(const std::string path);
    void getOptima(const std::string path);
    void genInitSolution();
    void tabuSearch();
    void initStructures();
    MoveStruct findMove();
    MoveStruct perturbSolu();
    void makeMove(MoveStruct &move_);
    const bool Check() const;
    const bool isTimeOut() const {return (clock() - kStartClock) / CLOCKS_PER_SEC > cfg_.timeout_seconds;
    }

private:
    int vertex_num_;
    int edge_num_;
    int color_num_;
    int iter_ = 0;
    //int tabu_base = 0;
    int perturbance = 0; // record the number of times that the optimal solution is not updated continuously
    
    Configure cfg_;
    
    std::vector<std::vector<int>> adj_list;//store the graph in adjacency list
    std::vector<std::vector<int>> adj_color_table; // record the number of each colors of adjacent vertex 
    std::vector<std::vector<int>> tabu_tenure; //restricts consideration to moves not forbidden 
    
    std::vector<int> color;//record the color of each vertex
    std::vector<int>best_color;//record the historical optimal sulution
    int conf_edges = 0; // record the conflicts of edges
    int best_conf_edges = kINF;//record the historical least conflicts 

    std::vector< std::vector<bool> > vertex_color_avail; //record the available color of each vertexs
    std::vector<int> num_avail_colors; // record the number of colors available of each vertexs
    std::vector<int> vertex_uncolored_degree;// record the number of uncolored adjacent node of the vertex
 };

}















#endif // !GRAPH_COLORING_H
