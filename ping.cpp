#include "../icmp/icmp.h"

using namespace std;

int main(int argc, char **argv){
    char *host;
    ICMP pkt;

    if(argc != 2){
        cout << "Usage: ping <hostname>\n";
        exit(1);
    } //error: wrong hostname info
    host=argv[1];
    pkt.echo(host);
    return 0;
}