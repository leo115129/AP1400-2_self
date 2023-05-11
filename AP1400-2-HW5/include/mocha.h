#ifndef MOCHA_H
#define MOCHA_H
#include "espresso_based.h"
#include "ingredient.h"
#include "sub_ingredients.h"
class Mocha : public EspressoBased {
 private:
  std::vector<Ingredient*> side_items;

 public:
  Mocha() : EspressoBased("Mocha") {
    ingredients.push_back(new Espresso(2));
    ingredients.push_back(new Milk(2));
    ingredients.push_back(new MilkFoam(1));
    ingredients.push_back(new Chocolate(1));
  }
  Mocha(const Mocha& cap) : EspressoBased(cap) {
    name = cap.name;
    for (const auto& i : cap.side_items) {
      side_items.push_back(i->clone());
    }
  }
  ~Mocha() override {
    for (const auto& i : side_items) delete i;
    side_items.clear();
  }
  void operator=(const Mocha& cap) {
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

  // function
  void add_side_item(Ingredient* side) { side_items.push_back(side); }
  std::vector<Ingredient*>& get_side_items() { return side_items; }
  virtual std::string get_name() override { return this->name; };
  void brew()override{
    std::cout << "making Mocha..." << std::endl;
    for (const auto& i : ingredients) {
      std::cout << "adding" << i->get_name() << "..." << '\n';
    }
    for (const auto& i : side_items) {
      std::cout << "adding" << i->get_name() << "..." << '\n';
    }
    std::cout << "Mocha finshed...";
  }
};
#endif  // MOCHA_H