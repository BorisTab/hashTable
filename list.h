//
// Created by boristab on 06.11.2019.
//
#include <iostream>
#include <fstream>
#include <cassert>
//#include <cstring>

enum errors {
    SUCCESS = 0,
    FILE_OPEN_FAILED = 1,
};

const char dumpFilePath[FILENAME_MAX] = "../ListDumpFile.txt";

int myStrcmp(const char *str1, const char *str2) {
    while (*str1 != '\0' || *str2 != '\0') {
        if (*str1 != *str2)
            return *str1 - *str2;

        ++str1;
        ++str2;
    }

    return *(str1 - 1) - *(str2 - 1);
}

template <class elemType>
class Node {
public:
    Node <elemType> *nextPointer = nullptr;
    Node <elemType> *previousPointer = nullptr;
    elemType value = {};

    explicit Node(elemType value) {
        this->value = value;
    }
};

template <class elemType>
class List {
private:
    Node <elemType> *head = nullptr;
    Node <elemType> *tail = nullptr;
    size_t size = 0;

    Node <elemType> *newNode(elemType val) {
        return new Node <elemType> (val);
    }

public:
    explicit List(elemType val){
        auto *node = newNode(val);

        head = node;
        tail = node;
        size = 1;
    }

    List() = default;

    ~List() {
        if (!head) return;

        Node <elemType> *currentNode = head;
        while (currentNode->nextPointer) {
            currentNode = currentNode->nextPointer;
            delete currentNode->previousPointer;
        }
        delete tail;
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void pushBack(elemType val) {
        auto *node = newNode(val);

        if (size == 0) {
            head = node;
        } else {
            node->nextPointer = nullptr;
            node->previousPointer = tail;
            tail->nextPointer = node;
        }

        tail = node;
        size++;
    }

    void pushFront(elemType val) {
        auto *node = newNode(val);

        if (size == 0) {
            tail = node;
        } else {
            node->nextPointer = head;
            node->previousPointer = nullptr;
            head->previousPointer = node;
        }

        head = node;
        size++;
    }

    void insertAfter(Node <elemType> *insertAfterNode, elemType val) {
        assert(insertAfterNode);

        if (insertAfterNode == tail) {
            pushBack(val);
            return;
        }

        auto *node = newNode(val);

        node->nextPointer = insertAfterNode->nextPointer;
        node->previousPointer = insertAfterNode;
        (*(insertAfterNode->nextPointer)).previousPointer = node;
        insertAfterNode->nextPointer = node;
        size++;
    }

    void insertBefore(Node <elemType> *insertBeforeNode, elemType val) {
        assert(insertBeforeNode);

        if (insertBeforeNode == head) {
            pushFront(val);
            return;
        }

        insertAfter(insertBeforeNode->previousPointer, val);
    }

    Node <elemType> *getHead() {
        return head;
    }

    Node <elemType> *getTail() {
        return tail;
    }

    size_t getSize() {
        return size;
    }

    Node <elemType> *elemAfter(const Node <elemType> *node) {
        assert(node);

        return node->nextPointer;
    }

    Node <elemType> *elemBefore(const Node <elemType> *node) {
        assert(node);

        return node->previousPointer;
    }

    void deleteElem(Node <elemType> *node) {
        assert(node);

        if (node->previousPointer) {
            (*(node->previousPointer)).nextPointer = node->nextPointer;
        }

        if (node->nextPointer) {
            (*(node->nextPointer)).previousPointer = node->previousPointer;

        }

        size--;
        delete node;
    }

    void eraseStr(char *str) {
        Node <elemType> *node = findStrNode(str);

        if (node)
            deleteElem(node);
    }

    Node <elemType> *findElem(elemType val) {
        Node <elemType> *valPointer = nullptr;

        Node <elemType> *currentPointer = head;
        while (currentPointer) {
            if (currentPointer->value == val) {
                valPointer = currentPointer;
                break;
            }
            currentPointer = currentPointer->nextPointer;
        }

        return valPointer;
    }

    Node <elemType> *findStrNode(elemType val) {
        Node <elemType> *valPointer = nullptr;

        Node <elemType> *currentPointer = head;
        while (currentPointer) {
//            int equal = 0;
//
//            asm (".intel_syntax noprefix\n"
//                 "            xor rax, rax\n"
//                 "            mov   bl, 0\n"
//                 "            cld\n"
//
//                 ".compare:   cmpsb\n"
//                 "            jne   .endCmp\n"
//                 "            dec   rsi\n"
//                 "            lodsb\n"
//                 "            cmp   bl, al\n"
//                 "            jne    .compare\n"
//
//                 ".endCmp:    mov   al, [rsi - 1]\n"
//                 "            sub   al, [rdi - 1]\n"
//                 ".att_syntax prefix\n"
//            : "=a"(equal)
//            : "S"(currentPointer->value), "D" (val)
//            : "rbx"
//            );

            if (!myStrcmp(currentPointer->value, val)) {
                valPointer = currentPointer;
                break;
            }
            currentPointer = currentPointer->nextPointer;
        }

        return valPointer;
    }

    elemType findStr(elemType val) {
        Node <elemType> *valPointer = findStrNode(val);

        if (valPointer)
            return valPointer->value;
        return nullptr;
    }

    Node <elemType> *getElemById(size_t id) {
        Node <elemType> *idNode = head;

        for (size_t i = 0; i < id; i++) {
            idNode = idNode->nextPointer;
        }

        return idNode;
    }

    void clear() {
        if (!head) return;

        Node <elemType> *currentPointer = head;

        while (currentPointer) {
            Node <elemType> *nextNode =  currentPointer->nextPointer;
            deleteElem(currentPointer);
            currentPointer = nextNode;
        }

        head = nullptr;
        tail = nullptr;
    }

    elemType getValue (const Node <elemType> *node) {
        return node->value;
    }

    void dump() {
        std::ofstream  dumpFile (dumpFilePath);
        if (!dumpFile) {
            std::cout << "File isn't open\n";
            exit(FILE_OPEN_FAILED);
        }

        dumpFile << "digraph G{\n";
        dumpFile << "\"Size: " << size <<"\";\n";

        if (head) {
            Node <elemType> *currentNode = head;
            while (currentNode) {
                dumpFile << "node_" << currentNode << " [shape=record, label=\" " <<  currentNode << " | Val: " << currentNode->value <<  " \"];\n";

                currentNode = currentNode->nextPointer;
            }

            dumpFile << "head->node_" << head << ";\n";
            currentNode = head;
            while (currentNode->nextPointer) {
                dumpFile << "node_" << currentNode << "->node_" << currentNode->nextPointer << ";\n";
                dumpFile << "node_" << currentNode->nextPointer << "->node_" << currentNode << ";\n";
                currentNode = currentNode->nextPointer;
            }
            dumpFile << "node_" << tail << "->tail;\n";
        }

        dumpFile << "}\n";

        dumpFile.close();

        char dotCommand[FILENAME_MAX] = "";
        sprintf(dotCommand, "dot -Tpng -O %s", dumpFilePath);
        std::system(dotCommand);
    }

    void termDump() {
        Node <elemType> *currentNode = head;
        while (currentNode) {
            std::cout << currentNode->value << ' ';

            currentNode = currentNode->nextPointer;
        }
        std::cout << '\n';
    }

    int listOk() {
        size_t sizeChecker = 0;
        Node <elemType> *currentNode = head;
        int errCount = 0;

        while (currentNode && sizeChecker <= size) {
            sizeChecker++;
            currentNode = currentNode->nextPointer;
        }

        if (sizeChecker != size) {
            printf("Wrong size:\n "
                   "  got: %ld\n"
                   "await: %ld\n", sizeChecker, size);
            errCount++;
        }

        currentNode = head;
        for (size_t i = 1; i < size; i++) {
            if (!currentNode) {
                printf("Wrong pointer to node\n");
                errCount++;
            }
            currentNode = currentNode->nextPointer;
        }

        if (currentNode != tail) {
            printf("List doesn't reach the tail\n");
            errCount++;
        }

        for (size_t i = 1; i < size; i++) {
            if (!currentNode) {
                printf("Wrong pointer to node\n");
                errCount++;
            }
            currentNode = currentNode->previousPointer;
        }

        if (currentNode != head) {
            printf("List doesn't reach the head\n");
            errCount++;
        }

        if(errCount) {
            printf("%d errors\n", errCount);
        } else {
            printf("List is OK\n");
        }

        return errCount;
    }
};
