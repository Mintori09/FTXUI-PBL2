//
// Created by Tvmth on 12/31/2024.
//

#ifndef MAP_H
#define MAP_H
#include "Vector.h"
#include <string>
#include <iostream>

template <typename K, typename V>
class Map {
public:
    struct Node {
        K key;
        V value;
        Node* next;

        Node(const K& key, const V& value) : key(key), value(value), next(nullptr) {}
    };

    Map() : table_size(10), count(0) {
        table = new Node*[table_size];
        for (int i = 0; i < table_size; i++) {
            table[i] = nullptr;
        }
    }

    ~Map() {
        for (int i = 0; i < table_size; i++) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    // Hàm hash để ánh xạ key thành chỉ số trong bảng
    int hash(const K& key) const {
        int hash_value = 0;
        for (char c : key) {
            hash_value = (hash_value * 31 + c) % table_size;
        }
        return hash_value;
    }

    // Thêm hoặc cập nhật giá trị trong map
    void insert(const K& key, const V& value) {
        int index = hash(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                current->value += value;  // Cộng doanh thu nếu phim đã tồn tại
                return;
            }
            current = current->next;
        }
        Node* new_node = new Node(key, value);
        new_node->next = table[index];
        table[index] = new_node;
        count++;
    }

    // Tìm giá trị theo key
    bool find(const K& key, V& value) const {
        int index = hash(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Duyệt qua các phần tử trong map
    void traverse() const {
        for (int i = 0; i < table_size; i++) {
            Node* current = table[i];
            while (current) {
                std::cout << current->key << ": " << current->value << std::endl;
                current = current->next;
            }
        }
    }

private:
    Node** table;
    int table_size;
    int count;
};

#endif //MAP_H
