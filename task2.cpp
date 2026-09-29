#include <iostream>
using namespace std;

// one node is equal to one person
struct Person {
    int id;
    Person* next;   // points towards the next person
};

// Creating Circle
Person* createCircle(int n) {
    Person* head = NULL;
    Person* tail = NULL;

    for (int i = 1; i <= n; i++) {
        Person* p = new Person;
        p->id = i;
        p->next = NULL;

        if (head == NULL) {
            head = p;
            tail = p;
        } else {
            tail->next = p;
            tail = p;
        }
    }

    // making it circular by pointing last person to the first
    tail->next = head;
    return head;
}

// removing every k-th person and printing the order
// then returns the survivor
Person* eliminate(Person* head, int k) {

    Person* prev = head;
    while (prev->next != head) {
        prev = prev->next;
    }

    Person* curr = head;

    cout << "Elimination order: ";

    // keep going until only one person is left
    while (curr->next != curr) {

        for (int i = 1; i < k; i++) {
            prev = curr;
            curr = curr->next;
        }

        cout << curr->id << " ";

        prev->next = curr->next;
        delete curr;

        curr = prev->next;
    }
    cout << endl;

    return curr;   // the only person left
}

// Displaying the  Survivor
void displaySurvivor(Person* survivor) {
    cout << "Survivor is person: " << survivor->id << endl;
}

int main() {
    int n, k;

    cout << "Enter number of people (N): ";
    cin >> n;
    cout << "Enter step count (k): ";
    cin >> k;

    if (n < 1 || k < 1) {
        cout << "N and k must be at least 1." << endl;
        return 0;
    }

    Person* head = createCircle(n);
    Person* survivor = eliminate(head, k);
    displaySurvivor(survivor);

    // deleting the last remaining node so there is no memory leak
    delete survivor;

    return 0;
}
