#pragma once
#include <inttypes.h>
#include <stddef.h>

namespace looper {

template <typename T>
class List {
   public:
    class Node {
        friend class List;

       public:
        inline T* getNext() {
            return _next;
        }

       private:
        T* _next = nullptr;
    };

    // добавить
    bool add(T& node) {
        return add(&node);
    }

    // добавить
    bool add(T* node) {
        if (!node) return false;

        node->_next = _first;
        _first = node;
        return true;
    }

    // удалить
    T* remove(T& node) {
        return remove(&node);
    }

    // удалить. Вернёт указатель на прошлое звено
    T* remove(T* node) {
        if (!node) return nullptr;

        if (_first == node) {
            _first = node->_next;
            return nullptr;
        }

        T* p = _first;
        while (p) {
            if (p->_next == node) {
                p->_next = node->_next;
                return p;
            }
            p = p->_next;
        }
        return nullptr;
    }

    // список содержит
    bool has(T& node) const {
        return has(&node);
    }

    // список содержит
    bool has(T* node) const {
        T* p = _first;
        while (p) {
            if (p == node) return true;
            p = p->_next;
        }
        return false;
    }

    // длина списка
    size_t length() const {
        size_t len = 0;
        T* p = _first;
        while (p) {
            len++;
            p = p->_next;
        }
        return len;
    }

    // очистить список
    void clear() {
        _first = nullptr;
    }

    // получить первый элемент в списке
    inline T* getFirst() const {
        return _first;
    }

   private:
    T* _first = nullptr;
};

}  // namespace looper