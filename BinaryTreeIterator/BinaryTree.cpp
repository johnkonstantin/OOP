#include "BinaryTree.h"

void BinaryTree::add(int value) {
	if (_root == nullptr) {
		_root = new TreeNode(nullptr, value);
		_endNode->_parent = _root;
	}
	else {
		TreeNode* t = _root;
		while (true) {
			if (value < t->_value) {
				if (t->_left != nullptr) {
					t = t->_left;
				}
				else {
					t->_left = new TreeNode(t, value);
					break;
				}
			}
			else {
				if (t->_right != nullptr) {
					t = t->_right;
				}
				else {
					t->_right = new TreeNode(t, value);
					if (_endNode->_parent->_value <= value) {
						_endNode->_parent = t->_right;
					}
					break;
				}
			}
		}
	}
	++_nodeCount;
}

BinaryTree::Iterator BinaryTree::begin() {
	if (_root == nullptr) {
		return Iterator();
	}
	else {
		return Iterator(this, Iterator::BEGIN);
	}
}

BinaryTree::Iterator BinaryTree::end() {
	if (_root == nullptr) {
		return Iterator();
	}
	else {
		return Iterator(this, Iterator::END);
	}
}

BinaryTree::~BinaryTree() {
	delete _root;
	delete _endNode;
}

size_t BinaryTree::size() {
	return _nodeCount;
}

BinaryTree::Iterator::Iterator(BinaryTree* tree, BinaryTree::Iterator::TreePosition pos) {
	_tree = nullptr;
	_currentNode = nullptr;
	if (tree != nullptr) {
		_tree = tree;
		if (_tree->_root != nullptr) {
			TreeNode* t = _tree->_root;
			if (pos == BEGIN) {
				while (t->_left != nullptr) {
					t = t->_left;
				}
			}
			else if (pos == END) {
				t = _tree->_endNode;
			}
			else {
				throw std::runtime_error("Unknown position arg!");
			}
			_currentNode = t;
		}
	}
}

BinaryTree::TreeNode* BinaryTree::Iterator::operator*() {
	if (_tree == nullptr) {
		throw std::runtime_error("Empty tree!");
	}
	return _currentNode;
}

BinaryTree::Iterator BinaryTree::Iterator::operator++() {
	if (_tree == nullptr) {
		return *this;
	}
	if (_currentNode == _tree->end()._currentNode->_parent) {
		*this = _tree->end();
		return *this;
	}
	if (*this == _tree->end()) {
		return *this;
	}
	if (_currentNode->_right != nullptr) {
		_currentNode = _currentNode->_right;
		while (_currentNode->_left != nullptr) {
			_currentNode = _currentNode->_left;
		}
	}
	else {
		TreeNode* p;
		p = _currentNode->_parent;
		while (p != nullptr and _currentNode == p->_right) {
			_currentNode = p;
			p = _currentNode->_parent;
		}
		_currentNode = p;
	}
	return *this;

}

BinaryTree::Iterator BinaryTree::Iterator::operator++(int) {
	Iterator prev = *this;
	*this = this->operator++();
	return prev;
}

BinaryTree::Iterator BinaryTree::Iterator::operator--() {
	if (_tree == nullptr or *this == _tree->begin()) {
		return *this;
	}
	else {
		if (_currentNode->_left != nullptr) {
			_currentNode = _currentNode->_left;
			while (_currentNode->_right != nullptr) {
				_currentNode = _currentNode->_right;
			}
		}
		else {
			TreeNode* p;
			p = _currentNode->_parent;
			while (p != nullptr and _currentNode == p->_left) {
				_currentNode = p;
				p = _currentNode->_parent;
			}
			_currentNode = p;
		}
		return *this;
	}
}

BinaryTree::Iterator BinaryTree::Iterator::operator--(int) {
	Iterator prev = *this;
	*this = this->operator--();
	return prev;
}

bool BinaryTree::Iterator::operator==(const BinaryTree::Iterator &iter) {
	if (_tree == iter._tree and _currentNode == iter._currentNode) {
		return true;
	}
	return false;
}

bool BinaryTree::Iterator::operator!=(const BinaryTree::Iterator &iter) {
	return !operator==(iter);
}

BinaryTree::TreeNode::~TreeNode() {
	delete _left;
	delete _right;
}

int BinaryTree::TreeNode::getValue() const {
	return _value;
}
