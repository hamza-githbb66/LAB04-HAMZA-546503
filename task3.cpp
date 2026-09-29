#include <iostream>
#include <string>
using namespace std;

struct Bit {
    int bit;      // 0 or 1
    Bit* next;
    Bit* prev;
};

struct Number {
    Bit* head;
    Bit* tail;
};

Number newNumber() {
    Number n;
    n.head = NULL;
    n.tail = NULL;
    return n;
}

// add a bit at the front
void addFront(Number &n, int b) {
    Bit* node = new Bit;
    node->bit = b;
    node->prev = NULL;
    node->next = n.head;

    if (n.head == NULL) {
        n.tail = node;
    } else {
        n.head->prev = node;
    }
    n.head = node;
}

// add a bit at the end
void addEnd(Number &n, int b) {
    Bit* node = new Bit;
    node->bit = b;
    node->next = NULL;
    node->prev = n.tail;

    if (n.tail == NULL) {
        n.head = node;
    } else {
        n.tail->next = node;
    }
    n.tail = node;
}

// count how many bits are in the list
int lengthOf(Number n) {
    int count = 0;
    Bit* temp = n.head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// adding zeros at the front until the length is a multiple of 8
void fixToBlocks(Number &n) {
    int len = lengthOf(n);
    while (len % 8 != 0) {
        addFront(n, 0);
        len++;
    }
}

// delete all nodes so there is no memory leak
void freeNumber(Number &n) {
    Bit* temp = n.head;
    while (temp != NULL) {
        Bit* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }
    n.head = NULL;
    n.tail = NULL;
}

// making a copy of a number
Number copyNumber(Number n) {
    Number c = newNumber();
    Bit* temp = n.head;
    while (temp != NULL) {
        addEnd(c, temp->bit);
        temp = temp->next;
    }
    return c;
}

// check that the input only has 0 and 1
bool isValidBinary(string s) {
    if (s.length() == 0) {
        return false;
    }
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != '0' && s[i] != '1') {
            return false;
        }
    }
    return true;
}

// Store Binary Number: put each bit in a node, then pad to 8-bit blocks
Number storeBinary(string s) {
    Number n = newNumber();
    for (int i = 0; i < s.length(); i++) {
        addEnd(n, s[i] - '0');   // char '0'/'1' to int 0/1
    }
    fixToBlocks(n);   // 9 bits becomes 16 bits, etc.
    return n;
}

// show the number, with a space after every 8 bits
void displayBinary(Number n) {
    Bit* temp = n.head;
    int count = 0;
    while (temp != NULL) {
        cout << temp->bit;
        count++;
        if (count % 8 == 0 && temp->next != NULL) {
            cout << " ";
        }
        temp = temp->next;
    }
    cout << endl;
}

// 1's Complement
Number onesComplement(Number n) {
    Number c = copyNumber(n);
    Bit* temp = c.head;
    while (temp != NULL) {
        if (temp->bit == 0) {
            temp->bit = 1;
        } else {
            temp->bit = 0;
        }
        temp = temp->next;
    }
    return c;
}

Number addBinary(Number a, Number b, bool keepCarry) {
    Number result = newNumber();
    Bit* p = a.tail;
    Bit* q = b.tail;
    int carry = 0;

    while (p != NULL || q != NULL) {
        int x = 0;
        int y = 0;
        if (p != NULL) {
            x = p->bit;
            p = p->prev;
        }
        if (q != NULL) {
            y = q->bit;
            q = q->prev;
        }
        int sum = x + y + carry;
        addFront(result, sum % 2);   
        carry = sum / 2;             
    }

    if (carry == 1 && keepCarry) {
        addFront(result, 1);
    }
    fixToBlocks(result);
    return result;
}

// 2's Complement
Number twosComplement(Number n) {
    Number ones = onesComplement(n);
    Number one = storeBinary("1");
    Number result = addBinary(ones, one, false);
    freeNumber(ones);
    freeNumber(one);
    return result;
}

Number multiplyBinary(Number a, Number b) {
    Number product = storeBinary("0");
    Number shifted = copyNumber(a);

    Bit* q = b.tail;
    while (q != NULL) {
        if (q->bit == 1) {
            Number temp = addBinary(product, shifted, true);
            freeNumber(product);
            product = temp;
        }
        addEnd(shifted, 0);   
        q = q->prev;
    }

    freeNumber(shifted);
    fixToBlocks(product);
    return product;
}

// Conversion to Decimal
long long toDecimal(Number n) {
    long long value = 0;
    Bit* temp = n.head;
    while (temp != NULL) {
        value = value * 2 + temp->bit;
        temp = temp->next;
    }
    return value;
}

int main() {
    Number A = newNumber();
    Number B = newNumber();
    Number R = newNumber();    
    int choice;

    do {
        cout << "\n===== BINARY CALCULATOR =====" << endl;
        cout << "1. Store number A" << endl;
        cout << "2. Store number B" << endl;
        cout << "3. Display A and B" << endl;
        cout << "4. 1's Complement of A" << endl;
        cout << "5. 2's Complement of A" << endl;
        cout << "6. Add A + B" << endl;
        cout << "7. Multiply A * B" << endl;
        cout << "8. Convert to Decimal" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            string s;
            cout << "Enter binary number: ";
            cin >> s;
            if (!isValidBinary(s)) {
                cout << "Invalid input, use only 0 and 1." << endl;
            } else if (choice == 1) {
                freeNumber(A);
                A = storeBinary(s);
                cout << "A stored: ";
                displayBinary(A);
            } else {
                freeNumber(B);
                B = storeBinary(s);
                cout << "B stored: ";
                displayBinary(B);
            }
        } else if (choice == 3) {
            cout << "A = ";
            displayBinary(A);
            cout << "B = ";
            displayBinary(B);
        } else if (choice == 4) {
            if (A.head == NULL) {
                cout << "Store A first." << endl;
            } else {
                freeNumber(R);
                R = onesComplement(A);
                cout << "1's complement of A = ";
                displayBinary(R);
            }
        } else if (choice == 5) {
            if (A.head == NULL) {
                cout << "Store A first." << endl;
            } else {
                freeNumber(R);
                R = twosComplement(A);
                cout << "2's complement of A = ";
                displayBinary(R);
            }
        } else if (choice == 6) {
            if (A.head == NULL || B.head == NULL) {
                cout << "Store both A and B first." << endl;
            } else {
                freeNumber(R);
                R = addBinary(A, B, true);
                cout << "A + B = ";
                displayBinary(R);
            }
        } else if (choice == 7) {
            if (A.head == NULL || B.head == NULL) {
                cout << "Store both A and B first." << endl;
            } else {
                freeNumber(R);
                R = multiplyBinary(A, B);
                cout << "A * B = ";
                displayBinary(R);
            }
        } else if (choice == 8) {
            int which;
            cout << "Convert which one? 1 = A, 2 = B, 3 = last result: ";
            cin >> which;
            if (which == 1 && A.head != NULL) {
                cout << "Decimal = " << toDecimal(A) << endl;
            } else if (which == 2 && B.head != NULL) {
                cout << "Decimal = " << toDecimal(B) << endl;
            } else if (which == 3 && R.head != NULL) {
                cout << "Decimal = " << toDecimal(R) << endl;
            } else {
                cout << "Nothing stored there." << endl;
            }
        } else if (choice == 0) {
            cout << "Exiting..." << endl;
        } else {
            cout << "Invalid choice." << endl;
        }

    } while (choice != 0);

    // free everything at the end
    freeNumber(A);
    freeNumber(B);
    freeNumber(R);
    return 0;
}