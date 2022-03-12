#ifndef __BINARY_TREES_HPP__
#define __BINARY_TREES_HPP__
//types defs
using ll = long long;
using ull = unsigned long long;

//nodes
template<class Ty>
struct basic_bynode
{
private:
	using _bynode = basic_bynode;
	using ull = unsigned long long;
public:
	Ty value;
	ull level;
	ull height;
	_bynode* right;
	_bynode* left;
	basic_bynode(const Ty& value, ull level, ull height = 0) 
		: value(value), height(height), level(level), left(nullptr), right(nullptr) {}
	basic_bynode(const Ty& value, const _bynode*& right, const _bynode*&left, ull level, ull height = 0) :
		value(value), right(right), left(left), height(height), level(level) {}

	~basic_bynode() { delete this->left, this->right; }
	//_bynode operator const() { return *this; }
};

//class template for BynarieTree class
//compare type by default
template<typename Ty>
class ByFunction {

private:

    //reference const bool funtion
    bool(*_refc_function)(const Ty&, const Ty&);
    
    //normal bool function
    bool(*_function)(Ty&, Ty&);
    //normal const bool function

    short use;

public:
    ByFunction(): _function(nullptr), _refc_function(nullptr), use(3) {}
    ByFunction(bool(*_function)(Ty&, Ty&)) : _function(_function), _refc_function(nullptr), use(0) {}

    ByFunction(bool(*_refc_function)(const Ty&, const Ty&)) : _refc_function(_refc_function), _function(nullptr), use(1) {}

    bool operator()(const Ty& i, const Ty& j) const { 

        switch (use)
        {
        case 0: return _function(const_cast<Ty&>(i), const_cast<Ty&>(j));
        case 1: return _refc_function(i, j);
        default: return false;
        }
    }
};

template<class Ty, typename EvalFunction>
class BasicBinaryTree;

template<class Ty, typename EvalFunction>
class BasicBinaryTree {

protected:
	using _Node = basic_bynode<Ty>;
	using _Fn = EvalFunction;
	_Node* root;
	ull _size;
	_Fn _left;
	_Fn _equals;

	ll _height, a_height;

	template<typename BoolFunction>
	void _insert(_Node*& node, Ty& value, BoolFunction& _function, ull level = 0) {
		if (node != nullptr) {
			if (_function(node->value, value))
				this->_insert(node->left, value, _function, level + 1);
			else this->_insert(node->right, value, _function, level + 1);
		}
		else node = new _Node(value, level), ++this->_size;

		++this->a_height;
	}

	template<typename Function>
	Function _in_order(_Node*& node, Function& _function){
		if (node == nullptr) return _function;
		this->_in_order(node->left, _function);
		_function(node->value);
		this->_in_order(node->right, _function);
		return _function;
	}

	template<typename Function>
	Function _rin_order(_Node*& node, Function _function) {
		if (node == nullptr)
			return _function;
		this->_rin_order(node->right, _function);
		_function(node->value);
		this->_rin_order(node->left, _function);

		return _function;
	}

	template<typename Function>
	Function _pre_order(_Node*& node, Function& _function) {
		if (node == nullptr) return _function;
		_function(node->value);
		this->_pre_order(node->left, _function);
		this->_pre_order(node->right, _function);
		return _function;
	}

	template<typename Function>
	Function _post_order(_Node*& node, Function& _function) {
		if (node == nullptr)return _function;
		this->_post_order(node->left, _function);
		this->_post_order(node->right, _function);
		_function(node->value);
		return _function;
	}

	template<typename Function>
	Function _lefts(_Node*&node, Function& _function){
		if (node != nullptr) {
			_function(node->value);
			this->_lefts(node->left, _function);
		}
		return _function;
	}

	template<typename Function>
	Function _rights(_Node*& node, Function& _function) {
		if (node != nullptr) {
			_function(node->value);
			this->_rights(node->right, _function);
		}
		return _function;
	}

	template<typename Function>
	Function _s_level(_Node*& node, Function& _function, const ull& level, ull _level = 0) {
		if (node != nullptr) {
			node->level = _level;
			if (node->level == level)
				_function(node->value);
			else if (node->level < level) {
				this->_s_level(node->left, _function, level, _level + 1);
				this->_s_level(node->right, _function, level, _level + 1);
			}
		}
		return _function;
	}

	template<typename BoolFunction>
	_Node*& _find(_Node*& node, Ty& value, BoolFunction& _equals, BoolFunction& _left) {
		if (node == nullptr)
			return node;
		else if (_equals(node->value, value))
			return node;
		else if (_left(node->value, value))
			return this->_find(node->left, value, _equals, _left);
		else return this->_find(node->right, value, _equals, _left);
	}

	Ty& _min(_Node*& node) {
		if (node->left == nullptr)
			return node->value;

		return this->_min(node->left);
	}

	Ty& _max(_Node*& node) {
		if (node->right == nullptr)
			return node->value;
		return this->_max(node->right);
	}

	bool _e_side(_Node*& node) {
		if (node->left == nullptr && node->right == nullptr) {
			node = nullptr;
			delete node;
		}
		else if (node->left == nullptr) { node = node->right; }
		else if (node->right == nullptr) { node = node->left; }
		else {
			_Node*& aux = node->right;
			while (aux->left != nullptr)
				aux = aux->left;
			
			return true;
		}
		return false;
	}

	template<typename BoolFunction>
	bool _erase(_Node*&node, Ty&value, BoolFunction& _equals, BoolFunction& _left){
		if (node == nullptr)return false;
		
		if (_equals(node->value, value)) {
			if (this->_e_side(node))
				return this->_erase(node->right, node->value);
			else return true;
		}
		else {
			if (_left(node->value, value))
				return this->_erase(node->left, value);
			else return this->_erase(node->right, value);
		}
	}

	void _e_rights(_Node*& node) {
		if (node != nullptr) {
			this->_e_rights(node->right);
			if (node != nullptr && node != this->root) {
				if (this->_e_side(node))
					this->_e_rights(node->right);
			}
		}
	}

	void _e_lefts(_Node*&node){
		if (node != nullptr) {
			this->_e_lefts(node->left);
			if (node != nullptr && node != this->root) {
				if (this->_e_side(node))
					this->_e_lefts(node->left);
			}
		}
	}

	void _c_height(){
		if (this->_height < this->a_height)this->_height = this->a_height;
		this->a_height = 0;
	}

public:

	BasicBinaryTree(const _Fn& _left, const _Fn& _equals) :
		root(nullptr), _size(0), _height(0), a_height(0), 
		_left(_left), _equals(_equals)
	{}

	using value_type = Ty;
	using pointer = Ty*;
	using reference = Ty&;

	
	template<typename Function>
	Function in_order(Function _function) { return this->_in_order(this->root, _function); }

	template<typename Function>
	Function rin_order(Function _function) { return this->_rin_order(this->root, _function); }

	template<typename Function>
	Function pre_order(Function _function) { return this->_pre_order(this->root, _function); }

	template<typename Function>
	Function post_order(Function _function) { return this->_post_order(this->root, _function); }

	template<typename Function>
	Function lefts(Function _function) { if (this->root != nullptr) return this->_lefts(this->root->left, _function); return _function; }

	template<typename Function>
	Function rights(Function _function) { if (this->root != nullptr) return this->_rights(this->root, _function); return _function; }

	template<typename Function>
	Function r_lefts(Function _function) { return this->_lefts(this->root, _function); }

	template<typename Function>
	Function r_rights(Function _function) { return this->_rights(this->root, _function); }

	bool erase(const Ty& value) { 
		if (this->_erase(this->root, const_cast<Ty&>(value), this->_equals, this->_left)) {
			--this->_size;
			return true;
		}

		return false;
	}
	
	void e_rights() { this->_e_rights(this->root); }

	void e_lefts() { this->_e_lefts(this->root); }

	bool find(const Ty& value) { return this->_find(this->root, const_cast<Ty&>(value), this->_equals, this->_left) != nullptr; }

	Ty& v_find(const Ty& value){ 
		_Node* aux = this->_find(this->root, const_cast<Ty&>(value), this->_equals, this->_left);

		return aux != nullptr ? aux->value : const_cast<Ty&>(value);
	}

	Ty& min() { return this->_min(this->root); }

	Ty& max() { return this->_max(this->root); }

	ull size() const { return this->_size; }

	ll height() const { return this->_height; }

	bool empty() const { return this->_size == 0; }

	template<typename Function>
	Function for_level(Function _function, const ull& level) { return this->_s_level(this->root, _function, level); }

	void clear() { 
		delete this->root;
		this->root = nullptr;
		this->_size = this->_height = this->a_height = 0;
	}

};

template<class Ty, typename EvalFunction = ByFunction<Ty>>
class BinaryTree : public BasicBinaryTree<Ty, EvalFunction> {
	using _Fn = EvalFunction;
public:
	BinaryTree(const _Fn& _left, const _Fn& _equals) :
		BasicBinaryTree<Ty, EvalFunction>(_left, _equals) {}
	~BinaryTree() { delete this->root; this->root = nullptr; }

	void insert(const Ty& value) { this->_insert(this->root, const_cast<Ty&>(value), this->_left); this->_c_height(); }

	template<typename BoolFunction>
	void insert(const Ty& value, BoolFunction _function) { this->_insert(this->root, const_cast<Ty&>(value), _function); this->_c_height(); }

};

template<class Ty, typename EvalFunction = ByFunction<Ty>>
class BasicAvlTree : public BasicBinaryTree<Ty, EvalFunction> {

protected:
	using _Fn = EvalFunction;
	using _Node = basic_bynode<Ty>;
	
	void _right_rotation(_Node*& node){
		_Node* father = node->left;
		node->left = father->right;
		father->right = node;
		node = father;
	}

	void _left_rotation(_Node*& node) {
		_Node* father = node->right;
		node->right = father->left;
		father->left = node;
		node = father;
	}

	ll _n_height(_Node*& node) {
		if (node == nullptr)
			return 0;
		return node->height;
	}

	void swing(_Node*& node) {
		ll diference = this->_n_height(node->left) - this->_n_height(node->right);
		if (diference > 1) {
			--this->a_height;
			ll h_left = this->_n_height(node->left->left);
			ll h_right = this->_n_height(node->left->right);
			if (h_left < h_right)
				this->_left_rotation(node->left);
			this->_right_rotation(node);
		}
		else if (diference < -1) {
			--this->a_height;
			ll h_left = this->_n_height(node->right->left);
			ll h_right = this->_n_height(node->right->right);
			if (h_left > h_right)
				this->_right_rotation(node->right);
			this->_left_rotation(node);
		}
	}
	
	template<typename BoolFunction>
	void _avl_insert(_Node*& node, Ty& value, BoolFunction& _function, ull level = 0,  ull height = 0) {
		if (node != nullptr) {
			node->height = height;
			if (_function(node->value, value))
				this->_avl_insert(node->left, value, _function, level + 1, height - 1);
			else this->_avl_insert(node->right, value, _function, level + 1, height - 1);
			this->swing(node);
		}
		else {
			node = new _Node(value, level, height);
			++this->_size;
		}
		++this->a_height;
	}

public:
	BasicAvlTree(const _Fn& _left, const _Fn& _equals) : BasicBinaryTree<Ty, EvalFunction>(_left, _equals) {}
	~BasicAvlTree() { delete this->root; this->root = nullptr; }

	void insert(const Ty& value) {
		this->_avl_insert(this->root, const_cast<Ty&>(value), this->_left, 0, this->_height + 1);
		this->_c_height();
	}

	template<typename BoolFunction>
	void insert(const Ty& value, BoolFunction _function) {
		this->_avl_insert(this->root, const_cast<Ty&>(value), _function, 0, this->_height + 1);
		this->_c_height();
	}

};

#endif //__BASIC_BINARY_TREE_HPP__
