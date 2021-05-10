#include <iostream>
#include <string>
#include <memory>

/**
 * Базовый интерфейс Компонента определяет поведение, которое изменяется
 * декораторами.
 */
class Component {
public:
    virtual ~Component() {}
    virtual std::string Operation() const = 0;
};
/**
 * Конкретные Компоненты предоставляют реализации поведения по умолчанию. Может
 * быть несколько вариаций этих классов.
 */
class ConcreteComponent : public Component {
public:

    std::string Operation() const override {

       return "ConcreteComponent";

    }
};
/**
 * Базовый класс Декоратора следует тому же интерфейсу, что и другие компоненты.
 * Основная цель этого класса - определить интерфейс обёртки для всех конкретных
 * декораторов. Реализация кода обёртки по умолчанию может включать в себя поле
 * для хранения завёрнутого компонента и средства его инициализации.
 */
class Decorator : public Component {
    /**
     * @var Component
     */
protected:

    std::shared_ptr<Component> component_;

public:

    Decorator(std::shared_ptr<Component> component) :
    component_(component)
    {}
    /**
     * Декоратор делегирует всю работу обёрнутому компоненту.
     */
    std::string Operation() const override {

       return this->component_->Operation();

    }
};
/**
 * Конкретные Декораторы вызывают обёрнутый объект и изменяют его результат
 * некоторым образом.
 */
class ConcreteDecoratorA : public Decorator {
    /**
     * Декораторы могут вызывать родительскую реализацию операции, вместо того,
     * чтобы вызвать обёрнутый объект напрямую. Такой подход упрощает расширение
     * классов декораторов.
     */
public:

    ConcreteDecoratorA(std::shared_ptr<Component> component) :
    Decorator(component)
    {}

    std::string Operation() const override {

       return "ConcreteDecoratorA(" + Decorator::Operation() + ")";

    }
};
/**
 * Декораторы могут выполнять своё поведение до или после вызова обёрнутого
 * объекта.
 */
class ConcreteDecoratorB : public Decorator {
public:

    ConcreteDecoratorB(std::shared_ptr<Component> component) :
    Decorator(component)
    {}

    std::string Operation() const override {

       return "ConcreteDecoratorB(" + Decorator::Operation() + ")";

    }
};
/**
 * Клиентский код работает со всеми объектами, используя интерфейс Компонента.
 * Таким образом, он остаётся независимым от конкретных классов компонентов, с
 * которыми работает.
 */
void ClientCode(std::shared_ptr<Component> component) {
   // ...
   std::cout << "RESULT: " << component->Operation();
   // ...
}

int main() {
   /**
    * Таким образом, клиентский код может поддерживать как простые компоненты...
    */
   std::shared_ptr<Component> simple = std::make_shared<ConcreteComponent>();
   std::cout << "Client: I've got a simple component:\n";
   ClientCode(simple);
   std::cout << "\n\n";
   /**
    * ...так и декорированные.
    *
    * Обратите внимание, что декораторы могут обёртывать не только простые
    * компоненты, но и другие декораторы.
    */
   std::shared_ptr<Component> decorator1 = std::make_shared<ConcreteDecoratorA>(simple);
   std::shared_ptr<Component> decorator2 = std::make_shared<ConcreteDecoratorB>(decorator1);
   std::cout << "Client: Now I've got a decorated component:\n";
   ClientCode(decorator2);
   std::cout << "\n";

   return 0;

}