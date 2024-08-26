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
        inline T* getPrev() {
            return _prev;
        }

       private:
        T* _prev = nullptr;
    };

    // добавить
    bool add(T& node) {
        return add(&node);
    }

    // добавить
    bool add(T* node) {
        if (!node || has(node)) return false;
        node->_prev = _last;
        _last = node;
        return true;
    }

    // список содержит
    bool has(T& node) {
        return has(&node);
    }

    // список содержит
    bool has(T* node) {
        T* p = _last;
        while (p) {
            if (p == node) return true;
            p = p->_prev;
        }
        return false;
    }

    // удалить
    void remove(T& node) {
        remove(&node);
    }

    // удалить
    void remove(T* node) {
        if (!node) return;
        if (_last == node) {
            _last = _last->_prev;
        } else {
            T* p = _last;
            while (p) {
                if (p->_prev == node) {
                    p->_prev = p->_prev->_prev;
                    break;
                }
                p = p->_prev;
            }
        }
    }

    // длина списка
    size_t length() {
        size_t len = 0;
        T* p = _last;
        while (p) {
            len++;
            p = p->_prev;
        }
        return len;
    }

    // очистить список
    void clear() {
        _last = nullptr;
    }

    // получить последний элемент в списке
    inline T* getLast() {
        return _last;
    }

   private:
    T* _last = nullptr;
};

}  // namespace looper