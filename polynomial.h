// Zoe Dauphinee, hw 08, header file for Polynomial class

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

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
        Polynomial& operator+(const Polynomial& other);
        bool operator==(const Polynomial& other);
        bool operator!=(const Polynomial& other);
        friend ostream& operator<<(ostream& os, const Polynomial& poly);
        int evaluate(int val);
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

#endif