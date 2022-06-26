#ifndef __RED__BLACK__TREE__HELL__
#define __RED__BLACK__TREE__HELL__
#include <iostream>
#include <windows.h>                            //For color printing

/*There are still bugs with the DELETE operation. OMGGGGG!!!!!!!!*/
template <typename T>
class RBTree {
    enum Color {BLACK, RED};
    struct Node {
        const T key;
        Node* left;
        Node* right;
        Node* p;
        Color color;
        
        /*For visualization purposes only*/
        friend std::ostream& operator<<(std::ostream& out, const Node& u) {
            static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            int code;
            switch (u.color) {
                case BLACK: code = 0; break;
                case RED: code = 4; break;
            }
            SetConsoleTextAttribute(h, code);
            out << u.key;
            SetConsoleTextAttribute(h, 7);
            return out;
        }
    };
public:
    ~RBTree() {
        release_node(root);
    }

    void insert(const T& key) {
        Node* n = new Node{key, NIL_p, NIL_p, NIL_p, RED};          //Create a new RED Node
        
        Node* prev = NIL_p;
        Node* curr = root;
        while (curr != NIL_p) {
            prev = curr;
            if (key < curr->key) curr = curr->left;
            else curr = curr->right;
        }

        if (prev == NIL_p)          //Tree is empty
            root = n;
        else {
            n->p = prev;
            if (key < prev->key)
                prev->left = n;
            else prev->right = n;
        }

        insert_fixup(n); 
    }
    
    void erase(const T& key) {
        Node* u = const_cast<Node*>(search(key));
        if (u != NIL_p) erase(u);
    }
    
    const Node* search(const T& key) const {
        Node* curr = root;
        while (curr != NIL_p && curr->key != key) {
            if (key < curr->key) curr = curr->left;
            else curr = curr->right;
        }
        if (curr == NIL_p) return nullptr;
        return curr;
    }
    
    const Node* minimum() const {
        const Node* v = subtree_minimum(root);
        if (v == NIL_p) return nullptr;
        return v;
    }
    const Node* maximum() const {
        const Node* v = subtree_maximum(root);
        if (v == NIL_p) return nullptr;
        return v;
    }
    
    const Node* predecessor(const Node* u) const {
        if (u->left != NIL_p) return maximum(u->left);
        while (u->p != NIL_p && u->p->right != u)
            u = u->p;
        if (u->p == NIL_p) return nullptr;
        return u->p;
    }
    const Node* successor(const Node* u) const {
        if (u->right != NIL_p) return minimum(u->right);
        while (u->p != NIL_p && u->p->left != u)
            u = u->p;
        if (u->p == NIL_p) return nullptr;
        return u->p;
    }

    void print() {
        print("", root, false);
    }
private:
    void erase(Node* pos) {
        Node* x = pos;              //Node that would be removed or moved
        Node* y = NIL_p;            //Node that would take the place of x
        Color x_original_color = x->color;
        if (pos->left == NIL_p) {
            y = pos->right;
            transplant(pos, pos->right);            //pos->right can be NIL_p here
        }
        else if (pos->right == NIL_p) {
            y = pos->left;
            transplant(pos, pos->left);
        }
        else {
            x = const_cast<Node*>(subtree_minimum(pos->right));
            x_original_color = x->color;
            y = x->right;                           //y can be NIL_p here
            if (x->p != pos) {
                transplant(x, y);
                x->right = pos->right;
                pos->right->p = x;
            }
            transplant(pos, x);
            x->left = pos->left;
            pos->left->p = x;

            x->color = pos->color;
        }
        delete pos;
        if (x_original_color == BLACK)
            erase_fixup(y);
    }
    
    const Node* subtree_minimum(const Node* u) const {
        /*Returns the minimum node in the subtree rooted at u.*/
        if (u == NIL_p) return NIL_p;
        while (u->left != NIL_p)
            u = u->left;
        return u;
    }
    const Node* subtree_maximum(const Node* u) const {
        /*Returns the maximum node in the subtree rooted at u.*/
        if (u == NIL_p) return NIL_p;
        while (u->right != NIL_p)
            u = u->right;
        return u;
    }

    void transplant(Node* u, Node* v) {
        v->p = u->p;
        if (u->p == NIL_p) root = v;
        else if (u->p->left == u) u->p->left = v;
        else u->p->right = v;
    }
    
    void left_rotate(Node* u) {
        if (u->right == NIL_p) return;
        Node* v = u->right;
        v->p = u->p;
        if (u->p == NIL_p) root = v;
        else if (u->p->right == u) u->p->right = v;
        else u->p->left = v;
        v->left->p = u;
        u->right = v->left;
        v->left = u;
        u->p = v;
    }
    void right_rotate(Node* u) {
        if (u->left == NIL_p) return;
        Node* v = u->left;
        v->p = u->p;
        if (u->p == NIL_p) root = v;
        else if (u->p->right == u) u->p->right = v;
        else u->p->left = v;
        v->right->p = u;
        u->left = v->right;
        v->right = u;
        u->p = v;
    }
    
    void insert_fixup(Node* n) {
        while (n->p->color == RED) {                    //n's grandfather must be BLACK
            if (n->p == n->p->p->left) {                //n->p is a left child
                Node* m = n->p->p->right;               //m is n's uncle

                if (m->color == RED) {                  //case 1: n's uncle is RED (Which is the same color as n's father)
                    n->p->color = BLACK;                //case 1
                    m->color = BLACK;                   //case 1
                    n->p->p->color = RED;               //case 1
                    n = n->p->p;                        //case 1
                }
                else {
                    if (n == n->p->right) {             //case 2: n's uncle is BLACK and n is a right child
                        n = n->p;                       //case 2 -- tranforms case 2 to case 3
                        left_rotate(n);                 //case 2
                    }
                    n->p->color = BLACK;                //case 3: n's uncle is BLACK and n is a left child
                    n->p->p->color = RED;               //case 3
                    right_rotate(n->p->p);              //case 3
                    /*when case 2 or case 3 is entered, the loop is done.*/
                }
            }
            else {                                      //n->p is a right child
                                                        //essentially the same as when n->p is a left child
                                                        //just need to swap left and right
                                                        
                Node* m = n->p->p->left;                //m is n's uncle

                if (m->color == RED) {                  //case 1: n's uncle is RED (Which is the same color as n's father)
                    n->p->color = BLACK;                //case 1
                    m->color = BLACK;                   //case 1
                    n->p->p->color = RED;               //case 1
                    n = n->p->p;                        //case 1
                }
                else {
                    if (n == n->p->left) {              //case 2: n's uncle is BLACK and n is a right child
                        n = n->p;                       //case 2 -- tranforms case 2 to case 3
                        right_rotate(n);                //case 2
                    }
                    n->p->color = BLACK;                //case 3: n's uncle is BLACK and n is a left child
                    n->p->p->color = RED;               //case 3
                    left_rotate(n->p->p);               //case 3
                    /*when case 2 or case 3 is entered, the loop is done.*/
                }
            }
        }
        root->color = BLACK;                            //set root's color to BLACK,
                                                        //essentially the reason why the tree's black height can increase 
    }
    
    void erase_fixup(Node* n) {
        /*n could be NIL here, with its parent set appropriately and still handled correctly.*/
        while (n->color == BLACK && n != root) {
            if (n == n->p->left) {                                          //n is a left child
                Node* m = n->p->right;                                      //m is n's sibling
                if (m->color == RED) {                                      //case 1: n's sibling is RED, then n's father is BLACK
                    n->p->color = RED;                                      //case 1 -- transforms case 1 to case 2, 3 or 4
                    m->color = BLACK;                                       //case 1
                    left_rotate(n->p);                                      //case 1
                    m = n->p->right;                                        //case 1
                }
                /*If m's color is BLACK in this case, m can't be NIL. So referencing m's child is fine.*/
                if (m->left->color == BLACK && m->right->color == BLACK) {  //case 2: n's sibling is BLACK, and its children are BLACK
                    m->color = RED;                                         
                    n = n->p;
                }
                else if (m->right->color == BLACK) {                        //case 3: n's sibling is BLACK, and its right child is BLACK
                    m->left->color = BLACK;
                    m->color = RED;
                    right_rotate(m);
                    m = n->p->right;
                }
                m->color = n->p->color;                                     //case 4: n's sibling is BLACK, and its right child is RED
                n->p->color = BLACK;
                m->right->color = BLACK;
                left_rotate(n->p);
                n = root;
            }
            else {                                                          //essentially the same, but with left, right swapped
                Node* m = n->p->left;                      
                if (m->color == RED) {                      
                    n->p->color = RED;                      
                    m->color = BLACK;                       
                    right_rotate(n->p);                      
                    m = n->p->left;                        
                }
                if (m->left->color == BLACK && m->right->color == BLACK) {
                    m->color = RED;                                         
                    n = n->p;
                }
                else if (m->left->color == BLACK) {                      
                    m->right->color = BLACK;
                    m->color = RED;
                    left_rotate(m);
                    m = n->p->left;
                }
                m->color = n->p->color;
                n->p->color = BLACK;
                m->left->color = BLACK;
                right_rotate(n->p);
                n = root;
            }
        }
        n->color = BLACK;
    }

    void release_node(Node* u) {
        /*Deallocate resources*/
        //Used in destructor

        if (u == NIL_p) return;
        release_node(u->left);
        release_node(u->right);
        delete u;    
    }

    /*Stolen function from: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c*/
    void print(const std::string& prefix, const Node* curr, bool isLeft)
    {
        if( curr != NIL_p )
        {
            std::cout << prefix;

            std::cout << (isLeft ? "├──" : "└──" );

            // print the value of the node
            std::cout << *curr << std::endl;

            // enter the next tree level - left and right branch
            print( prefix + (isLeft ? "│   " : "    "), curr->left, true);
            print( prefix + (isLeft ? "│   " : "    "), curr->right, false);
        }
    }

        Node* root = NIL_p;
    
    //using sentinel is more convenient as you won't have to worry about dereferencing nullptr.
    static Node NIL;                            //NIL is BLACK                
    constexpr static Node* NIL_p = &NIL;        //pointer to sentinel Node
    
};

template <typename T>
typename RBTree<T>::Node RBTree<T>::NIL = {0, NIL_p, NIL_p, NIL_p, BLACK};
#endif