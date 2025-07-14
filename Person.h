#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
private:
    std::string name;
    int age;

public:
    Person(const std::string& name, int age);
    void greet() const;
};

#endif