#ifndef Q2_H
#define Q2_H

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace q2 {

struct Patient {
  std::string name;
  size_t age;
  size_t smokes;
  size_t area_q;
  size_t alkhol;
};

std::vector<Patient> read_file(std::string filename) {
  std::ifstream ifs(filename);
  std::vector<Patient> res;
  std::string str;
  std::getline(ifs, str);
  std::getline(ifs, str);
  while (std::getline(ifs, str)) {
    std::istringstream iss(str);
    std::string name, surname, tmp;
    size_t age, smokes, area_q, alkhol;
    getline(iss, name, ',');
    getline(iss, surname, ',');
    getline(iss, tmp, ','), age = std::stoul(tmp);
    getline(iss, tmp, ','), smokes = std::stoul(tmp);
    getline(iss, tmp, ','), area_q = std::stoul(tmp);
    getline(iss, tmp, ','), alkhol = std::stoul(tmp);
    if (name.ends_with(' ')) {
      name += surname;
    } else {
      name = name + " " + surname;
    }
    res.push_back({name, age,
        smokes, area_q, alkhol});
  }
  return res;
}

void sort(std::vector<Patient> &patients) {
  sort(patients.begin(), patients.end(), [](auto p1, auto p2) {
    return 3 * p1.age + 5 * p1.smokes + 2 * p1.area_q + 4 * p1.alkhol >
        3 * p2.age + 5 * p2.smokes + 2 * p2.area_q + 4 * p2.alkhol;
  });
}

}  // namespace q2

#endif //Q2_H