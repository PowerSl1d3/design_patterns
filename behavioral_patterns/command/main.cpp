#include <iostream>
#include <string>
#include <memory>

/**
 * Интерфейс Команды объявляет метод для выполнения команд.
 */
class Command {
public:

    virtual ~Command()
    {}

    virtual void Execute() const = 0;
};
/**
 * Некоторые команды способны выполнять простые операции самостоятельно.
 */
class SimpleCommand : public Command {
private:

    std::string pay_load_;

public:

    explicit SimpleCommand(std::string pay_load) : pay_load_(pay_load)
    {}

    void Execute() const override {
        std::cout << "SimpleCommand: See, I can do simple things like printing (" << this->pay_load_ << ")\n";
    }
};

/**
 * Классы Получателей содержат некую важную бизнес-логику. Они умеют выполнять
 * все виды операций, связанных с выполнением запроса. Фактически, любой класс
 * может выступать Получателем.
 */
class Receiver {
public:

    void DoSomething(const std::string& a) {

        std::cout << "Receiver: Working on (" << a << ".)\n";

    }

    void DoSomethingElse(const std::string& b) {

        std::cout << "Receiver: Also working on (" << b << ".)\n";

    }
};

/**
 * Но есть и команды, которые делегируют более сложные операции другим объектам,
 * называемым «получателями».
 */
class ComplexCommand : public Command {
    /**
     * @var Receiver
     */
private:
    std::shared_ptr<Receiver> receiver_;
    /**
     * Данные о контексте, необходимые для запуска методов получателя.
     */
    std::string a_;
    std::string b_;
    /**
     * Сложные команды могут принимать один или несколько объектов-получателей
     * вместе с любыми данными о контексте через конструктор.
     */
public:
    ComplexCommand(std::shared_ptr<Receiver> receiver, std::string a, std::string b) :
    receiver_(receiver),
    a_(a),
    b_(b)
    {}

    /**
     * Команды могут делегировать выполнение любым методам получателя.
     */
    void Execute() const override {

        std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";

        this->receiver_->DoSomething(this->a_);
        this->receiver_->DoSomethingElse(this->b_);

    }
};

/**
 * Отправитель связан с одной или несколькими командами. Он отправляет запрос
 * команде.
 */
class Invoker {
    /**
     * @var Command
     */
private:

    std::shared_ptr<Command> on_start_;
    /**
     * @var Command
     */
    std::shared_ptr<Command> on_finish_;
    /**
     * Инициализация команд.
     */
public:

    ~Invoker() = default;

    void SetOnStart(std::shared_ptr<Command> command) {

        this->on_start_ = command;

    }
    void SetOnFinish(std::shared_ptr<Command> command) {

        this->on_finish_ = command;

    }
    /**
     * Отправитель не зависит от классов конкретных команд и получателей.
     * Отправитель передаёт запрос получателю косвенно, выполняя команду.
     */
    void DoSomethingImportant() {
        std::cout << "Invoker: Does anybody want something done before I begin?\n";
        if (this->on_start_) {
            this->on_start_->Execute();
        }
        std::cout << "Invoker: ...doing something really important...\n";
        std::cout << "Invoker: Does anybody want something done after I finish?\n";
        if (this->on_finish_) {
            this->on_finish_->Execute();
        }
    }
};
/**
 * Клиентский код может параметризовать отправителя любыми командами.
 */

int main() {

    std::unique_ptr<Invoker> invoker = std::make_unique<Invoker>();
    invoker->SetOnStart(std::make_shared<SimpleCommand>("Say Hi!"));
    std::shared_ptr<Receiver> receiver = std::make_shared<Receiver>();
    invoker->SetOnFinish(std::make_unique<ComplexCommand>(receiver, "Send email", "Save report"));
    invoker->DoSomethingImportant();

    return 0;
}