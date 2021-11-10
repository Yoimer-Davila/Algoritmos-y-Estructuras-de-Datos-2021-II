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
    static bool less_equal(T*& i, T*& j) { return *i <= *j; }
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

    void c_height() {
        if (this->_height < this->a_height)this->_height = this->a_height;
        this->a_height = 0;
    }

    template<typename BoolFunction>
    void _insert(Node*& node, T& value, BoolFunction _function) {
        if (node != nullptr) {
            if (_function(node->value, value))
                this->_insert(node->left, value, _function);
            else this->_insert(node->rigth, value, _function);
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


    //_function --> for erase value in the tree
    //_s_function --> for left way, else rigth way
    template<typename BoolFunction>
    bool _erase(Node*& node, T& value, BoolFunction _function, BoolFunction _s_function) {
        if (node == nullptr) return false;
        
        if (_function(node->value, value)) {
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

                return this->_erase(node->rigth, aux->value, _function, _s_function);
            }
        }
        else {
            if (_s_function(node->value, value))
                return this->_erase(node->left, value, _function, _s_function);
            else return this->_erase(node->rigth, value, _function, _s_function);
        }
    }

    template<typename BoolFunction>
    bool _find(Node*& node, T& value, BoolFunction _function, BoolFunction _s_function) {
        if (node == nullptr)
            return false;
        else if (_function(node->value, value))
            return true;
        else if (_s_function(node->value, value))
            return this->_find(node->left, value, _function, _s_function);
        else return this->_find(node->rigth, value, _function, _s_function);
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
    BinaryTree() : root(nullptr), _size(0), _height(0), a_height(0) {}
    ~BinaryTree() { delete this->root; }

    
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    
    //insert left if (T _value > value) == true, right if not
    //BoolFunction is a binary function(return true or false) whose parameters is a two T types
    //preferably passed by reference
    //like this --> bool function(const T& a, const T&b) if T not is a pointer
    //T is the type that the BinaryTree contain
    template<typename BoolFunction>
    void insert(T value, BoolFunction  _function) { 
        this->_insert(this->root, value, _function); 
        this->c_height();
    }

    //insert left if (Node->value > value), right if not
    void insert(T value) { 
        this->_insert(this->root, value, Compare<T>::higher); 
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
    bool erase(T value) { return this->_erase(this->root, value, Compare<T>::equals, Compare<T>::higher); }

    //erase the value in the BinaryTree if exist
    //BoolFunction es una funcion booleana que recibe dos elementos de tipo T
    //_f_equals is the criteria that will be used to remove the value if it returns true
    //_function is the criterion to go left if it returns true
    template<typename BoolFunction>
    bool erase(T value, BoolFunction _f_equals, BoolFunction _function) { this->_erase(this->root, value, _f_equals, _function); }

    bool find(T value) { return this->_find(this->root, value, Compare<T>::equals, Compare<T>::higher); }

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
