#include <iostream>
#include <memory>
#include <string>

class State {
public:
    virtual std::vector<int64_t> getFunctionResult(const std::string& str) = 0;
    virtual ~State() = default;
};

class PrefixFunction : public State {
public:
    void std::vector<int64_t> getFunctionResult(const std::string& str) override {}
};

class CFunction : public State {
public:
    void std::vector<int64_t> getFunctionResult(const std::string& str) override {}
};

class ManakerOdd : public State {
public:
    void std::vector<int64_t> getFunctionResult(const std::string& str) override {}
};

class ManakerEven : public State {
public:
    void std::vector<int64_t> getFunctionResult(const std::string& str) override {}
};


class StringFunction {
private:
    std::shared_ptr<State> currentState;

public:
    VendingMachine() : currentState(std::make_shared<PrefixFunction>()) {}

    void setState(const std::shared_ptr<State>& newState) {
        currentState = newState;
    }

    void std::vector<int64_t> getFunctionResult(const std::string& str){
        currentState->getFunctionResult(str);
    }

    bool isCorrectPrefix(const std::vector<int64_t>& preffunc) {

    }
};

int main() {
    StringFunction algo;

    algo.getFunctionResult("abcdeabc");

    algo.setState(std::make_shared<ZFunction>());

    algo.getFunctionResult("abcdeabc");

    return 0;
}
