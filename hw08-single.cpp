// Zoe Dauphinee, homework 8, linked list polynomials

#include <iostream>
#include <vector>
using namespace std;

class Polynomial {
    public:
        Polynomial();
        Polynomial(vector<int> coefficients);
        ~Polynomial();
        Polynomial(const Polynomial& other);
        Polynomial& operator=(const Polynomial& other);
        Polynomial& operator+=(const Polynomial& other);
        Polynomial operator+(const Polynomial& other);
        bool operator==(const Polynomial& other);
        bool operator!=(const Polynomial& other);
        friend ostream& operator<<(ostream& os, const Polynomial& poly);
        int evaluate(int val);
        int get_degree() {
            return degree;
        }
        void strip_leading_zeros();

    private:
        struct Node {
            Node* next;
            int data;
        };
        Node* header;
        Node* trailer;
        int degree;
};





void doNothing(Polynomial temp) {}

int main() {
        
    //test constructor
    Polynomial p1({17});                 // 17
    Polynomial p2({1, 2});               // x + 2
    Polynomial p3({-1, 5});              // -x + 5
    Polynomial p4({5, 4, 3, 2, 1});      // 5x^4 + 4x^3 + 3x^2 + 2x + 1
    Polynomial has_a_zero({4, 0, 1, 7}); // 4x^3 + x + 7

    Polynomial temp(p4);
    cout << "temp: " << temp << endl;
        
    cerr << "displaying polynomials\n";
    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;
    cout << "p3: " << p3 << endl;
    cout << "p4: " << p4 << endl;
    cout << "has_a_zero: " << has_a_zero << endl;

    Polynomial temp2;
    temp2 = p2 + p3;
    cout << "temp2: " << temp2 << endl;
    
    cout << "p2 + p3: " << (p2+p3) << endl; 
    cout << "p2 + p4: " << (p2+p4) << endl; 
    cout << "p4 + p2: " << (p4+p2) << endl;

    //test copy constructor - the statement below uses the copy constructor
    //to initialize poly3 with the same values as poly4
    Polynomial p5(p4);
    p5 += p3;
    cout << "Polynomial p5(p4);\n"
         << "p5 += p3;\n";

    cout << "p4: " << p4 << endl;  
    cout << "p5: " << p5 << endl;  


    cout << "Calling doNothing(p5)\n";
    doNothing(p5);
    cout << "p5: " << p5 << endl;

    //tests the assignment operator
    Polynomial p6;
    cout << "p6: " << p6 << endl;
    cout << boolalpha;  // Causes bools true and false to be printed that way.
    cout << "(p4 == p6) is " << (p4 == p6) << endl;
    p6 = p4;
    cout << "p6: " << p6 << endl;
    cout << boolalpha;
    cout << "(p4 == p6) is " << (p4 == p6) << endl;

    //test the evaluaton
    int x = 5;
    cout << "Evaluating p1 at " << x << " yields: " << p1.evaluate(5) << endl;
    cout << "Evaluating p2 at " << x << " yields: " << p2.evaluate(5) << endl;
        
    Polynomial p7({3, 2, 1});           // 3x^2 + 2x + 1
    cout << "p7: " << p7 << endl;
    cout << "Evaluating p7 at " << x << " yields: " << p7.evaluate(5) << endl;

    cout << boolalpha;
    cout << "(p1 == p2) is " << (p1 == p2) << endl;
    cout << "(p1 != p2) is " << (p1 != p2) << endl;

    Polynomial p8({ 1, 1 });
    Polynomial p9({ -1, 1 });
    Polynomial p10({ 0, 0, 2 });

    //p8 + p9 tests if += does the cleanup()
    //p10 tests if constructor does the cleanup()
    cout << "p8: " << p8 << endl
         << "p9: " << p9 << endl
         << "p10: " << p10 << endl;
    
    cout << "((p8 + p9) == p10) is " << ((p8 + p9) == p10) << endl;
}








// default constructor
Polynomial::Polynomial()
    : header(new Node{nullptr, 0}), trailer(new Node{nullptr, 0}), degree(0) {
        header->next = trailer;
    }

// constructor that takes in vector of coefficients
Polynomial::Polynomial(vector<int> coefficients) 
    : header(new Node{nullptr, 0}), trailer(new Node{nullptr, 0}), degree(0) {
        header->next = trailer;
        Node* curr_node = header;
        bool first_nonzero = false;
        int first_index = 0;

        for (size_t index = 0; index < coefficients.size(); index++) {
            if (coefficients[index] != 0 || first_nonzero) {
                if (!first_nonzero) {
                    first_index = index;
                    first_nonzero = true;
                }
                Node* new_node = new Node{curr_node->next, coefficients[index]};
                curr_node->next = new_node;
                curr_node = curr_node->next;
            }
        }

        if (!first_nonzero) { degree = 0; }
        else { degree = coefficients.size() - first_index - 1; }
    }

// destructor
Polynomial::~Polynomial() {
    Node* curr_node = header->next;
    while (curr_node != trailer) {
        Node* next_node = curr_node->next;
        delete curr_node;
        curr_node = next_node;
    }
    delete header;
    delete trailer;
}

// copy constructor
Polynomial::Polynomial(const Polynomial& other) {
    degree = other.degree;
    header = new Node{nullptr, 0};
    trailer = new Node{nullptr, 0};
    header->next = trailer;

    Node* curr_node = other.header->next;
    Node* prev_node = header;
    while (curr_node != other.trailer) {
        Node* new_node = new Node{nullptr, curr_node->data};
        prev_node->next = new_node;
        prev_node = new_node;
        curr_node = curr_node->next;
    }
    prev_node->next = trailer;
}

// assignment operator
Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this != &other) {
        Node* curr_node = header->next;
        while (curr_node != trailer) {
            Node* next_node = curr_node->next;
            delete curr_node;
            curr_node = next_node;
        }
        delete header;
        delete trailer;

        degree = other.degree;
        header = new Node{nullptr, 0};
        trailer = new Node{nullptr, 0};
        header->next = trailer;

        Node* curr_other = other.header->next;
        Node* prev_node = header;
        while (curr_other != other.trailer) {
            Node* new_node = new Node{nullptr, curr_other->data};
            prev_node->next = new_node;
            prev_node = new_node;
            curr_other = curr_other->next;
        }
        prev_node->next = trailer;
    }
    return *this;
}

// += operator
Polynomial& Polynomial::operator+=(const Polynomial& other) {
    if (degree < other.degree) { 
        int num_zeros = other.degree - degree;
        for (int num = 0; num < num_zeros; num++) {
            Node* new_node = new Node{header->next, 0};
            header->next = new_node;
        }
    }
    else if (other.degree < degree) { 
        int num_zeros = degree - other.degree;
        for (int num = 0; num < num_zeros; num++) {
            Node* new_node = new Node{other.header->next, 0};
            other.header->next = new_node;
        }
    }

    Node* curr_node = header->next;
    Node* other_node = other.header->next;

    while (curr_node != trailer) {
        curr_node->data += other_node->data;
        curr_node = curr_node->next;
        other_node = other_node->next;
    }

    return *this;
}

// + operator
Polynomial Polynomial::operator+(const Polynomial& other) {
    Polynomial sum = *this;
    sum += other;
    return sum;
}

// == operator
bool Polynomial::operator==(const Polynomial& other) {
    this->strip_leading_zeros();
    Node* curr_node = header->next;
    Node* other_node = other.header->next;
    while (curr_node != trailer) {
        if (curr_node->data != other_node->data) {
            return false;
        }
        curr_node = curr_node->next;
        other_node = other_node->next;
    }
    return true;
}

// != operator
bool Polynomial::operator!=(const Polynomial& other) {
    return !(*this == other);
}

// output operator
ostream& operator<<(ostream& os, const Polynomial& poly) {
    if (poly.header->next == poly.trailer) {
        os << "0";
        return os;
    }

    Polynomial::Node* curr_node = poly.header->next;
    int curr_exponent = poly.degree;
    bool printed_first = false;

    while (curr_node != poly.trailer) {
        if (curr_node->data != 0) {
            if (printed_first) {
                os << (curr_node->data > 0 ? "+" : "");
            }
            if (curr_node->data != 1 && curr_node->data != -1 
            || curr_exponent == 0) { 
                os << curr_node->data; 
            } 
            else if (curr_node->data == -1) {
                os << "-";
            }

            if (curr_exponent > 0) {
                os << "x";
                if (curr_exponent > 1) {
                    os << "^" << curr_exponent;
                }
            }

            printed_first = true;
        }
        curr_node = curr_node->next;
        curr_exponent--;
    }

    return os;
}

// evaluates the polynomial for the value of x passed in
int Polynomial::evaluate(int val) {
    Node* curr_node = header->next;
    int result = 0;
    while (curr_node != trailer) {
        result = result * val + curr_node->data;
        curr_node = curr_node->next;
    }
    return result;
}

// strips leading zeros for == operator
void Polynomial::strip_leading_zeros() {
    Node* curr_node = header->next;
    Node* prev_node = header;

    while (curr_node != trailer && curr_node->data == 0) {
        prev_node->next = curr_node->next;
        delete curr_node;                
        curr_node = prev_node->next;
    }
}