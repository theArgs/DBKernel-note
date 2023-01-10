#ifndef Q3_H
#define Q3_H

#include <fstream>
#include <queue>
#include <iostream>
#include <string>
#include <vector>

namespace q3 {

struct Flight {
  std::string flight_number;
  size_t duration;
  size_t connections;
  size_t connection_times;
  size_t price;
};

struct comparitor {
  bool operator()(const Flight &f1, const Flight &f2) const {
    return (f1.duration + f1.connection_times + 3 * f1.price) >=
        (f2.duration + f2.connection_times + 3 * f2.price);
  }
};

size_t parse_times_str(std::string str, int part) {
  std::istringstream iss(str);
  size_t res = 0;
  std::string time;
  for (int i = 0; i < part; ++i) {
    std::getline(iss, time, ',');
    int pos = 0;
    if (time.find('h') != std::string::npos) {
      pos = time.find('h');
      res += 60 * std::stoi(time.substr(0, pos));
      pos ++;
    }
    if (time.find('m') != std::string::npos) {
      res += std::stoi(time.substr(pos, time.find('m') - pos));
    }
  }
  return res;
}

auto gather_flights(std::string filename) {
  std::ifstream ifs(filename);
  std::string str;
  std::priority_queue<Flight, std::vector<Flight>, comparitor> que;
  while (std::getline(ifs, str)) {
    std::istringstream iss(str);
    std::string tstr;
    int cnt = 0;
    Flight f;
    while (iss >> tstr) {
      if (tstr.find(':') != std::string::npos) {
        size_t pos = tstr.find(':') + 1;
        switch(cnt) {
          case 0:
            f.flight_number = tstr.substr(pos);
            break;
          case 1:
            f.duration = parse_times_str(tstr.substr(pos), 1);
            break;
          case 2:
            f.connections = std::stoi(tstr.substr(pos));
            break;
          case 3:
            f.connection_times = parse_times_str(tstr.substr(pos), f.connections); 
            break;
          case 4:
            f.price = std::stoi(tstr.substr(pos));
            break;
        }
        ++cnt;
      }
    }
    que.push(f);
  }
  return que;
}

} // namespace q3

#endif //Q3_H