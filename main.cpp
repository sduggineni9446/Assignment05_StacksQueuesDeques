#include <iostream>
#include <string>
#include <random>

using namespace std;

struct Property {
    string name;
    int cost;
    string owner;
};

struct Node {
    Property property;
    Node* next;

    Node(const string& propertyName, int propertyCost)
        : property{propertyName, propertyCost, "Unowned"},
          next(nullptr) {}
};

class CircularLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    CircularLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~CircularLinkedList() {
        clear();
    }

    void addProperty(const string& name, int cost) {
        Node* newNode = new Node(name, cost);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
            newNode->next = head;
        } else {
            newNode->next = head;
            tail->next = newNode;
            tail = newNode;
        }

        size++;
    }

    Node* searchProperty(const string& name) const {
        if (head == nullptr) {
            return nullptr;
        }

        Node* current = head;

        do {
            if (current->property.name == name) {
                return current;
            }

            current = current->next;
        } while (current != head);

        return nullptr;
    }

    bool removeProperty(const string& name) {
        if (head == nullptr) {
            return false;
        }

        Node* current = head;
        Node* previous = tail;

        do {
            if (current->property.name == name) {
                if (current == head && current == tail) {
                    head = nullptr;
                    tail = nullptr;
                } else {
                    previous->next = current->next;

                    if (current == head) {
                        head = current->next;
                    }

                    if (current == tail) {
                        tail = previous;
                    }
                }

                delete current;
                size--;
                return true;
            }

            previous = current;
            current = current->next;
        } while (current != head);

        return false;
    }

    void printProperties() const {
        if (head == nullptr) {
            cout << "The board is empty.\n";
            return;
        }

        Node* current = head;
        int position = 0;

        do {
            cout << position << ". "
                 << current->property.name
                 << " | Cost: $" << current->property.cost
                 << " | Owner: " << current->property.owner
                 << '\n';

            current = current->next;
            position++;
        } while (current != head);
    }

    Node* getHead() const {
        return head;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        if (head == nullptr) {
            return;
        }

        Node* current = head->next;

        while (current != head) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }

        delete head;
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
};

int rollDice(mt19937& generator) {
    uniform_int_distribution<int> dice(1, 6);
    return dice(generator) + dice(generator);
}

int main() {
    CircularLinkedList board;

    board.addProperty("Mediterranean Avenue", 60);
    board.addProperty("Baltic Avenue", 60);
    board.addProperty("Oriental Avenue", 100);
    board.addProperty("Vermont Avenue", 100);
    board.addProperty("Connecticut Avenue", 120);
    board.addProperty("St. Charles Place", 140);
    board.addProperty("States Avenue", 140);
    board.addProperty("Virginia Avenue", 160);
    board.addProperty("St. James Place", 180);
    board.addProperty("Tennessee Avenue", 180);

    cout << "CIRCULAR MONOPOLY BOARD\n";
    cout << "=======================\n";
    board.printProperties();

    cout << "\nSearching for a property:\n";
    Node* found = board.searchProperty("Oriental Avenue");

    if (found != nullptr) {
        cout << "Found: " << found->property.name
             << " costing $" << found->property.cost << '\n';
    } else {
        cout << "Property not found.\n";
    }

    cout << "\nStarting game simulation...\n";
    cout << "===========================\n";

    const int numberOfPlayers = 2;
    string players[numberOfPlayers] = {"Player 1", "Player 2"};
    int positions[numberOfPlayers] = {0, 0};

    mt19937 generator(random_device{}());

    for (int turn = 1; turn <= 10; turn++) {
        int playerIndex = (turn - 1) % numberOfPlayers;
        int roll = rollDice(generator);

        positions[playerIndex] =
            (positions[playerIndex] + roll) % board.getSize();

        Node* currentProperty = board.getHead();

        for (int i = 0; i < positions[playerIndex]; i++) {
            currentProperty = currentProperty->next;
        }

        cout << "\nTurn " << turn << '\n';
        cout << players[playerIndex] << " rolled " << roll << ".\n";
        cout << players[playerIndex] << " landed on "
             << currentProperty->property.name << ".\n";

        if (currentProperty->property.owner == "Unowned") {
            currentProperty->property.owner = players[playerIndex];

            cout << players[playerIndex]
                 << " bought the property for $"
                 << currentProperty->property.cost << ".\n";
        } else {
            cout << "This property is already owned by "
                 << currentProperty->property.owner << ".\n";
        }
    }

    cout << "\nFINAL BOARD:\n";
    cout << "============\n";
    board.printProperties();

    cout << "\nTesting removal:\n";
    if (board.removeProperty("Baltic Avenue")) {
        cout << "Baltic Avenue was removed successfully.\n";
    } else {
        cout << "Baltic Avenue was not found.\n";
    }

    cout << "\nBoard after removal:\n";
    board.printProperties();

    return 0;
}