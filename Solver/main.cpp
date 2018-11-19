#include "solver.h"
#include <iostream>
using namespace std;
using namespace ck;

constexpr char help_info[] = {
"Input switch is as follows:\n\
 -i: instance name;\n\
 -c: color num;\n\
 -p: perturb range;\n\
 -t: time out seconds;\n\
 -s: random seed value;\n\
 -m: max iteration steps;\n\
Switch -i is necessary and other switchs is optional." };

int main(int argc, char *argv[]) {
    char * instance_name = nullptr;
    int color_num = 10;
    GraphColoring::Configure cfg;
    for (int i = 1; i < argc; ++i) {
        if (string (argv[i]) == "-i")
            instance_name = argv[++i];
        else if (string(argv[i]) == "-c")
            color_num = atoi(argv[++i]);
        else if (string(argv[i]) == "-p")
            cfg.perturb_range = atoi(argv[++i]);
        else if (string(argv[i]) == "-t")
            cfg.timeout_seconds = atoi(argv[++i]);
        else if (string(argv[i]) == "-s")
            cfg.random_seed = atol(argv[++i]);
        else if (string(argv[i]) == "-m")
            cfg.max_tabu_steps = atoi(argv[++i]);
    }
    if (instance_name != nullptr) {
        GraphColoring solver(instance_name, cfg, color_num);
        solver.Solve();

    } else {
        string s = "DSJC125.1.col";
        //instance_name = s.begin();
        cout << instance_name << endl;
        getchar();
        GraphColoring solver(instance_name, cfg, color_num);
        solver.Solve();
        cout << help_info << endl;
    }
    getchar();
    return 0;
}