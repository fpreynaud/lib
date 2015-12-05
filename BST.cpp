#ifndef BST_H
#define BST_H
#include <iostream>
#include <string>
#include <vector>

template<class T>
class Node
{
	private:
		T value;
		Node<T>* left;
		Node<T>* right;

	public:
		Node(): left(0), right(0) {}

		Node(T v): value(v), left(0), right(0) {}

		Node(const Node<T>& src) : value(src.value)
		{
			if(src.left)
				left = new Node<T>(*(src.left));
			else
				left = 0;

			if(src.right)
				right = new Node<T>(*(src.right));
			else
				right = 0;
		}

		~Node()
		{
			delete left;
			delete right;
		}

		T& get(T v)
		{
			if(v < value)
				if(left)
					return left->get(v);
				else
					throw std::string("not found\n");
			else if(v > value)
				if(right)
					return right->get(v);
				else
					throw std::string("not found\n");
			else
				return value;
		}

		void add(T v)
		{
			if(v < value)
			{
				if(left)
					left->add(v);
				else
					left = new Node<T>(v);
			}
			else if(v > value)
			{
				if(right)
					right->add(v);
				else
					right = new Node<T>(v);
			}
			else
				value = v;
		}

		void print()
		{
			if(!left && !right)
				std::cout << "Leaf(" << value << ")";
			else
			{
				std::cout << "Node(" << value << ", ";
				if(left)
				{
					left->print();
					std::cout << ", ";
				}
				else
					std::cout << "Empty, ";

				if(right)
				{
					right->print();
					std::cout << ")";
				}
				else
					std::cout << "Empty)";
			}
		}

		Node<T>* remove(T v)
		{
			if(v < value)
			{
				if(left)
					left = left->remove(v);
				return this;
			}
			else if(v > value)
			{
				if(right)
					right = right->remove(v);
				return this;
			}
			else
			{
				return merge(left, right);
			}
		}

		static Node<T>* merge(Node<T>* receiver, Node<T>* annex)
		{
			if(!receiver)
				return annex;
			else if(!annex)
				return receiver;

			T vr = receiver->value, va = annex->value;

			if(va < vr)
				receiver->left = merge(receiver->left, annex);
			else if(va > vr)
				receiver->right = merge(receiver->right, annex);
			else
			{
				receiver->left = merge(receiver->left, annex->left);
				receiver->right = merge(receiver->right, annex->right);
			}

			return receiver;
		}

		bool contains(T v)
		{
			if(v == value)
				return true;

			if(!left)
			{
				if(right)
					return right->contains(v);
			}
			else if(!right)
			{
				if(left)
					return left->contains(v);
			}
			else
				return (left->contains(v) || right->contains(v));

			return false;
		}
};

template<class T>
class BST
{
	private:
		Node<T>* root;

	public:
		BST() : root(0) {};

		//BST(const BST<T>& src);

		~BST() { delete root; };

		T& get(T v)
		{
			if(isEmpty())
				throw std::string("not found\n");

			return root->get(v);
		}

		void add(T v)
		{
			if(isEmpty())
				root = new Node<T>(v);
			else
				root->add(v);
		}

		void print()
		{
			if(isEmpty())
				std::cout << "Empty";
			else
				root->print();
			std::cout << "\n";
		}

		bool isEmpty() { return root == 0; }

		void remove(T v)
		{
			if(!isEmpty())
				root = root->remove(v);
		}

		BST<T>& merge(BST<T>& t)
		{
			if(!root)
				return t;

			root = Node<T>::merge(root, t.root);
			return *this;
		}

		bool contains(T v)
		{
			if(isEmpty())
				return false;
			else
				return root->contains(v);
		}
};
#endif
