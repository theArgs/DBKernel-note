#include "client.h"
#include "server.h"

#include "crypto.h"

#include <random>

Client::Client(std::string id, const Server &server) : server(&server), id(id) {
  crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() const {
  return id;
}

std::string Client::get_publickey() const {
  return public_key;
}

double Client::get_wallet() const {
  return server->get_wallet(id);
}

std::string Client::sign(const std::string &txt) const {
  return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value) const {
  if (!server->get_client(receiver) || server->get_wallet(id) < value) {
    return false;
  }
  std::string msg = id + "-" + receiver + "-" + std::to_string(value);
  return server->add_pending_trx(msg, crypto::signMessage(private_key, msg));
}

size_t Client::generate_nonce() const {
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(0);
  return distribution(generator);
}