#include "solver.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

namespace ck {
void GraphColoring::Solve() {
    loadGraph("../Deploy/instance/" + kInstanceName);
    cout << "导入数据完毕！" << endl;
    getOptima("../Deploy/instance/" + OptimalSolName);
    cout << "optimal solution: " << color_num_ << endl;
    genInitSolution();
    cout << "初始化完毕！" <<endl;
    tabuSearch();
    cout << "禁忌搜索完毕！" << endl;
    
}
void GraphColoring::Record(const char * log_path) {
}
void GraphColoring::loadGraph(const string path) {
    if (path.rfind(".col") != string::npos) {
        char c; //read useless character
        char s[MAX_BUF_LEN]; // read useless string
        int v1, v2;
        cout << "path: " << path << endl;
        ifstream ifs(path);
        if (!ifs.is_open()) {
            cout << "ERROR:Can't open instance file!" << endl;
            return;
        } else {
            while (true) {
                ifs >> c;
                if (c == 'c') {
                    ifs.getline(s, MAX_BUF_LEN);
                } else {
                    break;
                }
            }
            ifs >> s >> vertex_num_ >> edge_num_;
            adj_list.assign(vertex_num_, vector<int>());
            for (int i = 0; i < edge_num_; ++i) {
                ifs >> c >> v1 >> v2;
                adj_list[--v1].push_back(--v2);
                adj_list[v2].push_back(v1);
            }
            ifs.close();
        }

    }


}

void GraphColoring::getOptima(const std::string path) {
    string instName;
    string time;

    ifstream ifs(path);
    if (!ifs.is_open()) {
        cout << "ERROR: Can't open the optimal file!" << endl;
        return;
    } else {
        while (!ifs.eof()) {
            ifs >> instName >> color_num_ >> time;
            if (instName.compare(kInstanceName) == 0)
                return;
        }
        ifs.close();
    }

}

void GraphColoring::genInitSolution() {

    color.assign(vertex_num_, -1);
    vertex_color_avail.assign(vertex_num_, vector<bool>(color_num_, true));
    num_avail_colors.assign(vertex_num_, color_num_);
    int largest_degree = -1, largest_degree_vertext = -1;// find the vertex of largest degree
    for (int i = 0; i < vertex_num_; ++i) {
        int verSize = adj_list[i].size();
        vertex_uncolored_degree.push_back(verSize);
        if (verSize > largest_degree) {
            largest_degree = verSize;
            largest_degree_vertext = i;
        }
    }
    // choose the vertex v1 with largest degree, color v1 with 1
    color[largest_degree_vertext] = 0;
    num_avail_colors[largest_degree_vertext] = kINF;
    for (int vj : adj_list[largest_degree_vertext]) {
        vertex_color_avail[vj][0] = false;
        num_avail_colors[vj]--;
        vertex_uncolored_degree[vj]--;
    }

    //select the vertex and color int the sequential
    for (int i = 1; i < vertex_num_; ++i) {
        // choose a vertex vi with smallest degs,the number of available colors that vertex vi can use
        int vi = min_element(num_avail_colors.begin(), num_avail_colors.end()) - num_avail_colors.begin();
        
        // choose a color  ki with the smallest number of uncolored vertex for which color ki is available
        int ki = -1, ki_count = kINF; // record the ki and the number
        for (int k = 0; k < color_num_; ++k) {
            if (vertex_color_avail[vi][k]) {
                int count = 0;
                for (int vj : adj_list[vi]) {
                    if (vertex_color_avail[vj][k])
                        count++;
                }
                if (count < ki_count) {
                    ki_count = count;
                    ki = k;
                }
            }
        }

        // color vertex vi with ki and updating:
        if (ki == -1)
            ki = rand() % color_num_;
        color[vi] = ki;
        num_avail_colors[vi] = kINF;
        for (int vj : adj_list[vi]) {
            vertex_uncolored_degree[vj]--;
            if (vertex_color_avail[vj][ki]) {
                num_avail_colors[vj]--;
                vertex_color_avail[vj][ki] = false;
            }
        }

    }
}

void GraphColoring::tabuSearch() {
    initStructures();
    cout << "禁忌搜索前冲突边数量：  " << best_conf_edges << endl;
    while (!isTimeOut() && iter_ < cfg_.max_tabu_steps) {
        if (best_conf_edges == 0) {
            break;
        }
        MoveStruct next_move = findMove();
        if (perturbance >= cfg_.perturb_range && next_move.delt + conf_edges >= best_conf_edges) {
            next_move = perturbSolu();
        }  
        makeMove(next_move);
        iter_++;
    }
    cout << "禁忌搜索后冲突边数量：  " << best_conf_edges  << endl;
    cout << "迭代次数：" << iter_ << "   迭代时间：" << time(NULL) - kStartTime << "  迭代时钟数： "<< double(clock() - kStartClock)/1000 <<"s" <<  endl;
}

void GraphColoring::initStructures() {
    adj_color_table.assign(vertex_num_, vector<int>(color_num_, 0));
    for (int i = 0; i < vertex_num_; ++i) {
        for (int vj : adj_list[i]) {
            adj_color_table[i][color[vj]]++;
        }
    }
    for (int i = 0; i < vertex_num_; ++i) {
        if (adj_color_table[i][color[i]] > 0) {
            best_conf_vertexs++;
            conf_edges += adj_color_table[i][color[i]];
        }
    }
        
    conf_edges /= 2;
    tabu_tenure.assign(vertex_num_, vector<int>(color_num_, 0));
    best_color = color;
    best_conf_edges = conf_edges;
}

GraphColoring::MoveStruct GraphColoring::findMove() {
    MoveStruct tabu_best;
    MoveStruct no_tabu_best;
    vector< MoveStruct > tabu_moves;
    vector< MoveStruct > no_tabu_moves;
    for (int i = 0; i < vertex_num_; ++i) {
        int color_i = color[i];
        if (adj_color_table[i][color_i] > 0) {
            for (int k = 0; k < color_num_; ++k) {
                if (color_i != k) {
                    int delt_ = adj_color_table[i][k] - adj_color_table[i][color_i];
                    if (tabu_tenure[i][k] > iter_) {
                        if (delt_ <= tabu_best.delt) {
                            tabu_best.vertex = i;
                            tabu_best.ki = color_i;
                            tabu_best.kj = k;
                            if (delt_ < tabu_best.delt)
                                tabu_moves.clear();
                            tabu_best.delt = delt_;
                            tabu_moves.push_back(tabu_best);
                        }
                    } else {
                        if (delt_ <= no_tabu_best.delt) {
                            no_tabu_best.vertex = i;
                            no_tabu_best.ki = color_i;
                            no_tabu_best.kj = k;
                            if (delt_ < no_tabu_best.delt)
                                no_tabu_moves.clear();
                            no_tabu_best.delt = delt_;
                            no_tabu_moves.push_back(no_tabu_best);
                        }
                    }
                }//color_i != k
            }// k = 0:color_num_
        }//if there exists an incompatible color in adjacent edge
    }
    if (conf_edges + tabu_best.delt < best_conf_edges)
        return tabu_moves[rand() % tabu_moves.size()];
    else if (no_tabu_moves.size() > 0)
        return no_tabu_moves[rand() % no_tabu_moves.size()];
    else {
        return tabu_best;//perturbSolu();//tabu_best;
    }
    
}

GraphColoring::MoveStruct GraphColoring::perturbSolu() {
    perturbance = 0;
    MoveStruct perSolu;
    int v = rand() % vertex_num_;
    int ki = color[v], kj = rand() % color_num_;
    int delt = adj_color_table[v][kj] - adj_color_table[v][ki];
    perSolu.vertex = v;
    perSolu.ki = ki;
    perSolu.kj = kj;
    perSolu.delt = delt;
    return perSolu;
}
void GraphColoring::makeMove(MoveStruct & move_) {
    color[move_.vertex] = move_.kj;
    conf_edges += move_.delt;
    if (conf_edges < best_conf_edges) {
        //update the historical optimal solution
        best_conf_edges = conf_edges;
        best_color = color;

        //
        best_conf_vertexs = 0;
        for (int i = 0; i < vertex_num_; ++i) {
            if (adj_color_table[i][best_color[i]] > 0) {
                best_conf_vertexs++;
            }
        }
        //


        perturbance = 0;
        cout << "迭代次数：" << iter_ <<"   冲突边数量："<< best_conf_edges << "   冲突节点数量：" << best_conf_vertexs << "   迭代时间：" << double(clock() - kStartClock) / 1000 << endl;
    }
    tabu_tenure[move_.vertex][move_.ki] = iter_  + best_conf_vertexs+ rand() % 10;
    perturbance++;
    for (int i : adj_list[move_.vertex]) {
        adj_color_table[i][move_.ki]--;
        adj_color_table[i][move_.kj]++;
    }
}

const bool GraphColoring::Check() const {
    return false;
}

}


