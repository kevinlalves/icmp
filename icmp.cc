#include "icmp.h"

Icmp::Icmp() {
    type_ = kIcmpEcho;
    code_ = 0;
    id_ = getpid()&0xffff;
    seq_ = 0;
}

Icmp::Icmp(uint8_t type, uint8_t code, uint16_t id, uint16_t seq) {
  type_ = type;
  code_ = code;
  id_ = id;
  seq_ = seq;
}

uint16_t Icmp::Checksum() {
  uint16_t cksum;
  uint32_t sum = type_ << 8;
  sum += code_;
  sum += id_;
  sum += seq_;
  int i = 1;
  for (auto it = data_.begin(); it != data_.end(); it++) {
    sum += *it << (8*i);
    i = (i+1)%2;
  }
  while (sum>>16)
    sum = (sum >> 16) + (sum & 0xffff);
  cksum = ~sum;
  return cksum;
}

std::vector<uint8_t> Icmp::Encode() {
  std::vector<uint8_t> byte_array;
  uint16_t cksum = htons(Checksum());
  byte_array.push_back(type_);
  byte_array.push_back(code_);
  byte_array.insert(byte_array.end(),(uint8_t*)&cksum, ((uint8_t*)&cksum)+2);

  uint16_t net_id = htons(id_);
  byte_array.insert(byte_array.end(),(uint8_t*)&net_id, ((uint8_t*)&net_id)+2);

  uint16_t net_seq = htons(seq_);
  byte_array.insert(byte_array.end(),(uint8_t*)&net_seq, ((uint8_t*)&net_seq)+2);
  byte_array.insert(byte_array.end(), data_.begin(), data_.end());
  return byte_array;
}

int Icmp::Decode(std::vector<uint8_t> &byte_array) {
  uint16_t recv_cksum;
  if(byte_array.size() < kHeaderLength)
    return -2; 
  if(byte_array.size() > kMaxDatasize)
    return -3; 
  auto it = byte_array.begin();
  type_ = *it++;
  code_ = *it++;
  recv_cksum = *it++ << 8;
  recv_cksum += *it++;
  id_ = *it++ << 8;
  id_ += *it++;
  seq_ = *it++ << 8;
  seq_ += *it++;
  data_.clear();
  data_.insert(data_.begin(),it,byte_array.end());
  if (Checksum() != recv_cksum) {
    return -1;
  }
  return 0;
}

void Icmp::IncrementSeq() {
  seq_++;
}

void Icmp::ToString() { 
  std::cout << "seq=" << seq_ << ", id=" << id_;
}

void Icmp::SetPayload(std::vector<uint8_t> data) {
  data_.clear();
  data_.insert(data_.end(), data.begin(),data.end());
}

int64_t Icmp::DecodeData() {
  int64_t t_to_epoch = 0, term;
  int i = 0;
  for (auto it = data_.begin(); it != data_.end(); it++, i++) {
    term = *it;
    t_to_epoch += term<<(8*i);
  }
  return t_to_epoch;
}