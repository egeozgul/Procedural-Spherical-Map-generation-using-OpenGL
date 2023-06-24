#pragma once

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef glm::vec3 Key;
template <typename Value, typename Hash = std::hash<Key>>
class hashtable 
{
public:
    hashtable() 
    {
        total_elements = 0;
        total_buckets = 1;
        buckets = new Node * [total_buckets]();
    }

    void put(Key key, const Value& val) {
        size_t bucket = getBucket(key);
        Node* curPos = searchNode(bucket, key);

        if (curPos != nullptr) {
            curPos->val = val;
        }
        else {
            if (total_elements == total_buckets) {
                resize();
                bucket = getBucket(key);
            }
            insertNode(bucket, key, val);
            ++total_elements;
        }
    }

    Value get(Key key) const {
        Node* curPos = searchNode(getBucket(key), key);
        if (curPos != nullptr)
            return curPos->val;
        return Value();
    }

    bool contains(Key key) const {
        return searchNode(getBucket(key), key) != nullptr;
    }

    void erase(Key key) {
        size_t bucket = getBucket(key);
        Node* cur = buckets[bucket];
        Node* prev = nullptr;

        while (cur) {
            if (cur->key == key)
                return deleteNode(bucket, cur, prev);
            
            prev = cur;
            cur = cur->next;
        }
    }

    size_t capacity() const {
        return total_buckets;
    }

    size_t size() {
        return total_elements;
    }

    void clear()
    {
        for (unsigned int i = 0; i < total_buckets; ++i) 
        {
            deleteList(buckets[i]);
            buckets[i] = NULL;
        }
    }

    ~hashtable() {
        for (unsigned int i = 0; i < total_buckets; ++i) {
            deleteList(buckets[i]);
        }
        delete[] buckets;
    }

    // void debugPrint() {
    //  for (unsigned int i = 0; i < total_buckets; ++i) {
    //      Node *cur = buckets[i];
    //      std::cout << i << "\t:";
    //      while (cur) {
    //          std::cout << cur->key << "," << cur->val << "--";
    //          cur = cur->next;
    //      }
    //      std::cout << std::endl;
    //  }
    //  std::cout << "------------------------------------------\n\n";
    // }
    struct Node {
        Key key;
        Value val;
        Node* next;

        Node(Key key, const Value& val) : key(key), val(val), next(nullptr) {}
    };

    Node** buckets;
private:

    int customHasher(glm::vec3 key)const
    {
        int prime[3] = { 37,89,97 };
        return ((key.x * prime[0] + key.y) * prime[1] + key.z) * prime[2];
    }

    size_t getBucket(glm::vec3 key)const 
    {
        //return 1 % total_buckets;
        return customHasher(key) % total_buckets;
    }

    Node* searchNode(size_t bucket, Key key)const {
        Node* cur = buckets[bucket];
        while (cur != nullptr) {
            if (key == cur->key)
                return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void insertNode(size_t bucket, Key key, const Value& val) const {
        Node* newNode = new Node(key, val);
        pushNode(buckets, bucket, newNode);
    }

    void pushNode(Node** buckets, size_t bucket, Node* node)const {
        node->next = buckets[bucket];
        buckets[bucket] = node;
    }

    void resize()  {
        total_buckets *= 2;
        Node** newBuckets = new Node * [total_buckets]();

        for (unsigned int i = 0; i < total_buckets / 2; ++i) {
            Node* cur = buckets[i];
            Node* next;
            while (cur != nullptr) {
                next = cur->next;
                pushNode(newBuckets, getBucket(cur->key), cur);
                cur = next;
            }
        }

        delete[] buckets;
        buckets = newBuckets;
    }

    void deleteList(Node* cur)
    {
        Node* next;
        while (cur) {
            next = cur->next;
            delete cur;
            cur = next;
            total_elements--;
        }
    }

    void deleteNode(size_t bucket, Node* cur, Node* prev) {
        if (prev == nullptr)
            buckets[bucket] = cur->next;
        else
            prev->next = cur->next;
        total_elements--;
        delete cur;
    }



    //Hash hasher;
    size_t total_buckets;
    size_t total_elements;
};
