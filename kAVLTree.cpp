/*
	Derek Garcia
	CMPSC 130A
	Decemeber 18, 2020
	Project 3
	kAVLTree.cpp
*/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

int string_to_int(string inputString)
{
	stringstream ss(inputString);
	int x = 0;
	ss >> x;
	return x;
}

class AVLtree
{
public:
	class AVLNode
	{
	public:
		int wholePart;
		int fraction;

		AVLNode *left;
		AVLNode *right;

		int height;

		//Default Constructor
		AVLNode()
		{
			wholePart = fraction = NULL;
			left = right = nullptr;
			height = NULL;
		}

		//Parameter Constructor
		AVLNode(int w, int f, AVLNode *l, AVLNode *r)
		{
			wholePart = w;
			fraction = f;
			left = l;
			right = r;
			height = 0;
		}
	};

	string totalString;
	int key;
	int minDifference;
	int kValue;
	AVLNode *root;
	AVLNode *tempAVLNode;
	//Methods to be added
	//1. Constructor: Takes K as parameter and constructs an empty k-AVL tree xWorking
	//2. Insert: Insert a Value into the k-AVL tree xWorking
	//3. Delete: Delete a value from the k-AVL tree. x
	//4. Search: Search a value in the k-AVL tree. xWorking
	//5. Approximate Search: Find the closest value in the k-AVL tree. xWorking
	//6. In Order Print: Print the tree according to an in-order traversal (Left, Root, Right)   xFix to add helper function
	//7. Pre Order Print: Print the tree according to a pre-order traversal (Root, Left, Right)  xFix to add helper function
	//8. Destructor: Clean Up Memory x

	//Constructor
	AVLtree()
	{
		AVLNode *root = new AVLNode;
		kValue = 1;
	}

	AVLtree(int inputKvalue)
	{
		AVLNode *root = new AVLNode;
		kValue = inputKvalue;
	}

	//Destructor
	~AVLtree()
	{
		//Make the tree empty
		makeEmpty(root);
	}


	//For destructor to clean up memory
	void makeEmpty(AVLNode *t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}

		t = nullptr;
	}

	//Return the height of node t or -1 if nullptr
	int height(AVLNode *t) const
	{
		return t == nullptr ? -1 : t->height;
	}

	//Insert Helper function
	void insertH(int inputWhole, int inputFraction)
	{
		insert(inputWhole, inputFraction, root);
	}

	//Insert method
	void insert(int inputWhole, int inputFraction, AVLNode *&t)
	{
		if (t == nullptr)
		{
			t = new AVLNode{ inputWhole, inputFraction, nullptr, nullptr };
			cout << inputWhole << "." << inputFraction << " inserted" << endl;
		}
		else if ((inputWhole < t->wholePart) || (inputWhole == t->wholePart && inputFraction < t->fraction))
			insert(inputWhole, inputFraction, t->left);
		else if ((inputWhole > t->wholePart) || (inputWhole == t->wholePart && inputFraction > t->fraction))
			insert(inputWhole, inputFraction, t->right);

		balance(t);
	}

	//Balancing the tree
	void balance(AVLNode *&t)
	{
		if (t == nullptr)
			return;

		if ((height(t->left) - height(t->right)) > kValue)
			if (height(t->left->left) >= height(t->left->right))
				rotateWithLeftChild(t);
			else
				doubleWithLeftChild(t);
		else if (height(t->right) - height(t->left) > kValue)
			if (height(t->right->right) >= height(t->right->left))
				rotateWithRightChild(t);
			else
				doubleWithRightChild(t);

		t->height = max(height(t->left), height(t->right)) + 1;
	}

	//Delete Helper Function to start from the root
	void deleteNodeH(int inputWhole, int inputFraction)
	{
		deleteNode(inputWhole, inputFraction, root);
		cout << inputWhole << "." << inputFraction << " deleted" << endl;
	}

	//Delete Function
	void deleteNode(int inputWhole, int inputFraction, AVLNode *&t)
	{
		//Item not found
		if (t == nullptr)
			return;

		if ((inputWhole < t->wholePart) || (inputWhole == t->wholePart && inputFraction < t->fraction))
			deleteNode(inputWhole, inputFraction, t->left);
		else if ((inputWhole > t->wholePart) || (inputWhole == t->wholePart && inputFraction > t->fraction))
			deleteNode(inputWhole, inputFraction, t->right);
		else if (t->left != nullptr && t->right != nullptr) //Two Children Case
		{

			t->wholePart = findMin(t->right)->wholePart;
			t->fraction = findMin(t->right)->fraction;
			deleteNode(t->wholePart, t->fraction, t->right);
		}
		else
		{
			AVLNode *oldNode = t;
			t = (t->left != nullptr) ? t->left : t->right;
			delete oldNode;
		}

		balance(t);
	}

	//Finding the Smallest Node for delete function
	AVLNode* findMin(AVLNode *t) const
	{
		if (t == nullptr)
			return nullptr;
		if (t->left == nullptr)
			return t;

		return findMin(t->left);
	}

	//Search helper function to start it at the root
	void searchH(int inputWhole, int inputFraction)
	{
		search(inputWhole, inputFraction, root);
	}

	//Search Function
	void search(int inputWhole, int inputFraction, AVLNode *t)
	{
		if (t == nullptr) //Word Not Found
		{
			cout << inputWhole << "." << inputFraction << " not found" << endl;
			return; 
		}
		else if ((inputWhole < t->wholePart) || (inputWhole == t->wholePart && inputFraction < t->fraction))
			search(inputWhole, inputFraction, t->left);
		else if ((inputWhole > t->wholePart) || (inputWhole == t->wholePart && inputFraction > t->fraction))
			search(inputWhole, inputFraction, t->right);
		else //Word Found
		{
			cout << inputWhole << "." << inputFraction << " found" << endl;
			return;
		}
	}

	//Approximate Search Helper Function
	void approxSearchH(int inputWhole, int inputFraction)
	{
		minDifference = INT_MAX;
		key = (inputWhole * 10) + inputFraction;
		approxSearch(root);

		if (minDifference != INT_MAX)
		{
			cout << "closest to " << inputWhole << "." << inputFraction << " is " << tempAVLNode->wholePart << "." << tempAVLNode->fraction << endl;
		}

	}

	//have it return the node with the lowest difference
	//Approximate Search
	void approxSearch(AVLNode *t)
	{
		//Base
		if (t == nullptr)
		{
			return;
		}

		int nodeKey = (t->wholePart * 10) + t->fraction;
		int tempDiff = abs(key - nodeKey);
		if (tempDiff < minDifference)
		{
			minDifference = tempDiff;
			tempAVLNode = t;
		}

		approxSearch(t->left);
		approxSearch(t->right);
		
	}

	//Single Rotate Left
	void rotateWithLeftChild(AVLNode *&t)
	{
		AVLNode *tempT = t->left;
		t->left = tempT->right;
		tempT->right = t;
		t->height = max(height(t->left), height(t->right)) + 1;
		tempT->height = max(height(tempT->left), t->height) + 1;
		t = tempT;
	}

	//Double Rotate Left
	void doubleWithLeftChild(AVLNode *&t)
	{
		rotateWithRightChild(t->left);
		rotateWithLeftChild(t);
	}

	//Single Rotate Right
	void rotateWithRightChild(AVLNode *&t)
	{
		AVLNode *tempT = t->right;
		t->right = tempT->left;
		tempT->left = t;
		t->height = max(height(t->left), height(t->right)) + 1;
		tempT->height = max(height(tempT->right), t->height) + 1;
		t = tempT;
	}

	//Double Rotate Right
	void doubleWithRightChild(AVLNode *&t)
	{
		rotateWithLeftChild(t->right);
		rotateWithRightChild(t);
	}

	//InOrder Traversal Helper Function
	void inOrderH()
	{
		totalString = "";
		inOrder(root);
		totalString.pop_back();
		cout << totalString << endl;
	}

	//InOrder Traversal Function (Left Root Right)
	void inOrder(AVLNode *t)
	{
		if (t == nullptr)
			return;

		//Left Root Right
		/*
		inOrder(t->left);
		cout << t->wholePart << "." << t->fraction << " ";
		inOrder(t->right);
		*/

		inOrder(t->left);
		totalString.append(to_string(t->wholePart));
		totalString.append(".");
		totalString.append(to_string(t->fraction));
		totalString.append(" ");
		inOrder(t->right);

	}

	//PreOrder Traversal Helper Function
	void preOrderH()
	{
		totalString = "";
		preOrder(root);
		totalString.pop_back();
		cout << totalString << endl;
	}

	//PreOrder Traversal Function (Root Left Right)
	void preOrder(AVLNode *t)
	{
		if (t == nullptr)
			return;

		//Root Left Right
		/*
		cout << t->wholePart << "." << t->fraction << " ";
		preOrder(t->left);
		preOrder(t->right);
		*/

		totalString.append(to_string(t->wholePart));
		totalString.append(".");
		totalString.append(to_string(t->fraction));
		totalString.append(" ");
		preOrder(t->left);
		preOrder(t->right);
	}
};


int main(int argc, char *argv[])
{
	//Driver

	//Parsing Time
	
	string Commands = argv[1];

	//string Commands = "2, insert 2 2, insert 5 2, search 2 2, approx_search 5 1, in_order, pre_order, delete 2 2";
	//Vector to hold the Parsed Words
	vector<string> ParsedCmds;

	//Used for Parsing
	stringstream s_stream(Commands);

	//Parsing the commas in the commands
	while (s_stream.good())
	{
		string substr;
		getline(s_stream, substr, ',');
		ParsedCmds.push_back(substr);
	}

	//Converting the first argument into a string
	//For our kValue
	//ParsedCmds[0] should have our kValue

	//Create a Tree with kValue
	int kValue = string_to_int(ParsedCmds[0]);
	AVLtree Tree(kValue);
	
	//Removing the space at the beginning of each word after index 1
	for (int i = 1; i < ParsedCmds.size(); i++)
	{
		ParsedCmds[i].erase(0, 1);
	}

	for (int i = 1; i < ParsedCmds.size(); i++)
	{
		//Temp Vector to hold both arguments and commands
		vector<string> ParsedCmds2;

		//Sstream used for parsing
		stringstream s_stream2(ParsedCmds[i]);

		//parsing the space between the command and argument
		while (s_stream2.good())
		{
			string substr2;
			getline(s_stream2, substr2, ' ');
			ParsedCmds2.push_back(substr2);
		}

		//Translating into commands
		//ParsedCmds2[0] will be the command while the follow will be the arguments

		if (ParsedCmds2[0] == "search")
		{
			Tree.searchH(string_to_int(ParsedCmds2[1]), string_to_int(ParsedCmds2[2]));
		}
		else if (ParsedCmds2[0] == "approx_search")
		{
			Tree.approxSearchH(string_to_int(ParsedCmds2[1]), string_to_int(ParsedCmds2[2]));
		}
		else if (ParsedCmds2[0] == "insert")
		{
			Tree.insertH(string_to_int(ParsedCmds2[1]), string_to_int(ParsedCmds2[2]));
		}
		else if (ParsedCmds2[0] == "delete")
		{
			Tree.deleteNodeH(string_to_int(ParsedCmds2[1]), string_to_int(ParsedCmds2[2]));
		}
		else if (ParsedCmds2[0] == "in_order")
		{
			Tree.inOrderH();
		}
		else if (ParsedCmds2[0] == "pre_order")
		{
			Tree.preOrderH();
		}
		else
		{
			cout << "***Invalid Command***" << endl;
		}
	}

	return EXIT_SUCCESS;
}
