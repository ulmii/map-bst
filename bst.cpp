#include <iostream>

template <typename T>
class BSTree
{
    friend std::ostream& operator<<(std::ostream& os, const BSTree<T> &tree)
    {
        tree.printBST(os, tree.root);
        return os << "\n";
    }
 
    friend void swap(BSTree<T>& first, BSTree<T>& second)
    {
        using std::swap;
 
        swap(first.root, second.root);
        second.root = nullptr;
    }
 
    public:
        struct Node {
            T value{};
            Node *left = nullptr, *right = nullptr;
        };

        BSTree() : root(nullptr) { log("BST constructor"); }
        BSTree(const BSTree& other);
        BSTree(BSTree&& other) noexcept;
        ~BSTree() { log("BST destructor"); destroyer(root); }
 
        BSTree<T>& operator=(const BSTree<T>& other);
        BSTree<T>& operator=(BSTree<T>&& other);
 
        bool isEmpty() const { return root == nullptr; }
        Node* insert(T value);
        Node* search(T value) const;
        Node* remove(Node* root, const T& removeVal);
        Node* getRoot() const { return root; }
        int size() const { return helperSize(getRoot()); }
        Node* findParent(const T& x) const { return findParent(getRoot(), x); }

    private:
        Node* root;
 
        void destroyer(Node* Node)
        {
            if(Node != NULL)
            {
                destroyer(Node->left);
                destroyer(Node->right);
                delete Node;
            }
        }
 
        void copy(Node* &origin, const Node* sourceNode)
        {
            if(sourceNode == nullptr)
                origin == nullptr;
            else
            {
                origin = new Node;
                origin->value = sourceNode->value;

                copy(origin->left, sourceNode->left);
                copy(origin->right, sourceNode->right);
            }
        }
 
        //inorder
        void printBST(std::ostream& os, Node* pass) const
        {
            if (pass != nullptr)
            {
                printBST(os, pass->left);
                std::cout << pass->value << " ";
                printBST(os, pass->right);
            }
        }
 
        int helperSize(Node* root) const
        {
            if(root == nullptr)
                return 0;
            else
                return (helperSize(root->left) + 1 + helperSize(root->right));
        }
 
        void log(const char* msg) const
        {
            std::cout << "[" << this << "] " << msg << "\n";
        }

        Node* findParent(Node* node, const T& x) const
        {
            if(node == nullptr)
                return nullptr;

            if(node->left == nullptr && node->right == nullptr)
                return nullptr;
            
            if( (node->left != NULL && node->left->value == x) || (node->right != NULL && node->right->value == x))
                 return node;

            if(node->value > x)
                return findParent(node->left, x);

            if(node->value < x)
                return findParent(node->right, x);
        }
};
 
template <typename T>
BSTree<T>::BSTree(const BSTree<T>& other)
    : root(nullptr)
{
    log("BST copy constructor");
 
    if(other.root != nullptr)
    copy(this->root, other.root);
}
 
template <typename T>
BSTree<T>::BSTree(BSTree<T>&& other) noexcept
    : root(nullptr)
{
    log("BST move constructor");
 
    swap(*this, other);
}
 
template <typename T>
BSTree<T>& BSTree<T>::operator=(const BSTree<T>& other)
{
    log("BST copy assignment operator");
 
    BSTree temp(other);
    swap(*this, temp);
    return *this;
}
 
template <typename T>
BSTree<T>& BSTree<T>::operator=(BSTree<T>&& other)
{
    log("BST move assignment operator");
 
    swap(*this, other);
    return *this;
}
 
template <typename T>
auto BSTree<T>::insert(T value) -> Node*
{
    if(root == nullptr)
    {
        root = new Node;
        root->value = value;
        root->left = nullptr;
        root->right = nullptr;
 
        return root;
    }
    else
    {
        Node* temp = root;
        Node* temp2 = nullptr;
 
        Node* pass = new Node;
        pass->value = value;
        pass->left = nullptr;
        pass->right = nullptr;
 
        while(temp != nullptr)
        {
            if (value > temp->value)
            {
                temp2 = temp;
                temp = temp->right;
            }
 
            else if (value < temp->value)
            {
                temp2 = temp;
                temp = temp->left;
            }
        }
 
        if (value > temp2->value)
        {
            temp2->right = pass;
        }
        else if(value < temp2->value)
        {
            temp2->left = pass;
        }
 
        return pass;
    }
}
 
template <typename T>
auto BSTree<T>::search(T value) const -> Node*
{
    if (root == nullptr)
    {
        log("BST tree is empty");
        return nullptr;
    }
 
    Node* temp = root;
 
    while(temp != nullptr)
    {
        if (value == temp->value)
            return temp;
 
        else if (value > temp->value)
            temp = temp->right;
 
        else if (value < temp->value)
            temp = temp->left;
    }
 
    if (temp == nullptr)
        return nullptr;
}
 
template <typename T>
auto BSTree<T>::remove(Node* root, const T& removeVal) -> Node*
{
    if(root == nullptr)
        return nullptr; 

    if(root->value > removeVal) 
    { 
        root->left = remove(root->left, removeVal);
    } 
    else if(root->value < removeVal) 
    {
        root->right = remove(root->right, removeVal); 

    } 
    else 
    {
        if(root->left == nullptr) 
        {
            Node* temp = root->right;

            delete root;
            return temp;
        } 
        else if(root->right == nullptr) 
        {
            Node* temp = root->left;

            delete root;
            return temp;
        }

        Node* temp = root->right;
    
        while(temp->left != nullptr) 
            temp = temp->left;

        root->value = temp->value;
        root->right = remove(root->right, temp->value); 
  }

  return root;
}