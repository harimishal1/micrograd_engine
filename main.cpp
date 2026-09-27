#include <iostream>
#include <memory>
#include <vector>

class Value {
private:
    struct meta_data {
        float data;
        std::vector <std::shared_ptr<meta_data>> prev;
        meta_data (float data) : data(data) {}
    };
    std::shared_ptr<meta_data> value;

public:
    Value(float data)
        : value(std::make_shared<meta_data>(data)) {}
        
    Value operator+(const Value& other) {
        Value sol(this->value->data + other.value->data);
        sol.value->prev.push_back(this->value);
        sol.value->prev.push_back(other.value);
        return sol;
    }

    Value operator+(float other) {
        Value sol(this->value->data + other);
        return sol;
    }

    Value operator*(const Value& other) {
        Value sol(this->value->data * other.value->data);
        sol.value->prev.push_back(this->value);
        sol.value->prev.push_back(other.value);
        return sol;
    }

    Value operator*(float other) {
        Value sol(this->value->data * other);
        return sol;
    }
    
    void print_prev() const {
    for (const auto& element : value->prev) {
        std::cout << element->data << '\n';
    }
}
};

int main() {
    float h = 0.001;
    Value a(3.0);
    Value b(2.0);
    Value c(10.0);
    Value d(a * b);
    Value d1(d + c);
    a = a + h; 
    Value d2(d + c);
    d.print_prev();
    d1.print_prev();
    d2.print_prev();
}