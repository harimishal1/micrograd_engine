// sudo apt install graphviz
// make

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <set>

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

    static const char* op_symbol(_op op) {
        switch (op) {
            case ADD: return "+";
            case MUL: return "*";
            case NONE: return "";
        }

        return "";
    }

    static void build_graph(const std::shared_ptr<_meta>& node,
                            std::ofstream& out, std::set<const _meta*>& visited) {
        
        if (!node || visited.count(node.get()))
            return;

        visited.insert(node.get());

        // Value node
        out << "    \"" << node.get()
            << "\" [label=\"data = " << node->data << "\"];\n";

        // Operation node
        if (node->operation != NONE) {
            std::string op_id =
                std::string("op_") +
                std::to_string(reinterpret_cast<uintptr_t>(node.get()));

            out << "    \"" << op_id
                << "\" [label=\"" << op_symbol(node->operation)
                << "\", shape=circle];\n";

            out << "    \"" << op_id
                << "\" -> \"" << node.get() << "\";\n";

            for (const auto& parent : node->prev) {
                build_graph(parent, out, visited);

                out << "    \"" << parent.get()
                    << "\" -> \"" << op_id << "\";\n";
            }
        }
    }

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

    // helpers for printing

    float data() const {
        return value->data;
    }
    
    void print_prev() const {
    for (const auto& element : value->prev) {
        //std::cout << element->data << '\n';
        printf("%f \n", element->data);
        }
    }

    void visualize(const std::string& filename) const {
        std::ofstream out(filename);

        out << "digraph G {\n";
        out << "    rankdir=LR;\n";

        std::set<const _meta*> visited;

        build_graph(value, out, visited);

        out << "}\n";
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

    d2.visualize("graph.dot");
}