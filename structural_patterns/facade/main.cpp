#include <iostream>
#include <string>
#include <memory>

/**
 * Подсистема может принимать запросы либо от фасада, либо от клиента напрямую.
 * В любом случае, для Подсистемы Фасад – это еще один клиент, и он не является
 * частью Подсистемы.
 */
class Subsystem1 {
public:

    std::string Operation1() const {

       return "Subsystem1: Ready!\n";

    }
    // ...
    std::string OperationN() const {

       return "Subsystem1: Go!\n";

    }
};
/**
 * Некоторые фасады могут работать с разными подсистемами одновременно.
 */
class Subsystem2 {
public:

    std::string Operation1() const {

       return "Subsystem2: Get ready!\n";

    }
    // ...
    std::string OperationZ() const {

       return "Subsystem2: Fire!\n";

    }
};

/**
 * Класс Фасада предоставляет простой интерфейс для сложной логики одной или
 * нескольких подсистем. Фасад делегирует запросы клиентов соответствующим
 * объектам внутри подсистемы. Фасад также отвечает за управление их жизненным
 * циклом. Все это защищает клиента от нежелательной сложности подсистемы.
 */
class Facade {
protected:

    std::unique_ptr<Subsystem1> subsystem1_;
    std::unique_ptr<Subsystem2> subsystem2_;
    /**
     * В зависимости от потребностей вашего приложения вы можете предоставить
     * Фасаду существующие объекты подсистемы или заставить Фасад создать их
     * самостоятельно.
     */
public:
    /**
     * In this case we will delegate the memory ownership to Facade Class
     */
    Facade(std::unique_ptr<Subsystem1> subsystem1 = nullptr,
           std::unique_ptr<Subsystem2> subsystem2 = nullptr)
            {
               this->subsystem1_ = std::move(subsystem1) ? : std::make_unique<Subsystem1>();
               this->subsystem2_ = std::move(subsystem2) ? : std::make_unique<Subsystem2>();
            }

    ~Facade() = default;
    /**
     * Методы Фасада удобны для быстрого доступа к сложной функциональности
     * подсистем. Однако клиенты получают только часть возможностей подсистемы.
     */
    std::string Operation() {

       std::string result = "Facade initializes subsystems:\n";
       result += this->subsystem1_->Operation1();
       result += this->subsystem2_->Operation1();
       result += "Facade orders subsystems to perform the action:\n";
       result += this->subsystem1_->OperationN();
       result += this->subsystem2_->OperationZ();

       return result;

    }
};

/**
 * Клиентский код работает со сложными подсистемами через простой интерфейс,
 * предоставляемый Фасадом. Когда фасад управляет жизненным циклом подсистемы,
 * клиент может даже не знать о существовании подсистемы. Такой подход позволяет
 * держать сложность под контролем.
 */
void ClientCode(std::unique_ptr<Facade> facade) {
   // ...
   std::cout << facade->Operation();
   // ...
}
/**
 * В клиентском коде могут быть уже созданы некоторые объекты подсистемы. В этом
 * случае может оказаться целесообразным инициализировать Фасад с этими
 * объектами вместо того, чтобы позволить Фасаду создавать новые экземпляры.
 */

int main() {
   std::unique_ptr<Subsystem1> subsystem1 = std::make_unique<Subsystem1>();
   std::unique_ptr<Subsystem2> subsystem2 = std::make_unique<Subsystem2>();
   std::unique_ptr<Facade> facade = std::make_unique<Facade>(std::move(subsystem1), std::move(subsystem2));

   ClientCode(std::move(facade));

   return 0;

}