#include "icmp.h"

using namespace std;

int main(int argc, char **argv){
    int c;
    char *host;
    ICMP pkt;

    if(optind != argc-1)
        //error
    host=argv[optind];
    pkt.echo(host);
    return 0;
}
