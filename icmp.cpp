#include "icmp.h"


Icmp::Icmp(){
    type = kIcmpEcho;
    code = 0;
    id = getpid()&0xffff;
    seq = 0;
    Checksum();
}

Icmp::Icmp(std::vector<uint8_t> &_data){
    type = kIcmpReply;
    code = 0;
    id = getpid()&0xffff;
    seq = 0;
    data = _data;
    Checksum();
}


Icmp::Icmp(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq){
    type = _type;
    code = _code;
    id = _id;
    seq = _seq;
    Checksum();
}


Icmp::Icmp(uint8_t _type, uint8_t _code, uint16_t _id, uint16_t _seq, std::vector<uint8_t> &_data){
    type = _type;
    code = _code;
    id = _id;
    seq = _seq;
    data = _data;
    Checksum();
}

void Icmp::Checksum(){
    cksum = 0;
    auto byte_array = Encode();
    uint32_t sum = 0;
    int len = byte_array.size();
    for (auto it = byte_array.begin(); len > 1; it++, len -= 2)
    {
        sum += *it << 8;
        it++;
        sum += *it;
    }
    if(len > 0)
        sum += byte_array.back();
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    cksum = ~sum;
}

std::vector<uint8_t> Icmp::Encode(){
    std::vector<uint8_t> byte_array;
    byte_array.push_back(type);
    byte_array.push_back(code);
    byte_array.insert(byte_array.end(),(uint8_t*)&cksum, ((uint8_t*)&cksum)+2);
    byte_array.insert(byte_array.end(),(uint8_t*)&id, ((uint8_t*)&id)+2);
    byte_array.insert(byte_array.end(),(uint8_t*)&seq, ((uint8_t*)&seq)+2);
    byte_array.insert(byte_array.end(), data.begin(), data.end());
    return byte_array;
}

int Icmp::Decode(std::vector<uint8_t> &byte_array){
    if(byte_array.size() < kHeaderLength)
        return -1; //insufficient data for complete header
    if(byte_array.size() > kMaxDatasize)
        return -2; //exceeded maximum size for a icmp packet 
    auto it = byte_array.begin();
    type = *it++;
    code = *it++;
    cksum = *it++;
    cksum += *it++ << 8;
    id = *it++;
    id += *it++ << 8;
    seq = *it++;
    seq += *it++ << 8;
    data.insert(data.begin(),it,byte_array.end());
    return 0;
}

void Icmp::IncrementSeq(){
    seq++;
    Checksum();
}

bool Icmp::CheckId(pid_t pid){
    return id == pid;
}

void Icmp::ToString(){ 
    std::cout << "seq=" << seq << ", id=" << id;
}

void Icmp::SetPayload(int64_t time){
    int len = sizeof(time);
    std::vector<uint8_t> byte_data((uint8_t*)&time,(uint8_t*)(&time)+len);
    data = byte_data;
    Checksum();  
}