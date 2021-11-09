#ifndef __CIRCULAR_LIST_HPP__
#define __CIRCULAR_LIST_HPP__
typedef unsigned long ul;
typedef long long ll;
#include <iterator>
//clase template de una lista circular doblemente enlazada
template <class T>
class CCircularList
{
private:
    struct node
    {
    public:
        node* _next;
        node* _back;
        T _element;

        node(T element, node* _next = nullptr, node* _back = nullptr) :
            _element(element), _next(_next), _back(_back) {}
        ~node() { }
    };
    
    ul _size;
    node* _start;

    void push(node*& nnode) { this->_start->_next = this->_start->_back = this->_start = nnode; }
    void pop() { delete this->_start; this->_start = nullptr; }

public:
    //class iterator
    class iterator
    {
    private:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        ll _index;
        node* aux;
    public:
        iterator(ll _index, node*& aux) : _index(_index), aux(aux) {}
        iterator& operator++ () { _index++; aux = aux->_next; return *this; }
        bool operator!= (const iterator& _iterator) { return this->_index != _iterator._index; }
        iterator& operator-- () { _index--; aux = aux->_back; return *this; }
        iterator operator-(const iterator& _iterator) { return iterator(this->_index - _iterator._index, this->aux); }
        iterator operator+(const iterator& _iterator) { return iterator(this->_index + _iterator._index, this->aux); }
        T& operator* () { return aux->_element; }
        T* operator->() { return &aux->_element; }
    };
    //return the start iterador
    iterator begin() { return iterator(0, this->_start); }
    //return the end iterator
    iterator end() { return iterator(this->_size, this->_start); }
    //return the last element inserted like start iterator
    iterator rbegin() {
        if (this->_size == 0)
            return iterator(this->_size - 1, this->_start);
        return iterator(this->_size - 1, this->_start->_back);
    }
    //return the start like end iterator  
    iterator rend() { return iterator(-1, this->_start); }

    //constructor
    CCircularList() : _start(nullptr), _size(0) {}
    //destructor
    ~CCircularList() { delete this->_start; }
    //geter del final(_end)
    //geter del tamaño(_size)
    ul size() const { return this->_size; }

    //poner al principio
    void push_front(T elemment) {
        node* nnode = new node(elemment);
        if (this->_size == 0)
            this->push(nnode);
        else {
            nnode->_back = this->_start->_back;
            this->_start->_back->_next = nnode;
            this->_start->_back = nnode;
            nnode->_next = this->_start;
            this->_start = nnode;
        }
        ++this->_size;
    }

    //poner al final
    void push_back(T element) {
        node* nnode = new node(element);
        if (this->_size == 0)
            push(nnode);
        else {
            this->_start->_back->_next = nnode;
            nnode->_back = this->_start->_back;
            nnode->_next = this->_start;
            this->_start->_back = nnode;

        }
        ++this->_size;
    }

    //insertar en posicion
    void insert(T element, ul position) {
        if (this->_size == 0 && position != 0)return;
        else if (position < 0 || position >= this->_size)return;
        if (position == 0 && this->_size == 0)
            this->push(new node(element)), ++this->_size;
        else if (position == 0)
            this->push_front(element);
        else if (position == this->_size - 1)
            this->push_back(element);
        else {
            node* nnode = new node(element);
            node* aux = this->_start;
            while (--position)
                aux = aux->_next;

            nnode->_next = aux->_next;
            nnode->_back = aux;
            aux->_next->_back = nnode;
            aux->_next = nnode;
            ++this->_size;
        }
    }

    //eliminar el final
    void pop_back() {
        if (this->_size == 0)return;
        if (this->_size == 1)
            this->pop();
        else {
            node* aux = this->_start->_back->_back;
            aux->_next = this->_start;
            this->_start->_back->_back = nullptr;
            this->_start->_back->_next = nullptr;
            delete this->_start->_back;
            this->_start->_back = aux;
        }
        --this->_size;
    }

    //eliminar el principio
    void pop_front() {
        if (this->_size == 0)return;
        if (this->_size == 1)
            pop();
        else {
            node* aux = this->_start->_next;
            aux->_back = this->_start->_back;
            this->_start->_back->_next = aux;
            this->_start->_next = nullptr;
            this->_start->_back = nullptr;
            delete this->_start;
            this->_start = aux;

        }
        --this->_size;
    }

    //borrar en posicion
    void erase(ul position) {
        if (this->_size == 0) return;
        if (position == 0) {
            this->pop_front();
            return;
        }
        else if (position == this->_size - 1) {
            this->pop_back();
            return;
        }

        node* aux = this->_start;
        while (--position)
            aux = aux->_next;

        //se boora el siguiente de aux
        aux->_next->_next->_back = aux;
        aux->_next = aux->_next->_next;
        aux->_next->_next = nullptr;
        aux->_next->_back = nullptr;
        delete aux->_next;
        --this->_size;

    }
};
#endif // __CIRCULAR_LIST_HPP__
