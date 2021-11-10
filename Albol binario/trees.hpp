#ifndef __BINARY_TREE_HPP__
#define __BINARY_TREE_HPP__
using ull = unsigned long long;// --> typedef ull unsigned long long;

//contains functions(return a bool value) for basic comparisons:>, <,> =, <= and ==. between two elements of T type
template<class T>
struct Compare {
    //true if T is a pointer, false if not
    static const bool is_pointer = false;
    //return first parameter >= second parameter
    static bool higher_equal(const T& i, const T& j) { return i >= j; }
    //return first parameter <= second parameter
    static bool less_equal(const T& i, const T& j) { return i <= j; }
    //return first parameter > second parameter
    static bool higher(const T& i, const T& j) { return i > j; };
    //return first parameter < second parameter
    static bool less(const T& i, const T& j) { return i < j; };
    //return first parameter == second parameter
    static bool equals(const T& i, const T& j) { return i == j; }
};

//Partial specialization of Compare for T-type pointer
template<class T>
struct Compare<T*> {
    //true if T is a pointer, false if not
    static const bool is_pointer = true;
    //return first parameter >= second parameter
    static bool higher_equal(T*& i,T*& j) { return *i >= *j; }
    //return first parameter <= second parameter
    static bool less_equal( T*& i, T*& j) { return *i <= *j; }
    //return first parameter > second parameter
    static bool higher(T*& i, T*& j) { return *i > *j; };
    //return first parameter < second parameter
    static bool less(T*& i, T*& j) { return *i < *j; };
    //return first parameter == second parameter
    static bool equals(T*& i, T*& j) { return *i == *j; }
};

template<class T>
class BinaryTree
{
private:
    struct Node
    {
        T value;
        Node* rigth;
        Node* left;
        Node(T value) : value(value), left(nullptr), rigth(nullptr) {}
        ~Node() { delete this->left, this->rigth; }
    };

    Node* root;
    ull _size;
    long _height, a_height;

    //function for send left of tree the less elements
    bool(*_left)(T& i, T& j);

    //function for erases and finds
    bool(*_equals)(T& i, T& j);

    void c_height() {
        if (this->_height < this->a_height)this->_height = this->a_height;
        this->a_height = 0;
    }

    void _insert(Node*& node, T& value) {
        if (node != nullptr) {
            if (this->_left(node->value, value))
                this->_insert(node->left, value);
            else this->_insert(node->rigth, value);
        }
        else {
            node = new Node(value);
            ++this->_size;
        }
        ++this->a_height;
    }

    template<typename Function>
    void _in_order(Node*& node, Function _function) {
        if (node == nullptr)
            return;
        this->_in_order(node->left, _function);
        _function(node->value);
        this->_in_order(node->rigth, _function);
    }

    template<typename Function>
    void _pre_order(Node*& node, Function _function) {
        if (node == nullptr)return;

        _function(node->value);
        this->_pre_order(node->left, _function);
        this->_pre_order(node->rigth, _function);
    }

    template<typename Function>
    void _post_order(Node*& node, Function _function) {
        if (node == nullptr)return;
        this->_post_order(node->left, _function);
        this->_post_order(node->rigth, _function);
        _function(node->value);
    }

    bool _erase(Node*& node, T& value) {
        if (node == nullptr) return false;
        
        if (this->_equals(node->value, value)) {
            if (node->left == nullptr && node->rigth == nullptr) {
                node = nullptr;
                delete node;
                return true;
            }
            else if (node->left == nullptr) {
                node = node->rigth;
                return true;
            }
            else if (node->rigth == nullptr) {
                node = node->left;
                return true;
            }
            else {
                auto aux = node->rigth;
                while (aux->left != nullptr)
                    aux = aux->left;

                node->value = aux->value;

                return this->_erase(node->rigth, aux->value);
            }
        }
        else {
            if (this->_left(node->value, value))
                return this->_erase(node->left, value);
            else return this->_erase(node->rigth, value);
        }
    }

    bool _find(Node*& node, T& value) {
        if (node == nullptr)
            return false;
        else if (this->_equals(node->value, value))
            return true;
        else if (this->_left(node->value, value))
            return this->_find(node->left, value);
        else return this->_find(node->rigth, value);
    }

    T& _min(Node*& node) {
        if (node->left == nullptr)
            return node->value;
        
        return _min(node->left);
    }

    T& _max(Node*& node) {
        if (node->rigth == nullptr)
            return node->value;

        return _max(node->rigth);
    }

public:
    //Parametros:
    //_left --> criterio de comparacion para insercion de elementos para tipos primitivos  
    //_left --> por defecto es bool(T&i,T&j){return i > j}
    //si left retorna true insertará a la izquierda el elemento, a la derecha si retorna false
    //_equals --> criterio de comparacion para busqueda y eliminacion de elementos con tipos primitivos
    //_equals --> por defecto es bool(T&i, T&j){return i == j}
    //si _equals retorna true será el elemento que se busca o se quiere eliminar
    BinaryTree(bool(*_left)(T&,T&) = Compare<T>::higher, bool(*_equals)(T&, T&) = Compare<T>::equals)
        : 
        root(nullptr), _size(0), _height(0), a_height(0), _left(_left), _equals(_equals) {}
    ~BinaryTree() { delete this->root; }

    
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    
    //insert left if _function(T&, T&) == true, right if not
    void insert(T value) { 
        this->_insert(this->root, value); 
        this->c_height();
    }

    //Function is any function whose only parameter(preferably passed by reference) is the T type
    template<typename Function>
    void in_order(Function _function) { this->_in_order(this->root, _function); }

    //Function is any function whose only parameter(preferably passed by reference) is the T type
    template<typename Function>
    void pre_order(Function _function) { this->_pre_order(this->root, _function); }

    //Function is any function whose only parameter(preferably passed by reference)  is the T type
    template<typename Function>
    void post_order(Function _function) { this->_post_order(this->root, _function); }

    //erase the value in the BinaryTree if exist
    bool erase(T value) { return this->_erase(this->root, value); }

    //return true if value exist in the tree
    bool find(T value) { return this->_find(this->root, value); }

    //returns the minimum element --> returns the value most to the left of the tree
    //if your criteria inserted the minors to the right, use max() as min()
    T& min() { return this->_min(this->root); }

    //returns the maximum element -> returns the value most to the right of the tree
    //if your criteria inserted the highers on the left, use min() as max()
    T& max() { return this->_max(this->root); }

    //return the number of elements in the BinaryTree
    ull size() const { return this->_size; }

    //return the height of the BinaryTree
    long height() const { return this->_height; }

    //return true if the BinaryTree is empty, false if not
    bool empty() const { return this->_size == 0; }

};

#endif //__BINARY_TREE_HPP__
