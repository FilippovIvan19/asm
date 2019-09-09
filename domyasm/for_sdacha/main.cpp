#include "softCPU.h"
#include "bin_comp.h"


int main(int argc, char **argv)
{

    if      (!strcmp(argv[1], "CPUcomp"))
        compile(argv[2], argv[3]);
    else if (!strcmp(argv[1], "CPUrun"))
        do_cpu(argv[2]);
    else if (!strcmp(argv[1], "BINrun"))
        bin_compile_and_run(argv[2]);
    


    return 0;
}