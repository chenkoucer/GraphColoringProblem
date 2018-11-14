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

class GraphColoring {
public:
    const double kINF = INT32_MAX;
    const clock_t kStartClock = clock();
    const time_t kStartTime = time(NULL);
    const std::string kAlgorithm = "TabuSearch";
    const std::string kInstanceName;

    struct Configure {
        int consider_range = 50;
        int timeout_seconds = 10;
        int max_tabu_steps = INT32_MAX;
        unsigned int random_seed = 99995011 / (time(NULL) % 255) + 1;
        std::string ToString() {
            std::ostringstream res;
            res << "(" << consider_range << "/"
                << timeout_seconds << "/"
                << max_tabu_steps << "/"
                << random_seed << ")";
            return res.str();
        }
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
protected:
    void loadGraph(const std::string path);
    void genInitSolution();
private:
    int color_num_;
    Configure cfg_;
    std::vector<std::vector<int>> adj_list;//store the graph in adjacency list
    std::vector<int> color;//record the color of each vertex
    std::vector< std::vector<bool> > vertex_color_avail; //record the available color of each vertexs
    std::vector<int> num_avail_colors; // record the number of colors available of each vertexs
    std::vector<int> vertex_uncolored_degree;// record the number of uncolored adjacent node of the vertex
 };

}















#endif // !GRAPH_COLORING_H
