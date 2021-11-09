#ifndef __SIMPLE_LIST_HPP__
#define __SIMPLE_LIST_HPP__
typedef unsigned long ul;
typedef long long ll;
//clase template de una lista simple enlazada
template <class T>
class CList
{
private:
    struct node
    {
        node* _next;
        T _element;

        node(T element, node* _next = nullptr) : _element(element), _next(_next) {}
        ~node() {}
    };
    ul _size;
    node* _start;
    node* _end;

    void push(node* nnode) { this->_start = this->_end = nnode; }
    void pop() { delete this->_end, this->_start; }

public:
    //class iterator
    class iterator
    {
    private:
        ll _index;
        node* aux;
    public:
        iterator(ll _index, node*& aux) : _index(_index), aux(aux) {}
        iterator& operator++ () { _index++; aux = aux->_next; return *this; }
        bool operator!= (iterator _iterator) { return this->_index != _iterator._index; }
        T& operator* () { return aux->_element; }
        T* operator->() { return &aux->_element; }
    };
    //return the start iterador
    iterator begin() { return iterator(0, this->_start); }
    //return the end iterator
    iterator end() { return iterator(this->_size, this->_end->_next); }

    //constructor
    CList() : _start(nullptr), _size(0), _end(nullptr) {}
    //destructor
    ~CList() { delete this->_start, this->_end; }
    //geter del final(_end)
    //geter del tamaño(_size)
    ul size() const { return this->_size; }

    //poner al final de la lista
    void push_back(T element) {
        node* nnode = new node(element);
        if (this->_size == 0)
            push(nnode);
        else {
            this->_end->_next = nnode;
            this->_end = this->_end->_next;
        }
        ++this->_size;
    }

    //poner al principio de la lista
    void push_front(T element) {
        node* nnode = new node(element);
        if (this->_size == 0)
            push(nnode);
        else {
            nnode->_next = this->_start;
            this->_start = nnode;
        }
        ++this->_size;
    }

    //poner en posicion
    void insert(T element, ul position) {
        if (this->_size == 0 && position != 0)return;
        else if (position < 0 || position >= this->_size)return;
        if (position == 0) {
            this->push_front(element);
            return;
        }
        else if (position == this->_size - 1) {
            this->push_back(element);
            return;
        }
        node* nnode = new node(element);
        node* aux = this->_start;
        while (--position)
            aux = aux->_next;

        nnode->_next = aux->_next;
        aux->_next = nnode;
        ++this->_size;

    }

    //borrar el ultimo
    void pop_back() {
        if (this->_size == 1)
            this->pop();
        else {
            auto aux = this->_start;
            while (aux->_next->_next != nullptr)
                aux = aux->_next;

            delete this->_end;
            this->_end = aux;
        }
        --this->_size;
    }

    //borrar el primero de la lista
    void pop_front() {
        if (this->_size == 1)
            this->pop();
        else {
            node* aux = this->_start->_next;
            this->_start->_next = nullptr;
            delete this->_start;
            this->_start = aux;
        }
        --this->_size;
    }

    //borrar en posicion 
    void erase(ul position) {
        if (this->_size == 0)return;
        else if (position < 0 || position >= this->_size)return;
        if (position == 0) {
            this->pop_front();
            return;
        }
        else if (position == this->_size - 1) {
            this->pop_back();
            return;
        }
        node* aux = this->_start;
        node* temp = aux;
        while (--position) {
            aux = aux->_next;
            temp = aux;
        }
        aux = aux->_next;
        temp->_next = aux->_next;
        aux->_next = nullptr;
        delete aux->_next;

        --this->_size;
    }
};

#endif //__SIMPLE_LIST_HPP__
