#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>

/**
 * Имеет смысл использовать паттерн Строитель только тогда, когда ваши продукты
 * достаточно сложны и требуют обширной конфигурации.
 *
 * В отличие от других порождающих паттернов, различные конкретные строители
 * могут производить несвязанные продукты. Другими словами, результаты различных
 * строителей могут не всегда следовать одному и тому же интерфейсу.
 */

class Product{
public:
    std::vector<std::string> parts_;

    void ListParts() const {

       std::cout << "Product parts: ";

       bool is_first = true;

       for (const std::string& part : parts_) {

          if (is_first) {
             is_first = !is_first;
             std::cout << part;
          } else {
             std::cout << ", " << part;
          }

       }

       std::cout << "\n\n";

    }
};


/**
 * Интерфейс Строителя объявляет создающие методы для различных частей объектов
 * Продуктов.
 */
class Builder{
public:
    virtual ~Builder() = default;
    virtual void ProducePartA() const =0;
    virtual void ProducePartB() const =0;
    virtual void ProducePartC() const =0;
};
/**
 * Классы Конкретного Строителя следуют интерфейсу Строителя и предоставляют
 * конкретные реализации шагов построения. Ваша программа может иметь несколько
 * вариантов Строителей, реализованных по-разному.
 */
class ConcreteBuilder1 : public Builder{
private:

    std::unique_ptr<Product> product;

    /**
     * Новый экземпляр строителя должен содержать пустой объект продукта,
     * который используется в дальнейшей сборке.
     */
public:

    ConcreteBuilder1(){
       this->Reset();
    }

    ~ConcreteBuilder1() override = default;

    void Reset() {
       this->product = std::make_unique<Product>();
    }
    /**
     * Все этапы производства работают с одним и тем же экземпляром продукта.
     */

    void ProducePartA() const override {
       this->product->parts_.emplace_back("PartA1");
    }

    void ProducePartB() const override {
       this->product->parts_.emplace_back("PartB1");
    }

    void ProducePartC() const override {
       this->product->parts_.emplace_back("PartC1");
    }

    /**
     * Конкретные Строители должны предоставить свои собственные методы
     * получения результатов. Это связано с тем, что различные типы строителей
     * могут создавать совершенно разные продукты с разными интерфейсами.
     * Поэтому такие методы не могут быть объявлены в базовом интерфейсе
     * Строителя (по крайней мере, в статически типизированном языке
     * программирования). Обратите внимание, что PHP является динамически
     * типизированным языком, и этот метод может быть в базовом интерфейсе.
     * Однако мы не будем объявлять его здесь для ясности.
     *
     * Как правило, после возвращения конечного результата клиенту, экземпляр
     * строителя должен быть готов к началу производства следующего продукта.
     * Поэтому обычной практикой является вызов метода сброса в конце тела
     * метода getProduct. Однако такое поведение не является обязательным, вы
     * можете заставить своих строителей ждать явного запроса на сброс из кода
     * клиента, прежде чем избавиться от предыдущего результата.
     */

    /**
     * Please be careful here with the memory ownership. Once you call
     * GetProduct the user of this function is responsable to release this
     * memory. Here could be a better option to use smart pointers to avoid
     * memory leaks
     */

    std::unique_ptr<Product> GetProduct() {

       std::unique_ptr<Product> result= std::move(this->product);
       this->Reset();

       return result;
    }
};

/**
 * Директор отвечает только за выполнение шагов построения в определённой
 * последовательности. Это полезно при производстве продуктов в определённом
 * порядке или особой конфигурации. Строго говоря, класс Директор необязателен,
 * так как клиент может напрямую управлять строителями.
 */
class Director{
    /**
     * @var Builder
     */
private:
    std::shared_ptr<Builder> builder_;
    /**
     * Директор работает с любым экземпляром строителя, который передаётся ему
     * клиентским кодом. Таким образом, клиентский код может изменить конечный
     * тип вновь собираемого продукта.
     */

public:

    void set_builder(std::shared_ptr<Builder> builder){
       this->builder_ = std::move(builder);
    }

    /**
     * Директор может строить несколько вариаций продукта, используя одинаковые
     * шаги построения.
     */

    void BuildMinimalViableProduct(){
       this->builder_->ProducePartA();
    }

    void BuildFullFeaturedProduct(){
       this->builder_->ProducePartA();
       this->builder_->ProducePartB();
       this->builder_->ProducePartC();
    }
};
/**
 * Клиентский код создаёт объект-строитель, передаёт его директору, а затем
 * инициирует процесс построения. Конечный результат извлекается из объекта-
 * строителя.
 */
/**
 * I used raw pointers for simplicity however you may prefer to use smart
 * pointers here
 */
void ClientCode(std::unique_ptr<Director> director)
{
   std::shared_ptr<ConcreteBuilder1> builder = std::make_shared<ConcreteBuilder1>();
   director->set_builder(builder);
   std::cout << "Standard basic product:\n";
   director->BuildMinimalViableProduct();

   std::unique_ptr<Product> p = builder->GetProduct();
   p->ListParts();

   std::cout << "Standard full featured product:\n";
   director->BuildFullFeaturedProduct();

   p = builder->GetProduct();
   p->ListParts();

   // Помните, что паттерн Строитель можно использовать без класса Директор.
   std::cout << "Custom product:\n";
   builder->ProducePartA();
   builder->ProducePartC();
   p = builder->GetProduct();
   p->ListParts();

}

int main(){
   std::unique_ptr<Director> director= std::make_unique<Director>();
   ClientCode(std::move(director));

   return 0;
}