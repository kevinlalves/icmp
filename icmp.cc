#include "icmp.h"

Icmp::Icmp() {
    type_ = kIcmpEcho;
    code_ = 0;
    id_ = getpid()&0xffff;
    seq_ = 0;
    Checksum();
}

Icmp::Icmp(uint8_t type, uint8_t code, uint16_t id, uint16_t seq) {
  type_ = type;
  code_ = code;
  id_ = id;
  seq_ = seq;
  Checksum();
}

void Icmp::Checksum() {
  cksum_ = 0;
  std::vector<uint8_t> byte_array = Encode();
  uint32_t sum = 0;
  int i = 1;
  for (auto it = byte_array.begin(); it != byte_array.end(); it++) {
    sum += *it << (8*i);
    i = (i+1)%2;
  }
  while (sum>>16)
    sum = (sum >> 16) + (sum & 0xffff);
  cksum_ = ~sum;
}

std::vector<uint8_t> Icmp::Encode() {
  std::vector<uint8_t> byte_array;
  byte_array.push_back(type_);
  byte_array.push_back(code_);

  uint16_t net_cksum = htons(cksum_);
  byte_array.insert(byte_array.end(),(uint8_t*)&net_cksum, ((uint8_t*)&net_cksum)+2);

  uint16_t net_id = htons(id_);
  byte_array.insert(byte_array.end(),(uint8_t*)&net_id, ((uint8_t*)&net_id)+2);

  uint16_t net_seq = htons(seq_);
  byte_array.insert(byte_array.end(),(uint8_t*)&net_seq, ((uint8_t*)&net_seq)+2);
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
  cksum_ = *it++ << 8;
  cksum_ += *it++;
  id_ = *it++ << 8;
  id_ += *it++;
  seq_ = *it++ << 8;
  seq_ += *it++;
  data_.insert(data_.begin(),it,byte_array.end());
  return 0;
}

void Icmp::IncrementSeq() {
  seq_++;
  Checksum();
}

void Icmp::ToString() { 
  std::cout << "seq=" << seq_ << ", id=" << id_;
}

void Icmp::SetPayload(int64_t time) {
  int len = sizeof(time);
  std::vector<uint8_t> host_byte_data((uint8_t*)&time,(uint8_t*)(&time)+len);
  std::vector<uint8_t> byte_data;
  // checks if the host is little or big endian
  if (*(uint32_t*)&time != htonl(*(uint32_t*)&time) ||
      *((uint32_t*)&time+1) != htonl(*((uint32_t*)&time)+1)) { // little endian
    byte_data.insert(byte_data.end(), host_byte_data.rbegin(), host_byte_data.rend());
  } else { // big endian
    byte_data = host_byte_data;
  }
  data_ = byte_data;
  Checksum();  
}

// Decodes only the payload, which is the time since epoch that the packet was sent
int64_t Icmp::DecodeData() {
  int64_t t_to_epoch = 0, term;
  int i = data_.size()-1;
  for (auto it = data_.begin(); it != data_.end(); it++, i--)
  {
    term = *it;
    t_to_epoch += term<<(8*i);
  }
  return t_to_epoch;
}