#include <iostrem>
#include "trees.hpp"

//.cpp para las pruebas del encabezado trees.hpp
int main() {
	const int size = 10;
	srand(time(nullptr));
	//los mayores van a la izquierda --> [](int*& i, int*& j) {return *i < *j; }
	BinaryTree<int*> tree([](int*& i, int*& j) {return *i < *j; });
	for (size_t i = 0; i < size; ++i)
		tree.insert(new int(10 - i));

	tree.post_order([](int*& i) {cout << *i << " "; });
	
	cout << endl;
	
	//devolvera el mayor valor, puesto que min devuelve el valor mas a la izquierda.
	//Debido a que el criterio de comparacion envió a los mayores a la izquierda
	cout << "Min: " << *tree.max() << endl;

	//devolvera el menor valor, puesto que max devuelve el valor mas a la derecha.
	//Debido a que el criterio de comparacion envió a los menores a la derecha
	cout << "Max: " << *tree.min() << endl;
	
	cout << tree.find(new int(10)) << endl;

	tree.in_order([](int*& i) {cout << *i << " "; });


	cout << endl;
	system("pause");
	return 0;
}
