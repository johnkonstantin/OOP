#include <gtest/gtest.h>
#include <cstring>
#include "../BinaryTree.h"

int compare(const void *x1, const void *x2)
{
	if (*(int*)x1 < *(int*)x2) {
		return -1;
	}
	if (*(int*)x1 == *(int*)x2) {
		return 0;
	}
	if (*(int*)x1 > *(int*)x2) {
		return 1;
	}
	return 0;
}

TEST(TESTS, simple_test) {
	BinaryTree tree;
	BinaryTree::Iterator iter;

	const int length = 256;
	int test_input[length];
	for (int i = 0; i < length; ++i) {
		test_input[i] = rand() % 1000;
	}
	for (int i = 0; i < length; ++i) {
		tree.add(test_input[i]);
	}
	int test_output[length];
	memcpy(test_output, test_input, length * sizeof(int));
	qsort(test_output, length, sizeof(int), compare);
	iter = tree.begin();
	for (int i = 0; i < length; ++i) {
		ASSERT_EQ(test_output[i], (*iter)->getValue());
		++iter;
	}
	for (int i = 0; i < length; ++i) {
		++iter;
	}
	ASSERT_TRUE(iter == tree.end());
	--iter;
	for (int i = 0; i < length; ++i) {
		ASSERT_EQ(test_output[length - i - 1], (*iter)->getValue());
		--iter;
	}
	ASSERT_EQ(tree.size(), length);
}

TEST(TESTS, foreach_test) {
	BinaryTree tree;
	BinaryTree::Iterator iter;

	const int length = 256;
	int test_input[length];
	for (int i = 0; i < length; ++i) {
		test_input[i] = rand() % 1000;
	}
	for (int i = 0; i < length; ++i) {
		tree.add(test_input[i]);
	}
	int test_output[length];
	memcpy(test_output, test_input, length * sizeof(int));
	qsort(test_output, length, sizeof(int), compare);

	int* j = test_output;
	for (auto i : tree) {
		ASSERT_EQ(*j, i->getValue());
		++j;
	}
}

TEST(TESTS, empty_tree_test) {
	BinaryTree tree;
	BinaryTree::Iterator iter;
	iter = tree.begin();
	ASSERT_TRUE(iter == BinaryTree::Iterator());
	iter = tree.end();
	ASSERT_TRUE(iter == BinaryTree::Iterator());
	ASSERT_ANY_THROW(iter.operator*());
}

TEST(TESTS, pos) {
	BinaryTree tree;
	tree.add(5);
	ASSERT_ANY_THROW(BinaryTree::Iterator(&tree, (BinaryTree::Iterator::TreePosition)55));
}

TEST(TESTS, inc_dec) {
	BinaryTree tree;
	BinaryTree::Iterator iter = tree.begin();
	ASSERT_TRUE((++iter) == iter);
	ASSERT_TRUE((--iter) == iter);
	ASSERT_TRUE((iter++) == iter);
	ASSERT_TRUE((iter--) == iter);
}
