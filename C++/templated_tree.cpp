#include <iostream>

using namespace std;

template<typename T>
class Container
{
public:
    virtual void insert(T value) = 0;
    virtual bool exists(T value) = 0;
    virtual void remove(T value) = 0;

    virtual void print() = 0;

    virtual ~Container();
};

template<typename T>
Container<T>::~Container() { };

template<typename T>
struct tree_element {
    T value;
    struct tree_element* left;
    struct tree_element* right;
    unsigned long height;
};

template<typename T>
class SimpleTree: public Container<T>
{
public:
    SimpleTree();
    ~SimpleTree();
    void insert(T value);
    bool exists(T value);
    void remove(T value);
    void print();

protected:
    struct tree_element<T>* root;

    struct tree_element<T>* create(T value);
    struct tree_element<T>* search_by_value(struct tree_element<T>* elem, T value);

    void remove_branch(struct tree_element<T>* elem);
    struct tree_element<T>* remove_from_branch(struct tree_element<T>* elem, T value);

    struct tree_element<T>* insert_into_branch(struct tree_element<T>* local_root, struct tree_element<T>* elem);

    void print_branch(struct tree_element<T>* elem);

private:
    unsigned long get_height(struct tree_element<T>* elem);
    int balance_factor(struct tree_element<T>* elem);
    void fix_height(struct tree_element<T>* elem);

    struct tree_element<T>* rotate_right(struct tree_element<T>* p);
    struct tree_element<T>* rotate_left(struct tree_element<T>* q);

    struct tree_element<T>* balance(struct tree_element<T>* elem);

    struct tree_element<T>* find_min(struct tree_element<T>* elem);
    struct tree_element<T>* remove_min(struct tree_element<T>* elem);
};

template<typename T>
unsigned long SimpleTree<T>::get_height(struct tree_element<T>* elem)
{
    return elem?elem->height:0;
}

template<typename T>
int SimpleTree<T>::balance_factor(struct tree_element<T>* elem)
{
    return get_height(elem->right) - get_height(elem->left);
}

template<typename T>
void SimpleTree<T>::fix_height(struct tree_element<T>* elem)
{
    unsigned long h_left = get_height(elem->left);
    unsigned long h_right = get_height(elem->right);

    elem->height = (h_left>h_right?h_left:h_right) + 1;
}

template<typename T>
struct tree_element<T>* SimpleTree<T>::rotate_right(struct tree_element<T>* p)
{
	struct tree_element<T>* q = p->left;
	p->left = q->right;
	q->right = p;
	fix_height(p);
	fix_height(q);
	return q;
}

template<typename T>
struct tree_element<T>* SimpleTree<T>::rotate_left(struct tree_element<T>* q)
{
	struct tree_element<T>* p = q->right;
	q->right = p->left;
	p->left = q;
	fix_height(q);
	fix_height(p);
	return p;
}

template<typename T>
struct tree_element<T>* SimpleTree<T>::balance(struct tree_element<T>* elem)
{
	fix_height(elem);

	if(balance_factor(elem) == 2)
	{
		if(balance_factor(elem->right) < 0)
			elem->right = rotate_right(elem->right);

		return rotate_left(elem);
	}
	if(balance_factor(elem) == -2)
	{
		if(balance_factor(elem->left) > 0)
			elem->left = rotate_left(elem->left);
		return rotate_right(elem);
	}

	return elem;
}

template<typename T>
SimpleTree<T>::SimpleTree()
{
    root = NULL;
    cout << "tree created" << endl;
};

template<typename T>
struct tree_element<T>* SimpleTree<T>::create(T value)
{
    struct tree_element<T>* elem = new struct tree_element<T>;
    elem->value = value;
    elem->left = NULL;
    elem->right = NULL;
    elem->height = 1;

    return elem;
};

template<typename T>
SimpleTree<T>::~SimpleTree()
{
    remove_branch(root);
    cout << "tree deleted";
}

template<typename T>
void SimpleTree<T>::remove_branch(struct tree_element<T>* elem)
{
    if(elem != NULL)
    {
        if(elem->left != NULL)
            remove_branch(elem->left);
        if(elem->right != NULL)
            remove_branch(elem->right);
        delete elem;
    }
}

template<typename T>
void SimpleTree<T>::insert(T value)
{
    root = insert_into_branch(root, create(value));
    cout << "inserted " << value << endl;
}

template<typename T>
struct tree_element<T>* SimpleTree<T>::insert_into_branch(struct tree_element<T>* local_root, struct tree_element<T>* elem) // ***
{
    if(local_root == NULL)
    {
        return elem;
    }

	if(elem->value < local_root->value)
		local_root->left = insert_into_branch(local_root->left, elem);
	else
		local_root->right = insert_into_branch(local_root->right, elem);

	return balance(local_root);
}

template<typename T>
bool SimpleTree<T>::exists(T value)
{
    return (search_by_value(root, value) != NULL);
}

template<typename T>
struct tree_element<T>* SimpleTree<T>::search_by_value(struct tree_element<T>* elem, T value)
{
    if(elem == NULL)
        return NULL;

    if(value == elem->value)
    {
        return elem;
    }

    else if(value < elem->value && elem->left != NULL)
        return search_by_value(elem->left, value);

    else if(value > elem->value && elem->right != NULL)
        return search_by_value(elem->right, value);

    return NULL;
}

template<typename T>
void SimpleTree<T>::print()
{
   print_branch(root);
   cout << "/ root is: " << root->value << endl;
}

template<typename T>
void SimpleTree<T>::print_branch(struct tree_element<T>* elem)
{
    if(elem != NULL)
    {
        if(elem->left != NULL)
            print_branch(elem->left);

        cout << elem->value << " ";

        if(elem->right != NULL)
            print_branch(elem->right);
    }
}

template<typename T>
struct tree_element<T>* SimpleTree<T>::find_min(struct tree_element<T>* elem)
{
    return elem->left?find_min(elem->left):elem;
}

template<typename T>
struct tree_element<T>* SimpleTree<T>::remove_min(struct tree_element<T>* elem)
{
	if(elem->left == NULL)
    {
        return elem->right;
    }

	elem->left = remove_min(elem->left);
	return balance(elem);
}

template<typename T>
void SimpleTree<T>::remove(T value)
{
    root = remove_from_branch(root, value);
}

template<typename T>
struct tree_element<T>* SimpleTree<T>::remove_from_branch(struct tree_element<T>* elem, T value)
{
    if(elem == NULL)
    {
        return NULL;
    }

    if(value < elem->value)
        elem->left = remove_from_branch(elem->left, value);
    else if(value > elem->value)
        elem->right = remove_from_branch(elem->right, value);
    else
    {
        struct tree_element<T>* q = elem->left;
        struct tree_element<T>* r = elem->right;

        delete elem;

        if(r == NULL)
        {
            return q;
        }

        struct tree_element<T>* min = find_min(r);
        min->right = remove_min(r);
        min->left = q;
        return balance(min);
    }

    return balance(elem);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, Container<T>* m)
{
      m->print();
      return os;
}

int main()
{
    Container<float>* c = new SimpleTree<float>();

    for(float i=-0.5; i<5; i++)
    {
        c->insert(i);
    }

    cout << c << endl;

    cout << "0: " << c->exists(0) << " ; " << "1: " << c->exists(1) << " ; " << "3.5: " << c->exists(3.5) << endl;

    cout << "removed 0.5 and 1.5" << endl;
    c->remove(0.5);
    c->remove(1.5);

    cout << c << endl;

    delete c;

    return 0;
}
