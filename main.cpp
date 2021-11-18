#include "icmp.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv){
    char *host;
    ICMP pkt;

    if(argc != 2){
        std::cout << "Usage: ping <hostname>\n";
        exit(1);
    } //error: wrong hostname info
    host=argv[1];
    return 0;
}