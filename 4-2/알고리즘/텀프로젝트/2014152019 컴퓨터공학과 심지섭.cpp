// C program to demonstrate insert operation in binary search tree 
#include<stdio.h> 
#include<stdlib.h> 
#include<time.h>

struct node
{
	int key;
	struct node* left, * right;
	int height;
};

// A utility function to create a new BST node 
struct node* newNode(int item)
{
	struct node* temp = (struct node*)malloc(sizeof(struct node));
	temp->key = item;
	temp->left = temp->right = NULL;
	temp->height = 1;
	return temp;
}

int max(int a, int b);
int height(struct node* N);

void Free_Postorder(struct node* root);
struct node* BST_insert(struct node* node, int key);
int search(struct node* root, int key, int count);

struct node* rightRotate(struct node* y);
struct node* leftRotate(struct Node* x);
int getBalance(struct node* N);
struct node* insert(struct node* node, int key);
struct node* minValueNode(struct node* node);
struct node* deleteNode(struct node* root, int key);



// Driver Program to test above functions 
int main()
{
	srand(time(NULL));

	int count[] = { 100, 200, 500, 1000, 1500, 2000, 2500, 3000 };
	float BST_height[] = { 0,0,0,0,0,0,0,0 };
	float BST_SearchTime[] = { 0,0,0,0,0,0,0,0 };
	float AVL_height[] = { 0,0,0,0,0,0,0,0 };
	float AVL_SearchTime[] = { 0,0,0,0,0,0,0,0 };


	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 8; j++) {
			int* nums = (int*)malloc(sizeof(int) * count[j]);
			int key = rand() % 5000;
			nums[0] = key;
			struct node* BST_root = newNode(key);
			struct node* AVL_root = newNode(key);
			for (int k = 1; k < count[j]; k++) {
				key = rand() % 5000;
				nums[k] = key;
				BST_insert(BST_root, key);
				insert(AVL_root, key);
			}
			BST_height[j] += height(BST_root);
			AVL_height[j] += height(AVL_root);
			int target_key = nums[rand() % count[j]];
			BST_SearchTime[j] += search(BST_root, key, 0);
			AVL_SearchTime[j] += search(AVL_root, key, 0);

			free(nums);
			Free_Postorder(BST_root);
			Free_Postorder(AVL_root);
		}
	}
	
	for (int i = 0; i < 8; i++) {
		BST_height[i] /= 100.f;
		BST_SearchTime[i] /= 100.f;
		AVL_height[i] /= 100.f;
		AVL_SearchTime[i] /= 100.f;
		printf("============%d ���� ��� ��============\n\n", count[i]);
		printf(" - BST�� ���� : %f\n", BST_height[i]);
		printf(" - AVL�� ���� : %f\n\n", AVL_height[i]);
		printf(" - BST�� Ž��Ƚ�� : %f\n", BST_SearchTime[i]);
		printf(" - AVL�� Ž��Ƚ�� : %f\n\n", AVL_SearchTime[i]);
	}
	
	return 0;
}

// A utility function to do inorder traversal of BST 
void Free_Postorder(struct node* root)
{
	if (root != NULL)
	{
		Free_Postorder(root->left);
		Free_Postorder(root->right);
		free(root);
	}
}

/* A utility function to insert a new node with given key in BST */
struct node* BST_insert(struct node* node, int key)
{
	/* If the tree is empty, return a new node */
	if (node == NULL) return newNode(key);

	/* Otherwise, recur down the tree */
	if (key < node->key)
		node->left = BST_insert(node->left, key);
	else if (key > node->key)
		node->right = BST_insert(node->right, key);

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
		height(node->right));

	/* return the (unchanged) node pointer */
	return node;
}

// C function to search a given key in a given BST 
int search(struct node* root, int key, int count)
{
	count++;
	// Base Cases: root is null or key is present at root 
	if (root == NULL)
		return NULL;
	if (root->key == key)
		return count;

	// Key is greater than root's key 
	if (root->key < key)
		return search(root->right, key, count);

	// Key is smaller than root's key 
	return search(root->left, key, count);
}

// A utility function to get height 
// of the tree 
int height(struct node* N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// A utility function to get maximum 
// of two integers 
int max(int a, int b)
{
	return (a > b) ? a : b;
}

// A utility function to right 
// rotate subtree rooted with y 
// See the diagram given above. 
struct node* rightRotate(struct node* y)
{
	node* x = y->left;
	node* T2 = x->right;

	// Perform rotation 
	x->right = y;
	y->left = T2;

	// Update heights 
	y->height = max(height(y->left),
		height(y->right)) + 1;
	x->height = max(height(x->left),
		height(x->right)) + 1;

	// Return new root 
	return x;
}

// A utility function to left 
// rotate subtree rooted with x 
// See the diagram given above. 
struct node* leftRotate(struct node* x)
{
	node* y = x->right;
	node* T2 = y->left;

	// Perform rotation 
	y->left = x;
	x->right = T2;

	// Update heights 
	x->height = max(height(x->left),
		height(x->right)) + 1;
	y->height = max(height(y->left),
		height(y->right)) + 1;

	// Return new root 
	return y;
}

// Get Balance factor of node N 
int getBalance(struct node* N)
{
	if (N == NULL)
		return 0;
	return height(N->left) -
		height(N->right);
}

struct node* insert(struct node* node, int key)
{
	/* 1. Perform the normal BST rotation */
	if (node == NULL)
		return(newNode(key));

	if (key < node->key)
		node->left = insert(node->left, key);
	else if (key > node->key)
		node->right = insert(node->right, key);
	else // Equal keys not allowed 
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
		height(node->right));

	/* 3. Get the balance factor of this
		ancestor node to check whether
		this node became unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, 
	// then there are 4 cases 

	// Left Left Case 
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case 
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case 
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case 
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

/* Given a non-empty binary search tree,
return the node with minimum key value
found in that tree. Note that the entire
tree does not need to be searched. */
struct node* minValueNode(struct node* node)
{
	struct node* current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL)
		current = current->left;

	return current;
}

// Recursive function to delete a node 
// with given key from subtree with 
// given root. It returns root of the 
// modified subtree. 
struct node* deleteNode(struct node* root, int key)
{

	// STEP 1: PERFORM STANDARD BST DELETE 
	if (root == NULL)
		return root;

	// If the key to be deleted is smaller 
	// than the root's key, then it lies 
	// in left subtree 
	if (key < root->key)
		root->left = deleteNode(root->left, key);

	// If the key to be deleted is greater 
	// than the root's key, then it lies 
	// in right subtree 
	else if (key > root->key)
		root->right = deleteNode(root->right, key);

	// if key is same as root's key, then 
	// This is the node to be deleted 
	else
	{
		// node with only one child or no child 
		if ((root->left == NULL) ||
			(root->right == NULL))
		{
			struct node* temp = root->left ?
				root->left :
				root->right;

			// No child case 
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case 
				*root = *temp; // Copy the contents of 
							// the non-empty child 
			free(temp);
		}
		else
		{
			// node with two children: Get the inorder 
			// successor (smallest in the right subtree) 
			struct node* temp = minValueNode(root->right);

			// Copy the inorder successor's 
			// data to this node 
			root->key = temp->key;

			// Delete the inorder successor 
			root->right = deleteNode(root->right,
				temp->key);
		}
	}

	// If the tree had only one node 
	// then return 
	if (root == NULL)
		return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
	root->height = 1 + max(height(root->left),
		height(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF 
	// THIS NODE (to check whether this 
	// node became unbalanced) 
	int balance = getBalance(root);

	// If this node becomes unbalanced, 
	// then there are 4 cases 

	// Left Left Case 
	if (balance > 1 &&
		getBalance(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case 
	if (balance > 1 &&
		getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case 
	if (balance < -1 &&
		getBalance(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case 
	if (balance < -1 &&
		getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

// A utility function to print preorder 
// traversal of the tree. 
// The function also prints height 
// of every node 
void preOrder(struct node* root)
{
	if (root != NULL)
	{
		printf("%s",root->key);
		preOrder(root->left);
		preOrder(root->right);
	}
}