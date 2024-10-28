#ifndef _VECTOR_H
#define _VECTOR_H

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>

template <typename T>
class Vector {
   private:
    T *data;
    size_t size_;
    size_t capacity_;
    void resize(size_t new_capacity);

   public:
    Vector();
    Vector(std::initializer_list<T> init);
    ~Vector();
    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void push_back(const T &value);
    void erase(size_t index);
    void erase(T *value);
    T &operator[](size_t index);
    const T &at(size_t index) const;
    T &front();
    T &back();
    T *begin();
    T *end();
    const T *begin() const;
    const T *end() const;
    void clear();
    std::string toString(std::string (*item2str)(T &));
};

template <typename T>
Vector<T>::Vector() : data(nullptr), size_(0), capacity_(0) {}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> init) : Vector() {
    for (const T &item : init) {
        push_back(item);
    }
}

template <typename T>
void Vector<T>::resize(size_t new_capacity) {
    if (new_capacity <= capacity_) {
        return;  // No need to resize
    }

    T *new_data = new T[new_capacity];
    std::copy(data, data + size_, new_data);
    delete[] data;
    data = new_data;
    capacity_ = new_capacity;
}

template <typename T>
Vector<T>::~Vector() {
    delete[] data;
}

template <typename T>
size_t Vector<T>::size() const {
    return size_;
}

template <typename T>
size_t Vector<T>::capacity() const {
    return capacity_;
}

template <typename T>
bool Vector<T>::empty() const {
    return size_ == 0;
}

template <typename T>
void Vector<T>::push_back(const T &value) {
    if (size_ == capacity_) {
        resize(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data[size_] = value;
    ++size_;
}

template <typename T>
void Vector<T>::erase(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    std::copy(data + index + 1, data + size_, data + index);
    --size_;
}

template <typename T>
T &Vector<T>::operator[](size_t index) {
    return data[index];
}

template <typename T>
const T &Vector<T>::at(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template <typename T>
T &Vector<T>::front() {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return data[0];
}

template <typename T>
T &Vector<T>::back() {
    if (empty()) {
        throw std::out_of_range("Vector is empty");
    }
    return data[size_ - 1];
}

template <typename T>
T *Vector<T>::begin() {
    return data;
}

template <typename T>
T *Vector<T>::end() {
    return data + size_;
}

template <typename T>
const T *Vector<T>::begin() const {
    return data;
}

template <typename T>
const T *Vector<T>::end() const {
    return data + size_;
}

template <typename T>
void Vector<T>::erase(T *e) {
    for (size_t i = 0; i < size_; ++i) {
        if (data[i] == *e) {
            erase(i);
            break;
        }
    }
}

template <typename T>
void Vector<T>::clear() {
    delete[] data;
    data = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template <typename T>
std::string Vector<T>::toString(std::string (*item2str)(T &)) {
    std::string rtn = "[";
    for (size_t i = 0; i < size_; i++) {
        rtn = rtn + ((item2str) ? item2str(data[i]) : std::to_string(data[i])) +
              ";";
    }
    if (size_ > 0) {
        rtn = rtn.substr(0, rtn.length() - 2);
    }
    rtn += "]";
    return rtn;
}

// template <typename InputIterator, typename OutputIterator>
// OutputIterator copy(InputIterator first, InputIterator last,
//                     OutputIterator result) {
//   while (first != last) {
//     *result = *first;
//     ++first;
//     ++result;
//   }
//   return result;
// }
#endif