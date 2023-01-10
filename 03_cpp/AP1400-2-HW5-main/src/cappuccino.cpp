#include "cappuccino.h"
#include "sub_ingredients.h"

std::string Cappuccino::get_name() {
  return name;
}

double Cappuccino::price() {
  double res = 0;
  for (auto i : ingredients) {
    res += i->price();
  }
  for (auto i : side_items) {
    res += i->price();
  }
  return res;
}

Cappuccino::Cappuccino() : EspressoBased() {
  this->name = "Cappuccino";
  ingredients.push_back(new Espresso(2));
  ingredients.push_back(new Milk(2));
  ingredients.push_back(new MilkFoam(1));
}

Cappuccino::Cappuccino(const Cappuccino& cap) : EspressoBased(cap) {
  for (auto i : cap.side_items) {
    side_items.push_back(i->copy());
  }
}

void Cappuccino::operator=(const Cappuccino& cap) {
  if (this == &cap) {
    return;
  }
  EspressoBased::operator=(cap);
  for (const auto &i : side_items) {
    delete i;
  }
  side_items.clear();
  for (auto i : cap.side_items) {
    side_items.push_back(i->copy());
  }
}

Cappuccino::~Cappuccino() {
  for (const auto& i : side_items)
    delete i;
  side_items.clear();
}

void Cappuccino::add_side_item(Ingredient* side) {
  side_items.push_back(side);
}

std::vector<Ingredient*>& Cappuccino::get_side_items() {
  return side_items;
}