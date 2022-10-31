#include <BalancedTree/BalancedTree.h>
using namespace structures;
int main(int argc, char* argv[]) {
	srand(time(0));
	BalancedTree<int, int> tree;
	for (int i = 0; i < 100; i++) {
		tree.insert(rand(), rand() * 10);
	}
	tree.print();
}