// Zoe Dauphinee, hw 08, implementation file for Polynomial class

#include "polynomial.h"

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
Polynomial& Polynomial::operator+(const Polynomial& other) {
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