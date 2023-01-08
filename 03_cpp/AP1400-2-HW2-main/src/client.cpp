#include "client.h"
#include "server.h"

#include "crypto.h"

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