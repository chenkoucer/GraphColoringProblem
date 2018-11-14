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
        ifstream ifs(path);
        if (!ifs.open) {
            cout << "ERROR:Can't open instance file!" << endl;
            return;
        } else {

        }
    }


}

void GraphColoring::genInitSolution() {
}
}

