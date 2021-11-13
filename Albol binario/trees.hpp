#ifndef __BINARY_TREE_HPP__
#define __BINARY_TREE_HPP__
using ull = unsigned long long;// --> typedef ull unsigned long long;
//contains functions(return a bool value) for basic comparisons:>, <,> =, <= and ==. between two elements of T type
template<class T>
struct Compare {
    //true if T is a pointer, false if not
    static const bool is_pointer = false;
    //return first parameter >= second parameter
    static bool higher_equal(T& i, T& j) { return i >= j; }
    //return first parameter <= second parameter
    static bool less_equal(T& i, T& j) { return i <= j; }
    //return first parameter > second parameter
    static bool higher(T& i, T& j) { return i > j; };
    //return first parameter < second parameter
    static bool less(T& i, T& j) { return i < j; };
    //return first parameter == second parameter
    static bool equals(T& i, T& j) { return i == j; }
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
        ull level;
        Node* rigth;
        Node* left;
        Node(T value, ull level = 0) : value(value), level(level), left(nullptr), rigth(nullptr) {}
        ~Node() { delete this->left, this->rigth; }
    };

    Node* root;
    ull _size;
    long _height, a_height;

    //function(comparation) for send left of tree the less elements
    bool(*_left)(T& i, T& j);

    //function(comparation) for erases and finds
    bool(*_equals)(T& i, T& j);

    void c_height() {
        if (this->_height < this->a_height)this->_height = this->a_height;
        this->a_height = 0;
    }

    template<class BoolFunction>
    void _insert(Node*& node, T& value, BoolFunction _function, ull level = 0) {
        if (node != nullptr) {
            if (_function(node->value, value))
                this->_insert(node->left, value, _function, level + 1);
            else this->_insert(node->rigth, value, _function, level + 1);
        }
        else {
            node = new Node(value, level);
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
    void _rin_order(Node*& node, Function _function) {
        if (node == nullptr)
            return;
        this->_rin_order(node->rigth, _function);
        _function(node->value);
        this->_rin_order(node->left, _function);
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

    template<typename Function>
    void _lefts(Node*& node, Function _function) {
        if(node != nullptr){
            _function(node->value);
            this->_lefts(node->left, _function);
        }
    }

    template<typename Function>
    void _rigths(Node*& node, Function _function) {
        if (node != nullptr) {
            _function(node->value);
            this->_rigths(node->rigth, _function);
        }
    }
    
    bool _erase(Node*& node, T& value) {
        if (node == nullptr) return false;
        
        if (this->_equals(node->value, value)) {
            if (this->_e_side(node))
                return this->_erase(node->rigth, node->value);
            else return true;
        }
        else {
            if (this->_left(node->value, value))
                return this->_erase(node->left, value);
            else return this->_erase(node->rigth, value);
        }
    }

    Node*& _find(Node*& node, T& value) {
        if (node == nullptr)
            return node;
        else if (this->_equals(node->value, value))
            return node;
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

    void copy(Node*& node) {
        if (node != nullptr) {
            this->_insert(this->root, node->value, this->_left);
            this->copy(node->left);
            this->copy(node->rigth);
        }
    }

    bool _e_side(Node*& node) {
        if (node->left == nullptr && node->rigth == nullptr) {
            node = nullptr;
            delete node;
        }
        else if (node->left == nullptr) { node = node->rigth; }
        else if (node->rigth == nullptr) { node = node->left; }
        else {
            auto aux = node->rigth;
            while (aux->left != nullptr)
                aux = aux->left;

            node->value = aux->value;
            return true;
        }
        return false;
    }

    void _e_rigths(Node*& node) {
        if (node != nullptr) {
            this->_e_rigths(node->rigth);
            if (node != nullptr && node != this->root) {
                if(_e_side(node))
                    this->_e_rigths(node->rigth);
            }
        }
    }

    void _e_lefts(Node*& node) {
        if (node != nullptr) {
            this->_e_lefts(node->left);
            if (node != nullptr && node != this->root) {
                if (_e_side(node))
                    this->_e_lefts(node->left);
            }
        }
    }

    template<class Function>
    void _s_level(Node*&node, Function _function, ull &level){
        if (node != nullptr) {
            if (node->level == level) 
                _function(node->value);
            else if (node->level < level) {
                _s_level(node->left, _function, level);
                _s_level(node->rigth, _function, level);
            }
        }
        
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

    BinaryTree(BinaryTree<T>& tree) : root(nullptr), _size(0), _height(0), a_height(0) {
        this->_equals = tree._equals;
        this->_left = tree._left;
        this->copy(tree.root);
    }
    ~BinaryTree() { delete this->root; }

    void operator=(BinaryTree<T>& tree) { 
        this->~BinaryTree();
        this->_size = this->a_height =  0;
        this->_equals = tree._equals;
        this->_left = tree._left;
        this->copy(tree.root);
        this->_height = tree._height;
    }

    using value_type = T;
    using pointer = T*;
    using reference = T&;
    
    //insert left if _function(T&, T&) == true, right if not
    void insert(T value) { 
        this->_insert(this->root, value, this->_left); 
        this->c_height();
    }

    //insert left if BoolFunction return true, right if not
    //BoolFunction --> function(T&a, T&b) 
    template<class BoolFunction>
    void insert(T value, BoolFunction _function) { 
        this->_insert(this->root, value, _function);
        this->c_height();
    }

    //Function is any function whose only parameter(preferably passed by reference) is the T type
    //aply function all elements in the tree in order 
    template<typename Function>
    void in_order(Function _function) { this->_in_order(this->root, _function); }

    //Function is any function whose only parameter(preferably passed by reference) is the T type
    //aply function all elements in the tree in reverse order
    template<typename Function>
    void rin_order(Function _function) { this->_rin_order(this->root, _function); }

    //Function is any function whose only parameter(preferably passed by reference) is the T type
    template<typename Function>
    void pre_order(Function _function) { this->_pre_order(this->root, _function); }

    //Function is any function whose only parameter(preferably passed by reference)  is the T type
    template<typename Function>
    void post_order(Function _function) { this->_post_order(this->root, _function); }
    
    //Aplica Function a todas los elementos de la izquierda incluida la raiz
    template<typename Function>
    void r_lefts(Function _function) { this->_lefts(this->root, _function); }
    
    //Aplica Function a todas los elementos de la izquierda, menos a la raiz
    template<typename Function>
    void lefts(Function _function) { 
        if (this->root != nullptr)
            this->_lefts(this->root->left, _function);
    }

    //Aplica Function a todos los elementos de la derecha, menos a la raiz
    template<typename Function>
    void rigths(Function _function) {
        if (this->root != nullptr)
            this->_rigths(this->root->rigth, _function);
    }

    //Aplica Function a todos los elementos de la derecha incluida la raiz
    template<typename Function>
    void r_rigths(Function _function) { this->_rigths(this->root, _function); }

    //erase the value in the BinaryTree if exist
    bool erase(T value) { return this->_erase(this->root, value); }

    //return true if value exist in the tree
    bool find(T value) { return this->_find(this->root, value) != nullptr ? true : false; }

    //si value existe en el arbol, retornara el valor que se encuentra en el arbol
    //si no, retornará el valor pasado por parámetro
    T& v_find(T value) {
        auto aux = this->_find(this->root, value);
        return aux != nullptr ? aux->value : value;
    }

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

    //erase all elements in the rights exept the root
    void e_rigths() { this->_e_rigths(this->root); }
    
    //erase all elements in the lefts exept the root
    void e_lefts() { this->_e_lefts(this->root); }

    //Aplica Function a cada elemento del nivel deseado
    template<typename Function>
    void for_level(Function _function, ull level) { this->_s_level(this->root, _function, level); }

    //clear the tree
    void clear() { 
        this->~BinaryTree();
        this->root = nullptr;
        this->_size = this->_height = this->a_height = 0; 
    }

    //return root value
    T& v_root() { return this->root->value; }
};

#endif //__BINARY_TREE_HPP__
