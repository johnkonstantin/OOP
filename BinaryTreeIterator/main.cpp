#include <iostream>
#include "BinaryTree.h"

int main() {
	BinaryTree tree;
	BinaryTree::Iterator iter;

	bool flag = true;
	while (flag) {
		char c = getchar();
		while (c == '\n' or c == '\r') {
			c = getchar();
		}
		switch (c) {
			case 'a':
				int value;
				scanf("%d", &value);
				tree.add(value);
				break;
			case 'n':
				if (tree.size() != 0) {
					if (iter == BinaryTree::Iterator()) {
						iter = tree.begin();
					}
					++iter;
				}
				break;
			case 'p':
				if (tree.size() != 0) {
					if (iter == BinaryTree::Iterator()) {
						iter = tree.end();
					}
					--iter;
				}
				break;
			case 'b':
				iter = tree.begin();
				break;
			case 'e':
				iter = tree.end();
				break;
			case 'v':
				if (iter != BinaryTree::Iterator()) {
					(*iter)->printValue();
				}
				break;
			case 'x':
				flag = false;
				break;
			default:
				std::cout << "Unknown command!" << std::endl;
		}
	}
	return 0;
}
