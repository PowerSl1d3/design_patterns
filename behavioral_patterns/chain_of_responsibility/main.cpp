#include <iostream>
#include <string>
#include <vector>
#include <memory>

/**
 * Интерфейс Обработчика объявляет метод построения цепочки обработчиков. Он
 * также объявляет метод для выполнения запроса.
 */
class Handler {
public:
    virtual std::shared_ptr<Handler> SetNext(std::shared_ptr<Handler> handler) = 0;
    virtual std::string Handle(std::string request) = 0;
};
/**
 * Поведение цепочки по умолчанию может быть реализовано внутри базового класса
 * обработчика.
 */
class AbstractHandler : public Handler {
    /**
     * @var Handler
     */
private:
    std::shared_ptr<Handler> next_handler_;

public:

    AbstractHandler() : next_handler_(nullptr)
    {}

    std::shared_ptr<Handler> SetNext(std::shared_ptr<Handler> handler) override {
        this->next_handler_ = handler;
        // Возврат обработчика отсюда позволит связать обработчики простым способом,
        // вот так:
        // $monkey->setNext($squirrel)->setNext($dog);
        return handler;
    }
    std::string Handle(std::string request) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request);
        }

        return {};
    }
};
/**
 * Все Конкретные Обработчики либо обрабатывают запрос, либо передают его
 * следующему обработчику в цепочке.
 */
class MonkeyHandler : public AbstractHandler {
public:
    std::string Handle(std::string request) override {
        if (request == "Banana") {
            return "Monkey: I'll eat the " + request + ".\n";
        } else {
            return AbstractHandler::Handle(request);
        }
    }
};
class SquirrelHandler : public AbstractHandler {
public:
    std::string Handle(std::string request) override {
        if (request == "Nut") {
            return "Squirrel: I'll eat the " + request + ".\n";
        } else {
            return AbstractHandler::Handle(request);
        }
    }
};
class DogHandler : public AbstractHandler {
public:
    std::string Handle(std::string request) override {
        if (request == "MeatBall") {
            return "Dog: I'll eat the " + request + ".\n";
        } else {
            return AbstractHandler::Handle(request);
        }
    }
};
/**
 * Обычно клиентский код приспособлен для работы с единственным обработчиком. В
 * большинстве случаев клиенту даже неизвестно, что этот обработчик является
 * частью цепочки.
 */
void ClientCode(const std::shared_ptr<Handler>& handler) {
    std::vector<std::string> food = {"Nut", "Banana", "Cup of coffee"};
    for (const std::string& f : food) {
        std::cout << "Client: Who wants a " << f << "?\n";
        const std::string result = handler->Handle(f);
        if (!result.empty()) {
            std::cout << "  " << result;
        } else {
            std::cout << "  " << f << " was left untouched.\n";
        }
    }
}
/**
 * Другая часть клиентского кода создает саму цепочку.
 */
int main() {
    std::shared_ptr<MonkeyHandler> monkey = std::make_shared<MonkeyHandler>();
    std::shared_ptr<SquirrelHandler> squirrel = std::make_shared<SquirrelHandler>();
    std::shared_ptr<DogHandler> dog = std::make_shared<DogHandler>();
    monkey->SetNext(squirrel)->SetNext(dog);

    /**
     * Клиент должен иметь возможность отправлять запрос любому обработчику, а не
     * только первому в цепочке.
     */
    std::cout << "Chain: Monkey > Squirrel > Dog\n\n";
    ClientCode(monkey);
    std::cout << "\n";
    std::cout << "Subchain: Squirrel > Dog\n\n";
    ClientCode(squirrel);

    return 0;
}
