#include "icmp.h"


Icmp::Icmp() {
    type_ = kIcmpEcho;
    code_ = 0;
    id_ = getpid()&0xffff;
    seq_ = 0;
    Checksum();
}

Icmp::Icmp(std::vector<uint8_t> &data) {
    type_ = kIcmpReply;
    code_ = 0;
    id_ = getpid()&0xffff;
    seq_ = 0;
    data_ = data;
    Checksum();
}


Icmp::Icmp(uint8_t type, uint8_t code, uint16_t id, uint16_t seq) {
    type_ = type;
    code_ = code;
    id_ = id;
    seq_ = seq;
    Checksum();
}


Icmp::Icmp(uint8_t type, uint8_t code, uint16_t id, uint16_t seq, std::vector<uint8_t> &data) {
    type_ = type;
    code_ = code;
    id_ = id;
    seq_ = seq;
    data_ = data;
    Checksum();
}

void Icmp::Checksum() {
  cksum_ = 0;
  auto byte_array = Encode();
  uint32_t sum = 0;
  int len = byte_array.size();
  for (auto it = byte_array.begin(); len > 1; it++, len -= 2) {
    sum += *it << 8;
    it++;
    sum += *it;
  }
  if (len > 0)
    sum += byte_array.back();
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  cksum_ = ~sum;
}

std::vector<uint8_t> Icmp::Encode() {
  std::vector<uint8_t> byte_array;
  byte_array.push_back(type_);
  byte_array.push_back(code_);
  byte_array.insert(byte_array.end(),(uint8_t*)&cksum_, ((uint8_t*)&cksum_)+2);
  byte_array.insert(byte_array.end(),(uint8_t*)&id_, ((uint8_t*)&id_)+2);
  byte_array.insert(byte_array.end(),(uint8_t*)&seq_, ((uint8_t*)&seq_)+2);
  byte_array.insert(byte_array.end(), data_.begin(), data_.end());
  return byte_array;
}

int Icmp::Decode(std::vector<uint8_t> &byte_array) {
  if(byte_array.size() < kHeaderLength)
    return -1; //insufficient data for complete header
  if(byte_array.size() > kMaxDatasize)
    return -2; //exceeded maximum size for a icmp packet 
  auto it = byte_array.begin();
  type_ = *it++;
  code_ = *it++;
  cksum_ = *it++;
  cksum_ += *it++ << 8;
  id_ = *it++;
  id_ += *it++ << 8;
  seq_ = *it++;
  seq_ += *it++ << 8;
  data_.insert(data_.begin(),it,byte_array.end());
  return 0;
}

void Icmp::IncrementSeq() {
  seq_++;
  Checksum();
}

bool Icmp::CheckId(pid_t pid) {
  return (id_ == pid);
}

void Icmp::ToString() { 
  std::cout << "seq=" << seq_ << ", id=" << id_;
}

void Icmp::SetPayload(int64_t time) {
  int len = sizeof(time);
  std::vector<uint8_t> byte_data((uint8_t*)&time,(uint8_t*)(&time)+len);
  data_ = byte_data;
  Checksum();  
}