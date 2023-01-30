#pragma once
#include <memory>
#include <iostream>
#include <string>
namespace structures {
	template <class T, class Y>
	class BalancedTree {
	private:
		class BalancedNode {
		public:
			typedef typename std::shared_ptr<BalancedNode> SP_BalancedNode;
		private:
			T m_key;
			Y m_data;
			int m_leftHeight;
			int m_rightHeight;
			SP_BalancedNode m_leftChild;
			SP_BalancedNode m_rightChild;
			SP_BalancedNode m_parent;
		public:
			BalancedNode(T key, Y data);
			BalancedNode(T key, Y data, SP_BalancedNode leftChild, SP_BalancedNode rightChild);
			T key() const { return m_key; }
			Y data() const { return m_data; }
			int leftHeight() const { return m_leftHeight; }
			int rightHeight() const { return m_rightHeight; }
			SP_BalancedNode leftChild() const { return m_leftChild; }
			SP_BalancedNode rightChild() const { return m_rightChild; }
			SP_BalancedNode parent() const { return m_parent; }
			void incLeftHeight() { m_leftHeight++; }
			void incRightHeight() { m_rightHeight++; }
			void setParent(SP_BalancedNode par) { m_parent = par; }
			void setLeftChild(SP_BalancedNode child) { m_leftChild = child; }
			void setRightChild(SP_BalancedNode child) { m_rightChild = child; }
			int maxHeight() { return m_leftHeight > m_rightHeight ? m_leftHeight : m_rightHeight; }
			void updateHeight();
			SP_BalancedNode rotateLeft();
			SP_BalancedNode rotateRight();
			void dropRelations();
			void print(int spacer) const;
			~BalancedNode() {}
		};

	private:
		std::shared_ptr<BalancedNode> m_root;
	public:
		BalancedTree() {}
		bool insert(T key, Y data);
		Y search(T key) const;
		bool erase(T key) {}
		void rotateRight(std::shared_ptr<BalancedNode> node);
		void rotateLeft(std::shared_ptr<BalancedNode> node);
		void rebalance(std::shared_ptr<BalancedNode> node);
		bool empty() { return m_root; }
		void print() { m_root->print(0); }
		~BalancedTree();
	};

	template<class T, class Y>
	BalancedTree<T, Y>::BalancedNode::BalancedNode(T key, Y data) :m_key(key), m_data(data), m_leftHeight(0), m_rightHeight(0) {}

	template<class T, class Y>
	BalancedTree<T, Y>::BalancedNode::BalancedNode(T key, Y data, SP_BalancedNode leftChild, SP_BalancedNode rightChild) : m_key(key), m_data(data), m_leftChild(leftChild), m_rightChild(rightChild) {
		m_leftHeight += leftChild->maxHeight() + 1;
		m_rightHeight += rightChild->maxHeight() + 1;
	}

	template<class T, class Y>
	void BalancedTree<T, Y>::BalancedNode::updateHeight() {
		if (m_rightChild)
			m_rightHeight = m_rightChild->maxHeight() + 1;
		if (m_leftChild)
			m_leftHeight = m_leftChild->maxHeight() + 1;
		if (m_parent)
			m_parent->updateHeight();
	}

	template<class T, class Y>
	std::shared_ptr<typename BalancedTree<T, Y>::BalancedNode> BalancedTree<T, Y>::BalancedNode::rotateLeft() {
		BalancedNode::SP_BalancedNode parent = m_parent;
		BalancedNode::SP_BalancedNode substitude = m_rightChild;
		BalancedNode::SP_BalancedNode thisNode = m_rightChild->parent();


		substitude->setParent(parent);
		m_rightChild = substitude->leftChild();
		if (m_rightChild)
			m_rightChild->setParent(thisNode);
		substitude->setLeftChild(thisNode);
		m_parent = substitude;

		if (parent) {
			if (parent->leftChild() == thisNode)
				parent->setLeftChild(substitude);
			else
				parent->setRightChild(substitude);
		}

		m_rightHeight = substitude->leftHeight();
		substitude->m_leftHeight = maxHeight() + 1;
		substitude->updateHeight();
		return substitude;
	}

	template<class T, class Y>
	typename BalancedTree<T, Y>::BalancedNode::SP_BalancedNode BalancedTree<T, Y>::BalancedNode::rotateRight() {
		BalancedNode::SP_BalancedNode parent = m_parent;
		BalancedNode::SP_BalancedNode substitude = m_leftChild;
		BalancedNode::SP_BalancedNode thisNode = m_leftChild->parent();

		substitude->setParent(parent);
		m_leftChild = substitude->rightChild();
		if (m_leftChild)
			m_leftChild->setParent(thisNode);
		substitude->setRightChild(thisNode);
		m_parent = substitude;

		if (parent) {
			if (parent->leftChild() == thisNode)
				parent->setLeftChild(substitude);
			else
				parent->setRightChild(substitude);
		}

		m_leftHeight = substitude->rightHeight();
		substitude->m_rightHeight = maxHeight() + 1;
		substitude->updateHeight();
		return substitude;
	}

	template<class T, class Y>
	void BalancedTree<T, Y>::BalancedNode::dropRelations() {
		if (m_leftChild)
			m_leftChild->dropRelations();
		if (m_rightChild)
			m_rightChild->dropRelations();
		m_parent = m_rightChild = m_leftChild = nullptr;
	}

	template<class T, class Y>
	void BalancedTree<T, Y>::BalancedNode::print(int spacer) const {
		spacer++;
		if (m_rightChild)
			m_rightChild->print(spacer);
		std::string space(2 * (spacer - 1), ' ');
		std::cout << space;
		std::cout << m_key << std::endl;
		if (m_leftChild)
			m_leftChild->print(spacer);
	}

	//template<class T, class Y>
	//BalancedTree<T, Y>::BalancedNode::~BalancedNode() {
	//	std::cout << "Destruction of   " << m_key << std::endl;
	//}




	template<class T, class Y>
	bool BalancedTree<T, Y>::insert(T key, Y data) {
		std::shared_ptr<BalancedNode> newNode = std::make_shared<BalancedNode>(key, data);
		if (!m_root) {
			m_root = newNode;
			return true;
		}
		std::shared_ptr<BalancedNode> node = m_root;
		std::shared_ptr<BalancedNode> leaf = node;
		while (node) {
			leaf = node;
			if (node->key() == key)
				return false;
			if (node->key() > key)
				node = node->leftChild();
			else
				node = node->rightChild();
		}
		if (leaf->key() > key) {
			leaf->setLeftChild(newNode);
			newNode->setParent(leaf);
			leaf->incLeftHeight();
		}
		else {
			leaf->setRightChild(newNode);
			newNode->setParent(leaf);
			leaf->incRightHeight();
		}
		newNode->updateHeight();
		rebalance(newNode);
		return true;
	}

	template<class T, class Y>
	Y BalancedTree<T, Y>::search(T key) const {
		std::shared_ptr<BalancedNode> node = m_root;
		while (node && node->key() != key) {
			if (node->key() < key)
				node = node->leftChild();
			else
				node = node->rightChild();
		}
		if (node)
			return node->data();
		throw std::exception("Data not found");
	}

	template<class T, class Y>
	void BalancedTree<T, Y>::rotateRight(std::shared_ptr<BalancedNode> node) {
		auto subst = node->rotateRight();
		if (node == m_root)
			m_root = subst;
	}

	template<class T, class Y>
	void BalancedTree<T, Y>::rotateLeft(std::shared_ptr<BalancedNode> node) {
		auto subst = node->rotateLeft();
		if (node == m_root)
			m_root = subst;
	}

	template<class T, class Y>
	void BalancedTree<T, Y>::rebalance(std::shared_ptr<BalancedNode> node) {
		while (node) {
			int dif = node->rightHeight() - node->leftHeight();
			if (dif > 1) {
				if (node->rightChild() && node->rightChild()->leftHeight() > node->rightChild()->rightHeight())
					rotateRight(node->rightChild());
				rotateLeft(node);
			}
			if (dif < -1) {
				if (node->leftChild() && node->leftChild()->rightHeight() > node->leftChild()->leftHeight())
					rotateLeft(node->leftChild());
				rotateRight(node);
			}
			node = node->parent();
		}
	}

	template<class T, class Y>
	BalancedTree<T, Y>::~BalancedTree() {
		m_root->dropRelations();
	}
}