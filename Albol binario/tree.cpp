#include <iostrem>
#include "trees.hpp"

//.cpp para las pruebas del encabezado trees.hpp

int main() {
	const int size = 5;
	srand(time(nullptr));
	BinaryTree<int*> tree;

	int* aux = nullptr;
	for (size_t i = 0; i < size; ++i) {
		aux = new int(rand() % 1000);
		tree.insert(aux);
	}
	cout << endl;
	cout << "Height: " << tree.height() << endl;
	cout << "Size: " << tree.size() << endl;

	
	cout << endl;
	system("pause");
	return 0;
}
