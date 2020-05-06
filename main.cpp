#include <iostream>
#include "list.h"
#include "fileRead.h"
#include <cstring>

extern "C" unsigned int hash5asm (char * str);

class HashTable {
private:
    int tableSize = 0;
    unsigned int (*hash) (char *) = nullptr;
    List <char *> **table = nullptr;

    template <class ValueType>
    class TableIterator : public std::iterator <std::input_iterator_tag, ValueType> {
    public:
        ValueType *ptr = nullptr;

        explicit TableIterator(ValueType *ptr) :
        ptr(ptr) {}

        TableIterator(const TableIterator &it) :
        ptr(it.ptr) {}

        bool operator!= (TableIterator const& other) const {
            return ptr != other.ptr;
        }

        bool operator== (TableIterator const& other) const {
            return ptr == other.ptr;
        }

        typename TableIterator::reference operator*() const {
            return *ptr;
        }

        TableIterator &operator++() {
            ++ptr;
            return *this;
        }
    };

public:
    HashTable(int elemNum, int tableSize, unsigned int (*hash) (char *), char **words) :
        tableSize(tableSize),
        hash(hash) {
        table = new List <char *> *[tableSize] ();

        for (int i = 0; i < tableSize; ++i) {
            table[i] = new List <char *> ();
        }

        for (size_t i = 0; i < elemNum; ++i) {
            insert(words[i]);
        }
    }

    HashTable(int tableSize, unsigned int (*hash) (char *)) :
        tableSize(tableSize),
        hash(hash) {
        table = new List <char *> *[tableSize] ();

        for (int i = 0; i < tableSize; ++i) {
            table[i] = new List <char *> ();
        }
    }

    typedef TableIterator <List <char *> *> iterator;
    typedef TableIterator <List<char *> * const> constIterator;

    iterator begin() {
        return iterator(table);
    }

    iterator end() {
        return iterator(table + tableSize);
    }

    constIterator begin() const {
        return constIterator(table);
    }

    constIterator end() const {
        return constIterator(table + tableSize);
    }

    void insert(char *word) {
        int position = hash(word) % tableSize;

        if (!table[position]->findStr(word))
            table[position]->pushFront(word);
    }

    void erase(char *word) {
        int position = hash(word) % tableSize;

        table[position]->eraseStr(word);
    }

    bool find(char *word) {
        int position = hash(word) % tableSize;

        return table[position]->findStr(word);
    }

    int size() {
        return tableSize;
    }

    void dump() {
        for (int i = 0; i < tableSize; ++i) {
            printf("%5d. ", i);
            table[i]->termDump();
        }
    }

    ~HashTable() {
        for (int i = 0; i < tableSize; ++i) {
            delete table[i];
        }

        delete table;
    }
};

//const 1
unsigned int hash1 (char *word) {
    return 1;
}

//word size
unsigned int hash2(char *word) {
    return strlen(word);
};

//ASCII-sum
unsigned int hash3(char *word) {
    int sum = 0;
    while (*word != '\0') {
        sum += *word;
        ++word;
    }

    return sum;
}

//ASCII-sum / size
unsigned int hash4(char *word) {
    int sum = 0;
    int size = strlen(word);
    while (*word != '\0') {
        sum += *word;
        ++word;
    }
    return sum / size;
}

//xor and circle
unsigned int hash5(char *word) {
    unsigned int hash = 0;
    unsigned int mask = 1;
    mask <<= sizeof(unsigned int)*8 - 1;

    while (*word != '\0') {
        hash ^= *word;
        ++word;

        unsigned int firstBit = hash & mask;
        firstBit >>= sizeof(unsigned int)*8 - 1;

        hash <<= 1;
        hash |= firstBit;
    }

    return hash;
}

//hashH37
unsigned int hash6(char *word) {
    unsigned int hash = 2139062143;

    while (*word != '\0') {
        hash = 37*hash + *word;
        ++word;
    }

    return hash;
}

//hashH37 mod
unsigned int hash7(char *word) {
    unsigned int hash = 2139062143;

    while (*word != '\0') {
        hash = 37*hash + *word;
        ++word;
    }

    hash &= 0x0000ffff;
    return hash;
}

void prepareText(char * text, size_t textSize) {
    for (int i = 0; i < textSize; ++i) {
        if (text[i] < '0' || (text[i] > '9' && text[i] < 'A') || (text[i] > 'Z' && text[i] < 'a') || text[i] > 'z')
            text[i] = '\0';
    }
}


void fillTable(HashTable &hashTable, char *text, size_t textSize) {
    char *curPos = text;
    while (curPos < text + textSize) {
        if (*curPos != ' ' && *curPos != '\0' && (*(curPos - 1) == ' ' || *(curPos - 1) == '\0'))
            hashTable.insert(curPos);

        curPos = strchr(curPos, '\0');
        ++curPos;
    }
}

void writeCSVFile(HashTable &hashTable) {
    char csvFilePath[FILENAME_MAX] = "../hashTable1.csv";
    std::ofstream csvFile;
    csvFile.open(csvFilePath, std::ofstream::app);

    for (auto &it : hashTable) {
        csvFile << it->getSize() << "; ";
    }
    csvFile << '\n';

    csvFile.close();
}

void test(HashTable &hashTable) {
    char *words[] = {
            "that", "they", "drew", "back", "a", "little", "upon", "which", "I", "ordered",
            "our", "last", "pistols", "to", "be", "fired", "off", "in", "one", "volley",
            "and", "after", "that", "we", "gave", "shout", "upon", "this", "the", "wolves",
            "turned", "waste", "sence", "nifnifoo", "wrong", "am", "excuse", "rhfe", "qwerty", "once"
    };

    const int testIterations = 10000000;
    for (int i = 0; i < testIterations; ++i) {
        for (auto & word : words) {
            hashTable.find(word);
        }
    }
}

int main() {
    char textFile[FILENAME_MAX] = "../text.txt";

    size_t textSize = 0;

    char *text = readTextFromFile(textFile, &textSize);
    prepareText(text, textSize);

    HashTable hashTable(4597, hash5asm);
    fillTable(hashTable, text, textSize);

    unsigned int startTime = clock();

    test(hashTable);

    double endTime = (clock() - startTime) / 1000000.0;
    printf("time: %g", endTime);
    return 0;
}
