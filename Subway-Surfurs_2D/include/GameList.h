#ifndef GAMELIST_H
#define GAMELIST_H

#include <stdexcept>

template <typename T>
class GameList {
private:
    T* data;
    int capacity;
    int count;

    void expand() {
        int newCapacity = (capacity == 0) ? 5 : capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        if (capacity > 0) delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    GameList() {
        capacity = 10;
        count = 0;
        data = new T[capacity];
    }

    ~GameList() {
        if (data) delete[] data;
    }

    void add(T item) {
        if (count >= capacity) expand();
        data[count++] = item;
    }

    T get(int index) {
        if (index >= 0 && index < count) return data[index];
        throw std::out_of_range("Index out of bounds");
    }

    void remove(int index) {
        if (index < 0 || index >= count) return;
        for (int i = index; i < count - 1; i++) {
            data[i] = data[i + 1];
        }
        count--;
    }

    int getSize() const { return count; }
};
#endif
