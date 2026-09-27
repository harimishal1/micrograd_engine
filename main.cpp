#include <iostream>
#include <memory>
#include <vector>

class Value {
private:
    enum _op{
        NONE,
        ADD,
        MUL,
    };
    
    struct _meta {
        _op operation;
        float data;
        std::vector <std::shared_ptr<_meta>> prev;
        _meta (float data) : operation(NONE), data(data) {}
    };
    std::shared_ptr<_meta> value;

public:
    Value(float data)
        : value(std::make_shared<_meta>(data)) {}
        
    Value operator+(const Value& other) {
        Value sol(this->value->data + other.value->data);
        sol.value->prev.push_back(this->value);
        sol.value->prev.push_back(other.value);
        sol.value->operation = ADD;
        return sol;
    }

    Value operator+(float other) {
        Value sol(this->value->data + other);
        sol.value->prev.push_back(this->value);
        sol.value->operation = ADD;
        return sol;
    }

    Value operator*(const Value& other) {
        Value sol(this->value->data * other.value->data);
        sol.value->prev.push_back(this->value);
        sol.value->prev.push_back(other.value);
        sol.value->operation = MUL;
        return sol;
    }

    Value operator*(float other) {
        Value sol(this->value->data * other);
        sol.value->prev.push_back(this->value);
        sol.value->operation = MUL;
        return sol;
    }
    
    void print_prev() const {
    for (const auto& element : value->prev) {
        //std::cout << element->data << '\n';
        printf("%f \n", element->data);
        }
    }

    float data() const {
        return value->data;
    }
};

int main() {
    float h = 0.001;
    Value a(3.0);
    Value b(2.0);
    Value c(10.0);
    Value d = a * b;
    Value d1 = d + c;
    a = a + h; 
    d = a * b;
    Value d2 = d + c;
    d.print_prev();
    d2.print_prev();
    std::cout<< "slope: " << (d2.data() - d1.data())/h << "\n";
}