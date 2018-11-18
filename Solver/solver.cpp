#include "solver.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

namespace ck {

void GraphColoring::loadGraph(const string path) {
    if (path.rfind(".col") != string::npos) {
        char c; //read useless character
        char s[MAX_BUF_LEN]; // read useless string
        int v1, v2;
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

void GraphColoring::getOptima(const std::string path, int inst) {
    string instName;
    string time;

    ifstream ifs(path);
    if (!ifs.is_open()) {
        cout << "ERROR: Can't open the file!" << endl;
        return;
    } else {
        for (int i = 0; i <= inst; ++i) {
            ifs >> instName >> color_num_ >> time;
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
    for (int vj : adj_list[largest_degree_vertext]) {
        vertex_color_avail[vj][0] = false;
        num_avail_colors[vj]--;
        vertex_uncolored_degree[vj]--;
    }

    //select the vertex and color int the sequential
    for (int i = 1; i < vertex_num_; ++i) {
        // choose a vertex vi with smallest degs,the number of available colors that vertex vi can use
        int vi = min_element(num_avail_colors.begin()+i, num_avail_colors.end()) - num_avail_colors.begin();
        
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
        color[vi] = ki;
        for (int vj : adj_list[vi]) {
            vertex_uncolored_degree[vj]--;
            if (vertex_color_avail[vj][ki]) {
                num_avail_colors[vj]--;
                vertex_color_avail[vj][ki] = false;
            }
        }
    
    }
}

void GraphColoring::localSearch() {
    adj_color_table.assign(vertex_num_, vector<int>(color_num_, 0));
    for (int i = 0; i < vertex_num_; ++i) {
        for (int vj : adj_list[i]) {
            adj_color_table[i][color[vj]]++;
        }
    }
}

std::vector<int> GraphColoring::findMove() {
    return std::vector<int>();
}

void GraphColoring::makeMove(int u, int vi, int vj, int delt) {
}



}

