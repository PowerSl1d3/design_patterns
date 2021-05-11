#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

/**
 * Целевой класс объявляет интерфейс, с которым может работать клиентский код.
 */
class Target {

public:

    virtual ~Target() = default;

    virtual std::string Request() const {

       return "Target: The default target's behavior.";

    }
};

/**
 * Адаптируемый класс содержит некоторое полезное поведение, но его интерфейс
 * несовместим с существующим клиентским кодом. Адаптируемый класс нуждается в
 * некоторой доработке, прежде чем клиентский код сможет его использовать.
 */
class Adaptee {

public:

    std::string SpecificRequest() const {

       return ".eetpadA eht fo roivaheb laicepS";

    }
};

/**
 * Адаптер делает интерфейс Адаптируемого класса совместимым с целевым
 * интерфейсом.
 */
class Adapter : public Target {
private:

    std::unique_ptr<Adaptee> adaptee_;

public:

    Adapter(std::unique_ptr<Adaptee> adaptee) : adaptee_(std::move(adaptee))
    {}

    std::string Request() const override {

       std::string to_reverse = this->adaptee_->SpecificRequest();
       std::reverse(to_reverse.begin(), to_reverse.end());

       return "Adapter: (TRANSLATED) " + to_reverse;

    }

};

/**
 * Клиентский код поддерживает все классы, использующие целевой интерфейс.
 */
void ClientCode(const std::unique_ptr<Target> target) {

   std::cout << target->Request();

}

int main() {

   std::cout << "Client: I can work just fine with the Target objects:\n";
   std::unique_ptr<Target> target = std::make_unique<Target>();
   ClientCode(std::move(target));
   std::cout << "\n\n";

   std::unique_ptr<Adaptee> adaptee = std::make_unique<Adaptee>();

   std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
   std::cout << "Adaptee: " << adaptee->SpecificRequest();
   std::cout << "\n\n";
   std::cout << "Client: But I can work with it via the Adapter:\n";

   std::unique_ptr<Adapter> adapter = std::make_unique<Adapter>(std::make_unique<Adaptee>());
   ClientCode(std::move(adapter));
   std::cout << "\n";

   return 0;
}