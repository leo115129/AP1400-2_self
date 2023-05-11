#ifndef CAPPUCCINO
#define CAPPUCCINO
#include <iostream>
#include <string>
#include <vector>

#include "espresso_based.h"
#include "ingredient.h"
#include "sub_ingredients.h"
class Cappuccino : public EspressoBased {
 public:
  Cappuccino() : EspressoBased("Cappuccino") {
    ingredients.push_back(new Espresso(2));
    ingredients.push_back(new Milk(2));
    ingredients.push_back(new MilkFoam(1));
  }
  Cappuccino(const Cappuccino& cap) : EspressoBased(cap) {
    name = cap.name;
    for (const auto& i : cap.side_items) {
      side_items.push_back(i->clone());
    }
  }
  ~Cappuccino() override {
    for (const auto& i : side_items) delete i;
    side_items.clear();
  }
  void operator=(const Cappuccino& cap) {
    if (&cap != this) {
      for (const auto& i : side_items) delete i;
      side_items.clear();
      for (const auto& i : ingredients) delete i;
      ingredients.clear();
      for (const auto& i : cap.side_items) {
        side_items.push_back(i->clone());
      }
      for (const auto& i : cap.ingredients) {
        ingredients.push_back(i->clone());
      }
    }
  }
  void brew() override {
    std::cout << "making cappuccino..." << std::endl;
    for (const auto& i : ingredients) {
      std::cout << "adding" << i->get_name() << "..." << '\n';
    }
    for (const auto& i : side_items) {
      std::cout << "adding" << i->get_name() << "..." << '\n';
    }
    std::cout << "cappuccino finshed...";
  }
  virtual std::string get_name() override { return this->name; };
  virtual double price() override {
    double sum = 0;
    for (const auto& i : this->ingredients) {
      sum += i->price();
    }
    for (const auto& i : side_items) {
      sum += i->price();
    }
    return sum;
  }

  void add_side_item(Ingredient* side) { side_items.push_back(side); }
  std::vector<Ingredient*>& get_side_items() { return side_items; }

 private:
  std::vector<Ingredient*> side_items;
};
#endif  // CAPPUCCINO