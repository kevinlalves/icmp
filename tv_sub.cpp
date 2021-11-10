#include "icmp.h"

void ICMP::tv_sub(timeval *out, timeval *in){
    if((out->tv_usec -= in->tv_usec) < 0){
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}