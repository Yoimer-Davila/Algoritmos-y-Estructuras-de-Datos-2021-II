#ifndef __DOUBLE_LIST_HPP__
#define __DOUBLE_LIST_HPP__
#include <iterator>
typedef unsigned long ul;
typedef long long ll;
//clase template de una lista doblemente enlazada
template <class T>
class CDoubleList
{
private:
    class node
    {
    public:
        node* _next;
        node* _back;
        T _element;

        node(T element,node* _next = nullptr, node* _back = nullptr) :
            _element(element), _next(_next), _back(_back) {}
        ~node() {}
    };

    ul _size;
    node* _start;
    node* _end;
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
        bool operator!= (const iterator& _iterator) const { return this->_index != _iterator._index; }
        iterator& operator-- () { _index--; aux = aux->_back; return *this; }
        iterator& operator-(const iterator& _iterator) { return iterator(this->_index - _iterator._index, this->aux); }
        iterator& operator+(const iterator& _iterator) { return iterator(this->_index + _iterator._index, this->aux); }
        T& operator* () { return aux->_element; }
        T* operator->() { return &aux->_element; }
    };
    //return the start iterador
    iterator begin() { return iterator(0, this->_start); }
    //return the end iterator
    iterator end() { return iterator(this->_size, this->_end->_next); }
    //return the end like start iterator
    iterator rbegin() { return iterator(this->_size - 1, this->_end); }
    //return the start like end iterator  
    iterator rend() { return iterator(-1, this->_start->_back); }

    //constructor
    CDoubleList() : _start(nullptr), _end(nullptr), _size(0) {}
    //destructor
    ~CDoubleList() { delete this->_start; delete this->_end; }
    //geter del inicio(_start)
    node* start() const { return this->_start; }
    //geter del final(_end)
    node* final() const { return this->_end; }
    //geter del tamaño(_size)
    ul size() const { return this->_size; }
    //seter del inicio(_start)
    void start(node* _start) { this->_start = _start; }
    //seter del final(_end)
    void final(node* _end) { this->_end = _end; }
    //seter del tamaño(_size)
    void size(ul _size) { this->_size = _size; }

    /*//regresa el nodo en posicion
    node* at(ul index) {
        if (this->_size == 0) return nullptr;
        else if (index < 0 || index >= this->_size) return nullptr;
        if (index == 0) return this->_start;
        else if (index == this->_size - 1) return this->_end;

        auto aux = this->_start->_next;
        while (--index)
            aux = aux->_next;

        return aux;
    }*/

    //inserta al final
    void push_back(T element) {
        node* aux = new node(element);
        if (this->_size == 0)
            this->_start = this->_end = aux;
        else {
            this->_end->_next = aux;
            aux->_back = this->_end;
            this->_end = aux;
        }
        ++this->_size;

    }

    void insert(ul position, T element) {
        if (this->_size == 0 && position != 0)return;
        else if (position < 0 || position >= this->_size)return;
        node* nnode = new node(element);
        if (position == 0 && this->_size == 0) { this->_start = this->_end = nnode, ++this->_size; return; }
        else if (position == 0) {
            nnode->_next = this->_start;
            this->_start->_back = nnode;
            ++this->_size;
            return;
        }
        else if (position == this->_size - 1) { this->push_back(element); return; }
        else {
            node* aux = this->_start;
            while (--position)
                aux = aux->_next;

            node* temp = aux->_next;
            aux->_next = nnode;
            aux->_next->_back = aux;
            aux->_next->_next = temp;
            temp->_back = aux->_next;
            ++this->_size;
            return;
        }
    }

    //borra al final
    void pop_back() {
        if (this->_size == 0) return;
        if (this->_size == 1) { this->pop_front(); return; }
        node* aux = this->_end->_back;
        aux->_next = nullptr;
        this->_end->_back = nullptr;
        delete this->_end;
        this->_end = aux;
        --this->_size;
    }

    //borra al inicio
    void pop_front() {
        if (this->_size == 0) return;
        if (this->_size == 1) {
            delete this->_end, this->_start;
            this->_end = this->_start = nullptr;
        }
        else {
            node* aux = this->_start->_next;
            aux->_back = nullptr;
            this->_start->_next = nullptr;
            delete this->_start;
            this->_start = aux;
        }

        --this->_size;
    }
    //borra en posición
    void erase(ul position) {
        if (this->_size == 0) return;
        else if (position < 0 || position >= this->_size) return;

        if (position == 0) { this->pop_front(); return; }
        else if (position == this->_size - 1) { this->pop_back(); return; }

        node* aux = this->_start;
        while (--position)
            aux = aux->_next;

        node* aux2 = aux->_next;
        aux->_next = aux2->_next;
        aux2->_next->_back = aux;
        aux2->_next = nullptr;
        delete aux2;

        --this->_size;
    }

    void erase(node*& nnode) {
        node* aux;
        if (nnode == this->_start) {
            aux = this->_start;
            this->_start = nnode->_next;
            this->_start->_back = nullptr;
            nnode = this->_start;
        }
        else if (nnode == this->_end) {
            aux = this->_end;
            this->_end = nnode->_back;
            this->_end->_next = nullptr;
            nnode = this->_end;
        }
        else {
            aux = nnode;
            nnode->_back->_next = nnode->_next;
            nnode->_next->_back = nnode->_back;
            nnode = nnode->_back;
        }
        delete aux;
        --this->_size;
    }
};

#endif //__DOUBLE_LIST_HPP
