#include <iostream>
#include "Person.h"

Person::Person(const std::string& name, int age) : name(name), age(age) {}

void Person::greet() const {
    std::cout << "Hi, I'm " << name << " and I'm " << age << " years old.\n";
}
