#ifndef SERVER_H
#define SERVER_H

#include "client.h"

#include <map>
#include <memory>
#include <string>

class Server {
 public:
  friend void show_wallets(const Server &server);
  Server();
  std::shared_ptr<Client> add_client(std::string id);
  std::shared_ptr<Client> get_client(std::string id) const;
  double get_wallet(std::string id) const;
  static bool parse_trx(const std::string &trx,
      std::string &sender, std::string &receiver, double &value);

 private:
  std::map<std::shared_ptr<Client>, double> clients;
};

void show_wallets(const Server &server);

#endif //SERVER_H