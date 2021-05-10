#include <iostream>
#include <string>
#include <memory>

/**
 * Реализация устанавливает интерфейс для всех классов реализации. Он не должен
 * соответствовать интерфейсу Абстракции. На практике оба интерфейса могут быть
 * совершенно разными. Как правило, интерфейс Реализации предоставляет только
 * примитивные операции, в то время как Абстракция определяет операции более
 * высокого уровня, основанные на этих примитивах.
 */

class Implementation {
public:

    virtual ~Implementation() = default;

    virtual std::string OperationImplementation() const = 0;

};

/**
 * Каждая Конкретная Реализация соответствует определённой платформе и реализует
 * интерфейс Реализации с использованием API этой платформы.
 */
class ConcreteImplementationA : public Implementation {
public:

    std::string OperationImplementation() const override {

       return "ConcreteImplementationA: Here's the result on the platform A.\n";

    }

};

class ConcreteImplementationB : public Implementation {
public:

    std::string OperationImplementation() const override {

       return "ConcreteImplementationB: Here's the result on the platform B.\n";

    }

};

/**
 * Абстракция устанавливает интерфейс для «управляющей» части двух иерархий
 * классов. Она содержит ссылку на объект из иерархии Реализации и делегирует
 * ему всю настоящую работу.
 */

class Abstraction {
    /**
     * @var Implementation
     */
protected:

    std::unique_ptr<Implementation> implementation_;

public:

    explicit Abstraction(std::unique_ptr<Implementation> implementation) :
    implementation_(std::move(implementation))
    {}

    virtual ~Abstraction()
    {}

    virtual std::string Operation() const {

       return "Abstraction: Base operation with:\n" +
              this->implementation_->OperationImplementation();

    }
};
/**
 * Можно расширить Абстракцию без изменения классов Реализации.
 */
class ExtendedAbstraction : public Abstraction {
public:

    ExtendedAbstraction(std::unique_ptr<Implementation> implementation) : Abstraction(std::move(implementation))
    {}

    std::string Operation() const override {

       return "ExtendedAbstraction: Extended operation with:\n" +
              this->implementation_->OperationImplementation();

    }
};

/**
 * За исключением этапа инициализации, когда объект Абстракции связывается с
 * определённым объектом Реализации, клиентский код должен зависеть только от
 * класса Абстракции. Таким образом, клиентский код может поддерживать любую
 * комбинацию абстракции и реализации.
 */
void ClientCode(const Abstraction& abstraction) {
   // ...
   std::cout << abstraction.Operation();
   // ...
}
/**
 * Клиентский код должен работать с любой предварительно сконфигурированной
 * комбинацией абстракции и реализации.
 */

int main() {

   std::unique_ptr<Implementation> implementation = std::make_unique<ConcreteImplementationA>();
   std::unique_ptr<Abstraction> abstraction = std::make_unique<Abstraction>(std::move(implementation));
   ClientCode(*abstraction);
   std::cout << std::endl;

   implementation = std::make_unique<ConcreteImplementationB>();
   abstraction = std::make_unique<ExtendedAbstraction>(std::move(implementation));
   ClientCode(*abstraction);

   return 0;
}