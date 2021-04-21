#include <iostream>
#include <string>
#include <memory>

/**
 * Каждый отдельный продукт семейства продуктов должен иметь базовый интерфейс.
 * Все вариации продукта должны реализовывать этот интерфейс.
 */
class AbstractProductA {
public:
    virtual ~AbstractProductA() = default;
    virtual std::string UsefulFunctionA() const = 0;
};

/**
 * Конкретные продукты создаются соответствующими Конкретными Фабриками.
 */
class ConcreteProductA1 : public AbstractProductA {
public:
    std::string UsefulFunctionA() const override {
       return "The result of the product A1.";
    }
};

class ConcreteProductA2 : public AbstractProductA {
    std::string UsefulFunctionA() const override {
       return "The result of the product A2.";
    }
};

/**
 * Базовый интерфейс другого продукта. Все продукты могут взаимодействовать друг
 * с другом, но правильное взаимодействие возможно только между продуктами одной
 * и той же конкретной вариации.
 */
class AbstractProductB {
    /**
     * Продукт B способен работать самостоятельно...
     */
public:
    virtual ~AbstractProductB() = default;
    virtual std::string UsefulFunctionB() const = 0;
    /**
     * ...а также взаимодействовать с Продуктами A той же вариации.
     *
     * Абстрактная Фабрика гарантирует, что все продукты, которые она создает,
     * имеют одинаковую вариацию и, следовательно, совместимы.
     */
    virtual std::string AnotherUsefulFunctionB(const std::shared_ptr<AbstractProductA> collaborator) const = 0;
};

/**
 * Конкретные Продукты создаются соответствующими Конкретными Фабриками.
 */
class ConcreteProductB1 : public AbstractProductB {
public:
    std::string UsefulFunctionB() const override {
       return "The result of the product B1.";
    }
    /**
     * Продукт B1 может корректно работать только с Продуктом A1. Тем не менее, он
     * принимает любой экземпляр Абстрактного Продукта А в качестве аргумента.
     */
    std::string AnotherUsefulFunctionB(const std::shared_ptr<AbstractProductA> collaborator) const override {
       const std::string result = collaborator->UsefulFunctionA();
       return "The result of the B1 collaborating with ( " + result + " )";
    }

    ~ConcreteProductB1() {
       std::cout << "Product B1 was deleted;" << std::endl;
    }
};

class ConcreteProductB2 : public AbstractProductB {
public:
    std::string UsefulFunctionB() const override {
       return "The result of the product B2.";
    }
    /**
     * Продукт B2 может корректно работать только с Продуктом A2. Тем не менее, он
     * принимает любой экземпляр Абстрактного Продукта А в качестве аргумента.
     */
    std::string AnotherUsefulFunctionB(const std::shared_ptr<AbstractProductA> collaborator) const override {
       const std::string result = collaborator->UsefulFunctionA();
       return "The result of the B2 collaborating with ( " + result + " )";
    }

    ~ConcreteProductB2() {
       std::cout << "Product B2 was deleted;" << std::endl;
    }
};

/**
 * Интерфейс Абстрактной Фабрики объявляет набор методов, которые возвращают
 * различные абстрактные продукты. Эти продукты называются семейством и связаны
 * темой или концепцией высокого уровня. Продукты одного семейства обычно могут
 * взаимодействовать между собой. Семейство продуктов может иметь несколько
 * вариаций, но продукты одной вариации несовместимы с продуктами другой.
 */
class AbstractFactory {
public:
    virtual std::shared_ptr<AbstractProductA> CreateProductA() const = 0;
    virtual std::shared_ptr<AbstractProductB> CreateProductB() const = 0;

    virtual ~AbstractFactory() {};
};

/**
 * Конкретная Фабрика производит семейство продуктов одной вариации. Фабрика
 * гарантирует совместимость полученных продуктов. Обратите внимание, что
 * сигнатуры методов Конкретной Фабрики возвращают абстрактный продукт, в то
 * время как внутри метода создается экземпляр конкретного продукта.
 */
class ConcreteFactory1 : public AbstractFactory {
public:
    std::shared_ptr<AbstractProductA> CreateProductA() const override {
       return std::make_shared<ConcreteProductA1>();
    }
    std::shared_ptr<AbstractProductB> CreateProductB() const override {
       return std::make_shared<ConcreteProductB1>();
    }
};

/**
 * Каждая Конкретная Фабрика имеет соответствующую вариацию продукта.
 */
class ConcreteFactory2 : public AbstractFactory {
public:
    std::shared_ptr<AbstractProductA> CreateProductA() const override {
       return std::make_shared<ConcreteProductA2>();
    }
    std::shared_ptr<AbstractProductB> CreateProductB() const override {
       return std::make_shared<ConcreteProductB2>();
    }
};

/**
 * Клиентский код работает с фабриками и продуктами только через абстрактные
 * типы: Абстрактная Фабрика и Абстрактный Продукт. Это позволяет передавать
 * любой подкласс фабрики или продукта клиентскому коду, не нарушая его.
 */

void ClientCode(const std::unique_ptr<AbstractFactory> factory) {

   const std::shared_ptr<AbstractProductA> product_a = factory->CreateProductA();
   const std::shared_ptr<AbstractProductB> product_b = factory->CreateProductB();
   std::cout << product_b->UsefulFunctionB() << "\n";
   std::cout << product_b->AnotherUsefulFunctionB(product_a) << "\n";

}

int main() {

   std::cout << "Client: Testing client code with the first factory type:\n";
   std::unique_ptr<ConcreteFactory1> f1 =  std::make_unique<ConcreteFactory1>();
   ClientCode(std::move(f1));

   std::cout << std::endl;

   std::cout << "Client: Testing the same client code with the second factory type:\n";
   std::unique_ptr<ConcreteFactory2> f2 = std::make_unique<ConcreteFactory2>();
   ClientCode(std::move(f2));

   return 0;

}

/*
Client: Testing client code with the first factory type:
The result of the product B1.
The result of the B1 collaborating with ( The result of the product A1. )

Client: Testing the same client code with the second factory type:
The result of the product B2.
The result of the B2 collaborating with ( The result of the product A2. )
 */