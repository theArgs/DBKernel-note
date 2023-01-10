#include "mocha.h"
#include "sub_ingredients.h"

std::string Mocha::get_name() {
  return name;
}

double Mocha::price() {
  double res = 0;
  for (auto i : ingredients) {
    res += i->price();
  }
  for (auto i : side_items) {
    res += i->price();
  }
  return res;
}

Mocha::Mocha() : EspressoBased() {
  this->name = "Mocha";
  ingredients.push_back(new Espresso(2));
  ingredients.push_back(new Milk(2));
  ingredients.push_back(new MilkFoam(1));
  ingredients.push_back(new Chocolate(1));
}

Mocha::Mocha(const Mocha& moc) : EspressoBased(moc) {
  for (auto i : moc.side_items) {
    side_items.push_back(i->copy());
  }
}

void Mocha::operator=(const Mocha& moc) {
  if (this == &moc) {
    return;
  }
  EspressoBased::operator=(moc);
  for (const auto &i : side_items) {
    delete i;
  }
  side_items.clear();
  for (auto i : moc.side_items) {
    side_items.push_back(i->copy());
  }
}

Mocha::~Mocha() {
  for (const auto& i : side_items)
    delete i;
  side_items.clear();
}

void Mocha::add_side_item(Ingredient* side) {
  side_items.push_back(side);
}

std::vector<Ingredient*>& Mocha::get_side_items() {
  return side_items;
}