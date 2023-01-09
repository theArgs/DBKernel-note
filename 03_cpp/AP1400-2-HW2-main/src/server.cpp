#include "server.h"

#include "crypto.h"

#include <iostream>
#include <random>

std::vector<std::string> pending_trxs;

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

bool Server::add_pending_trx(std::string trx, std::string signature) const {
  std::string sender, receiver;
  double value;
  parse_trx(trx, sender, receiver, value);
  bool authentic = crypto::verifySignature(get_client(sender)->get_publickey(),
      trx, signature);
  if (!authentic || get_wallet(sender) < value) {
    return false;
  }
  pending_trxs.push_back(trx);
  return true;
}

size_t Server::mine() {
  std::string mempool;
  for (auto &t : pending_trxs) {
    mempool += t;
  }
  if (mempool.empty()) {
    return -1;
  }
  size_t idx = 0;
  size_t nonce;
  std::string sender, receiver;
  double value;
  while (true) {
    parse_trx(pending_trxs[idx], sender, receiver, value);
    nonce = get_client(sender)->generate_nonce();
    std::string hash = crypto::sha256(mempool + std::to_string(nonce));
    if (hash.substr(0, 10).find("000") != std::string::npos) {
      break;
    }
    idx = (idx + 1) % pending_trxs.size();
  }

  std::cout << sender << std::endl;
  clients[get_client(sender)] += 6.25;
  for (auto &t : pending_trxs) {
    parse_trx(t, sender, receiver, value);

    clients[get_client(sender)] -= value;
    clients[get_client(receiver)] += value;
  }
  pending_trxs.clear();
  return nonce;
}