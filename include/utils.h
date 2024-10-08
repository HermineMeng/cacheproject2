#pragma once

#include "string.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <source_location>
#include <string>
#include <unordered_map>
#include <unordered_set>

#define COORDINATOR_RPC_PORT 11111
#define CLIENT_TRANSFER_DATA_PORT 22222

#define my_assert(condition)                                                   \
  exit_when((condition), std::source_location::current())

enum class Encode_Type { Azure_LRC };

enum class Placement_Type {
  random,
  flat,
  strategy_ECWIDE,
  strategy_ICPP23_IGNORE_LOAD,
  strategy_ICPP23_CONSIDER_LOAD
};

typedef struct {
  bool partial_decoding;
  Encode_Type encode_type;
  Placement_Type placement_type;
  int k;      // num_of_data_block
  int real_l; // num_of_local_parity_block, 不包括Azure_LRC_1中的“1”
  int g;      // num_of_global_parity_block
  int b;      // num_of_data_block_per_group
  size_t stripe_size_upper;
} EC_schema;

typedef struct {
  size_t value_len;
  std::vector<unsigned int> stripes;
} meta_info_of_object;

typedef struct {
  unsigned int stripe_id;
  Encode_Type encode_type;
  Placement_Type placement_type;
  int k;
  int real_l;
  int g;
  int b;
  size_t block_size;
  // 依次为数据块、全局校验块、局部校验块
  std::vector<unsigned int> nodes;
  /*依次为1个全局校验块、所有局部校验块*/
  std::vector<unsigned int> cache_nodes;
} stripe_item;

typedef struct {
  unsigned int node_id;

  unsigned int cluster_id;
  std::string ip;
  int port;
  std::unordered_set<unsigned int> stripe_ids;

  double storage_cost;
  double network_cost;
  double bandwidth;
  double storage;
} node_item;

typedef struct {
  unsigned int cache_node_id;

  unsigned int cluster_id;
  std::string ip;
  int port;
  std::unordered_set<unsigned int> stripe_ids;

  double storage_cost;
  double network_cost;
  double bandwidth;
  double storage;
} cache_node_item;

typedef struct {
  unsigned int cluster_id;
  std::string proxy_ip;
  int proxy_port;
  std::vector<unsigned int> nodes;
  /*cache_node*/
  std::vector<unsigned int> cache_nodes;
} cluster_item;

typedef struct {
  Encode_Type encode_type;
  std::vector<unsigned int> stripe_ids;
  std::string key;
  int k;
  int real_l;
  int g;
  int b;
  size_t value_len;
  size_t block_size;
  size_t tail_block_size;
  std::vector<std::pair<std::string, int>> datanode_ip_port;
  /**/
  std::vector<std::pair<std::string, int>> cachenode_ip_port;

  std::string client_ip;
  int client_port;
} placement_info;

typedef struct {
  unsigned int cluster_id;
  int k;
  int real_l;
  int g;
  int b;
  size_t block_size;
  unsigned int stripe_id;
  Encode_Type encode_type;
  bool partial_decoding;
  bool multi_clusters_involved;
  bool single_cluster_involved;
  std::vector<std::pair<std::pair<std::string, int>, int>>
      inner_cluster_help_blocks_info;
  std::vector<int> live_blocks_index;
  std::vector<int> failed_blocks_index;
  std::vector<std::pair<std::pair<std::string, int>, int>> new_locations;
  std::vector<unsigned int> help_cluster_ids;
} main_repair_plan;

typedef struct {
  unsigned int cluster_id;
  int k;
  int real_l;
  int g;
  int b;
  size_t block_size;
  unsigned int stripe_id;
  Encode_Type encode_type;
  bool partial_decoding;
  bool multi_clusters_involved;
  std::vector<std::pair<std::pair<std::string, int>, int>>
      inner_cluster_help_blocks_info;
  std::vector<int> live_blocks_index;
  std::vector<int> failed_blocks_index;
  std::string main_proxy_ip;
  int main_proxy_port;
} help_repair_plan;

// 生成若干不重复的随机字符串对
void generate_unique_random_strings(
    int key_length, int value_length, int n,
    std::unordered_map<std::string, std::string> &key_value);

// 生成某个数组或容器的随机索引
int random_index(size_t len);

void exit_when(bool condition, const std::source_location &location);

int bytes_to_int(std::vector<unsigned char> &bytes);

std::vector<unsigned char> int_to_bytes(int integer);