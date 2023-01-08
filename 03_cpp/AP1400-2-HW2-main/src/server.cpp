#include "server.h"

#include <iostream>
#include <random>

void show_wallets(const Server &server) {
  std::cout << std::string(20, '*') << std::endl;
  for (const auto &client : server.clients)
    std::cout << client.first->get_id() << " : " << client.second << std::endl;
  std::cout << std::string(20, '*') << std::endl;
}

Server::Server() {}

std::shared_ptr<Client> Server::add_client(std::string id) {
  std::string tid = id;

  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(1000, 9999);
  while (get_client(tid)) {
    tid = id + std::to_string(distribution(generator));
  }
  std::shared_ptr<Client> client = std::make_shared<Client>(tid, *this);
  clients.insert(std::make_pair(client, 5));
  return get_client(tid);
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
  for (auto item : clients) {
    if (item.first->get_id() == id) {
      return item.first;
    }
  }
  return std::shared_ptr<Client>();
}

double Server::get_wallet(std::string id) const {
  for (auto item : clients) {
    if (item.first->get_id() == id) {
      return item.second;
    }
  }
  return -1;
}

bool Server::parse_trx(const std::string &trx,
    std::string &sender, std::string &receiver, double &value) {
  auto idx1 = -1, idx2 = -1;
  for (auto i = 0; i < trx.size(); ++i) {
    if (trx[i] == '-') {
      idx1 = i;
      break;
    }
  }
  for (auto i = trx.size() - 1; i >= 0; --i) {
    if (trx[i] == '-') {
      idx2 = i;
      break;
    }
  }
  if (idx1 == -1 || idx2 == -1 || idx1 == idx2) throw std::runtime_error("");
  sender = trx.substr(0, idx1);
  receiver = trx.substr(idx1 + 1, idx2 - idx1 - 1);
  value = std::stod(trx.substr(idx2 + 1));
  return true;
}