#ifndef ITERATOR_BINARYTREE_H
#define ITERATOR_BINARYTREE_H

#include <iostream>
#include <stdexcept>

class BinaryTree {
private:
	struct TreeNode {
		TreeNode* _left;
		TreeNode* _right;
		TreeNode* _parent;
		int _value;

		TreeNode(TreeNode* parent, int value) : _left(nullptr), _right(nullptr), _parent(parent), _value(value) {};
		~TreeNode();

		int getValue() const;
	};

	TreeNode* _root;
	TreeNode* _endNode;

	size_t _nodeCount;

public:
	BinaryTree() : _root(nullptr), _endNode(new TreeNode(nullptr, 0)), _nodeCount(0) {};
	~BinaryTree();

	void add(int value);
	size_t size();

	class Iterator {
	private:
		BinaryTree* _tree;
		TreeNode* _currentNode;

	public:
		enum TreePosition {
			BEGIN, END
		};

		Iterator() : _tree(nullptr), _currentNode(nullptr) {};
		Iterator(const Iterator &) = default;
		Iterator(BinaryTree*, TreePosition = BEGIN);
		~Iterator() = default;

		TreeNode* operator*();
		Iterator operator++();
		Iterator operator++(int);
		Iterator operator--();
		Iterator operator--(int);
		bool operator==(const Iterator &);
		bool operator!=(const Iterator &);

	};

	Iterator begin();
	Iterator end();
};


#endif
