/////////////////////////////////////
// florestan's tree implementation //
/////////////////////////////////////
#ifndef FLORESTAN_TREE_H
#define FLORESTAN_TREE_H
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

template <typename valuetype> class tree {
protected:
    struct node {
        valuetype           value;
        node*               parent = NULL;
        std::vector<node*>  child;
        ~node();
    };
    node*                   topnode = NULL;
    size_t                  _size = 0;
    virtual node*           newnode(const valuetype &newvalue, node* parent, node* child = NULL);
public:
    struct error {
        class empty;
        class no_parent;
        class no_children;
        class child_index_not_exist;
        class child_value_not_exist;
        class node_not_exist;
    };
    class iterator {
    private:
        tree*               _tree = NULL;
        node*               _cursor;
        inline void         checkempty() const;
        inline void         checkcursor() const;
        inline size_t       depth(node* cursor) const;
    public:
        iterator(const tree& db);
        iterator(const tree& db, node* cursor);
        iterator(const tree& db, const node* cursor);
        valuetype&          operator*();
        std::vector<node*>  children() const;
        iterator            child_index(const size_t &childindex) const;
        iterator            parent() const;
        virtual iterator&   go_child_index(const size_t &childindex);
        virtual iterator&   go_child_value(const valuetype &findvalue);
        iterator&           go_parent();
        virtual iterator&   insert_child(const valuetype &newvalue);
        iterator&           erase();
        virtual iterator&   erase_child_index(const size_t &childindex);
        virtual iterator&   erase_child_value(const valuetype &findvalue);
        iterator&           print() const;
        inline size_t       depth() const;
    };
    tree();
    ~tree();
    inline tree&            clear();
    inline size_t           size() const;
    inline bool             empty() const;
    inline iterator         top() const;
    inline iterator         insert_top(const valuetype &newvalue);
    tree&                   print_preorder()   const;
    tree&                   print_postorder()  const;
    tree&                   print_depthorder() const;
    tree&                   print_preorder(const node* _node) const;
    tree&                   print_postorder(const node* _node) const;
    tree&                   print_depthorder(const node* _node) const;
    std::vector<valuetype*> preorder()   const;
    std::vector<valuetype*> postorder()  const;
    std::vector<valuetype*> depthorder() const;
    std::vector<valuetype*> preorder(const node* _node) const;
    std::vector<valuetype*> postorder(const node* _node) const;
    std::vector<valuetype*> depthorder(const node* _node) const;
};

#define template_v                  template <typename valuetype>
#define tree_v                      tree<valuetype>
#define t_node                      tree<valuetype>::node
#define t_node_t        typename    tree<valuetype>::node
#define t_error                     tree<valuetype>::error
#define t_error_t       typename    tree<valuetype>::error
#define t_iterator                  tree<valuetype>::iterator
#define t_iterator_t    typename    tree<valuetype>::iterator
template_v tree_v::node::~node() {
    if (!child.empty()) for (node* c : child) if (c != NULL) delete c;
}
template_v t_node_t* tree_v::newnode(const valuetype &newvalue, node* parent, node* child /* = NULL */) {
    node* _newnode;
    if (parent == NULL) {
        if (child == NULL) _newnode = new node{newvalue, NULL, {}}; // new top
        else _newnode = new node{newvalue, NULL, {child}}; // new parent of top
    }
    else _newnode = new node{newvalue, parent, {}}, parent->child.push_back(_newnode); // new child
    return _newnode;
}
template_v class t_error::empty                 : public std::runtime_error { public: empty() : std::runtime_error("The tree is empty, while trying to access its node.") {} };
template_v class t_error::no_parent             : public std::runtime_error { private: const char* msg(const t_node_t* i) const { char* msg = (char*)malloc(256); snprintf(msg, 256, "The tree node(%p) doesn't have any parent to access.", (void*)i); return msg; } public: no_parent(const node* i) : std::runtime_error(msg(i)) {} };
template_v class t_error::no_children           : public std::runtime_error { private: const char* msg(const node* i) const { char* msg = (char*)malloc(256); snprintf(msg, 256, "The tree node(%p) doesn't have any children to access.", (void*)i); return msg; } public: no_children(const node* i) : std::runtime_error(msg(i)) {} };
template_v class t_error::child_index_not_exist : public std::runtime_error { private: const char* msg(const node* i, const size_t &childsize, const size_t &accessing) const { char* msg = (char*)malloc(256); if (childsize == 1) snprintf(msg, 256, "The tree node(%p) has 1 child, while trying to access the children index #%zu.", (void*)i, accessing); else snprintf(msg, 256, "The tree node(%p) has %zu children, while trying to access the children index #%zu.", (void*)i, childsize, accessing); return msg; } public: child_index_not_exist(const node* i, const size_t &childsize, const size_t &accessing) : std::runtime_error(msg(i, childsize, accessing)) {} };
template_v class t_error::child_value_not_exist : public std::runtime_error { private: const char* msg(const node* i, const valuetype &value) const { char* msg = (char*)malloc(256); snprintf(msg, 256, "The tree node(%p) has no children matching the value of %s", (void*)i, std::to_string(value).c_str()); return msg; } public: child_value_not_exist(const node* i, const valuetype &value) : std::runtime_error(msg(i, value)) {} };
template_v class t_error::node_not_exist        : public std::runtime_error { private: const char* msg(const node* i) const { char* msg = (char*)malloc(256); snprintf(msg, 256, "The iterator(%p) doens't indicate any actual tree node.", (void*)i); return msg; } public: node_not_exist(const node* i) : std::runtime_error(msg(i)) {} };
template_v inline void t_iterator::checkempty() const { if (_tree->empty()) throw t_error_t::empty();}
template_v inline void t_iterator::checkcursor() const { if (_cursor == NULL) throw t_error_t::node_not_exist(_cursor); }
template_v inline size_t t_iterator::depth(t_node_t* cursor) const { return (cursor->parent == NULL ? 0 : depth(cursor->parent) + 1); }
template_v t_iterator::iterator(const tree& db): _tree((tree*)&db) { _cursor = (db.empty() ? NULL : db.topnode); }
template_v t_iterator::iterator(const tree& db, node* cursor): _tree((tree*)&db), _cursor(cursor) { checkempty(); checkcursor(); }
template_v t_iterator::iterator(const tree& db, const node* cursor): _tree((tree*)&db), _cursor((node*)cursor) { checkempty(); checkcursor(); }
template_v std::vector<t_node_t*> t_iterator::children() const { return _cursor->child; }
template_v t_iterator_t t_iterator::child_index(const size_t &childindex) const { iterator new_iterator(_tree, _cursor); return new_iterator.go.child(childindex); }
template_v t_iterator_t t_iterator::parent() const { iterator new_iterator(_tree, _cursor); return new_iterator.go.parent(); }
template_v t_iterator_t& t_iterator::go_child_index(const size_t &childindex) {
    checkempty(); checkcursor();
    if (_cursor->child.empty()) throw t_error_t::no_children(_cursor);
    if (_cursor->child.size() <= childindex) throw t_error_t::child_index_not_exist(_cursor, _cursor->child.size(), childindex);
    _cursor = _cursor->child[childindex];
    return *this;
}
template_v t_iterator_t& t_iterator::go_child_value(const valuetype &findvalue) {
    checkempty(); checkcursor();
    if (_cursor->child.empty()) throw t_error_t::no_children(_cursor);
    for (size_t i = 0; i < _cursor->child.size(); ++i) if (_cursor->child[i]->value == findvalue) { _cursor = _cursor->child[i]; return *this; }
    throw t_error_t::child_value_not_exist(_cursor, findvalue);
}
template_v t_iterator_t& t_iterator::go_parent() {
    checkempty(); checkcursor();
    if (_cursor->parent == NULL) throw t_error_t::no_parent(_cursor);
    _cursor = _cursor->parent;
    return *this;
}
template_v t_iterator_t& t_iterator::insert_child(const valuetype &newvalue) {
    if (_tree->empty()) _tree->topnode = _tree->newnode(newvalue, NULL), _cursor = _tree->topnode, _tree->_size = 1;
    else checkcursor(), _tree->newnode(newvalue, _cursor), _tree->_size++;
    return *this;
}
template_v t_iterator_t& t_iterator::erase() {
    checkempty(); checkcursor();
    if (_cursor == _tree->topnode) _tree->clear(), _cursor = NULL;
    else {
        node* _parent = _cursor->parent;
        for (size_t i = 0; i < _parent->child.size(); ++i) if (_parent->child[i] == _cursor) { typename std::vector<node*>::iterator it_c = _parent->child.begin() + i; _parent->child.erase(it_c); break; }
        delete _cursor;
        _cursor = _parent;
    }
    _tree->_size--;
    return *this;
}
template_v t_iterator_t& t_iterator::erase_child_index(const size_t &childindex) {
    checkempty(); checkcursor();
    if (_cursor->child.empty()) throw t_error_t::no_children(_cursor);
    if (_cursor->child.size() <= childindex) throw t_error_t::child_index_not_exist(_cursor, _cursor->child.size(), childindex);
    typename std::vector<node*>::iterator it_c = _cursor->child.begin() + childindex; delete *it_c; _cursor->child.erase(it_c); _tree->_size--;
    return *this;
}
template_v t_iterator_t& t_iterator::erase_child_value(const valuetype &findvalue) {
    checkempty(); checkcursor();
    if (_cursor->child.empty()) throw t_error_t::no_children(_cursor);
    for (size_t i = 0; i < _cursor->child.size(); ++i) if (_cursor->child[i]->value == findvalue) { typename std::vector<node*>::iterator  it_c = _cursor->child.begin() + i; delete *it_c; _cursor->child.erase(it_c); _tree->_size--; return *this; }
    throw t_error_t::child_value_not_exist(_cursor, findvalue);
}
template_v valuetype& t_iterator::operator*() { checkempty(); checkcursor(); return _cursor->value; }
template_v t_iterator_t& t_iterator::print() const {
    checkempty(); checkcursor();
    std::cout << "Tree size: " << _tree->_size << '\n';
    std::cout << "Current cursor: " << _cursor << '(' << _cursor->value << ')' << '\n';
    std::cout << "Its depth: " << depth() << '\n';
    std::cout << "Its value: " << _cursor->value << '\n';
    std::cout << "Its parent: "; (_cursor->parent != NULL ? std::cout << _cursor->parent << '(' << _cursor->parent->value << ')' : std::cout << "NULL") << '\n';
    std::cout << "Its children (total " << _cursor->child.size() << "): { ";
    if (!_cursor->child.empty()) for (node* i : _cursor->child) std::cout << (void*)i << '(' << i->value << ')' << ' ';
    std::cout << "}\n";
    return *(iterator*)this;
}
template_v inline size_t t_iterator::depth() const { checkempty(); checkcursor(); return depth(_cursor); }
template_v tree_v::tree() {}
template_v tree_v::~tree() { if (topnode != NULL) delete topnode; }
template_v inline tree_v& tree_v::clear() { if (topnode != NULL) delete topnode; topnode = NULL; _size = 0; return *this; }
template_v inline size_t tree_v::size() const { return _size; }
template_v inline bool tree_v::empty() const { return (_size == 0 || topnode == NULL); }
template_v inline t_iterator_t tree_v::top() const { if (empty()) { throw t_error_t::empty(); } else return iterator(*this, topnode); }
template_v inline t_iterator_t tree_v::insert_top(const valuetype &newvalue) {
    if (empty()) topnode = newnode(newvalue, NULL), _size = 1;
    else topnode->parent = newnode(newvalue, NULL, topnode), topnode = topnode->parent, ++_size;
    return iterator(*this, topnode);
}   
template_v tree_v& tree_v::print_preorder()   const { if (topnode != NULL) std::cout << "Preorder from the top: ",   print_preorder(topnode),   std::cout << '\n'; else std::cout << "The tree is empty.\n"; return *(tree*)this; }
template_v tree_v& tree_v::print_postorder()  const { if (topnode != NULL) std::cout << "Postorder from the top: ",  print_postorder(topnode),  std::cout << '\n'; else std::cout << "The tree is empty.\n"; return *(tree*)this; }
template_v tree_v& tree_v::print_depthorder() const { if (topnode != NULL) std::cout << "depthorder from the top: ", print_depthorder(topnode), std::cout << '\n'; else std::cout << "The tree is empty.\n"; return *(tree*)this; }
template_v tree_v& tree_v::print_preorder(const t_node_t* _node) const { std::cout << (void*)_node << '(' << _node->value << ')' << ' '; for (const node* c : _node->child) print_preorder(c); return *(tree*)this; }
template_v tree_v& tree_v::print_postorder(const t_node_t* _node) const { for (const node* c : _node->child) print_postorder(c); std::cout << (void*)_node << '(' << _node->value << ')' << ' '; return *(tree*)this; }
template_v tree_v& tree_v::print_depthorder(const t_node_t* _node) const { std::queue<const node*> q; q.push(_node); while(!q.empty()) { const node* qnode = q.front(); q.pop(); std::cout << (void*)qnode << '(' << qnode->value << ')' << ' '; for (const node* c : qnode->child) q.push(c); } return *(tree*)this; }
template_v std::vector<valuetype*> tree_v::preorder()   const { if (topnode != NULL) return preorder(topnode);   else return {}; }
template_v std::vector<valuetype*> tree_v::postorder()  const { if (topnode != NULL) return postorder(topnode);  else return {}; }
template_v std::vector<valuetype*> tree_v::depthorder() const { if (topnode != NULL) return depthorder(topnode); else return {}; }
template_v std::vector<valuetype*> tree_v::preorder(const t_node_t* _node) const { std::vector<valuetype*> result; result.push_back((valuetype*)&(_node->value)); for (const node* c : _node->child) { auto clist = preorder(c); result.insert(result.end(), clist.begin(), clist.end()); } return result; }
template_v std::vector<valuetype*> tree_v::postorder(const t_node_t* _node) const { std::vector<valuetype*> result; for (const node* c : _node->child) { auto clist = postorder(c); result.insert(result.end(), clist.begin(), clist.end()); } result.push_back((valuetype*)&(_node->value)); return result; }
template_v std::vector<valuetype*> tree_v::depthorder(const t_node_t* _node) const { std::vector<valuetype*> result; std::queue<const node*> q; q.push(_node); while(!q.empty()) { const node* qnode = q.front(); q.pop(); result.push_back((valuetype*)qnode); for (const node* c : qnode->child) q.push(c); } return result; }

#undef tree_v
#undef t_node
#undef t_node_t
#undef t_error
#undef t_error_t
#undef t_iterator
#undef t_iterator_t
#undef template_v
#endif


std::vector<std::string> split(const std::string &str, const char &delim) {
    std::vector<std::string> result;
    std::string parsestr;
    size_t prevdelim = 0;
    for (size_t i = 0; i < str.size(); i++) if (str[i] == delim) {
        parsestr = str.substr(prevdelim, i-prevdelim);
        if (parsestr.size() != 0) result.push_back(parsestr);
        prevdelim = i + 1;
    }
    parsestr = str.substr(prevdelim);
    if (parsestr.size() != 0) result.push_back(parsestr);
    return result;
}

using namespace std;
int main() {
    cout << "== Beginning of the program ==\n";
    tree<int> mytree;
    auto cursor = mytree.insert_top(1234);
    cursor.print();
    string console;
    vector<string> args;
    while (cout << "Your prompt > ", getline(cin, console), (console != "exit") && (console != "quit")) {
        if (console == "") continue;
        args = split(console, ' ');
        try {
            if (args[0] == "help")
                cout << "   value [value]\n   go top\n   go parent\n   go child index [index]\n   go child value [value]\n   insert top [value]\n   insert child [value]\n   erase\n   erase child index [index]\n   erase child value [value]\n   print\n   print preorder\n   print postorder\n   print depthorder\n   vector preorder\n   vector postorder\n   vector depthorder\n   exit / quit\n";
            else if (args[0] == "value")
                *cursor = stoi(args[1]), cursor.print();
            else if (args[0] == "go") {
                if (args[1] == "top") cursor = mytree.top(), cursor.print();
                else if (args[1] == "parent") cursor.go_parent(), cursor.print();
                else if (args[1] == "child") {
                    if (args[2] == "index") cursor.go_child_index(stoi(args[3])), cursor.print();
                    else if (args[2] == "value") cursor.go_child_value(stoi(args[3])), cursor.print();
                    else throw std::runtime_error("Type prompt like \"go child index [index]\" or \"go child value [value]\".");
                }
            } else if (args[0] == "insert") {
                if (args[1] == "top") {
                    if (mytree.empty()) cursor = mytree.insert_top(stoi(args[2]));
                    else mytree.insert_top(stoi(args[2]));
                } else if (args[1] == "child") cursor.insert_child(stoi(args[2]));
                cursor.print();
            } else if (args[0] == "erase") {
                if (args.size() == 1) cursor.erase();
                else if (args[1] == "child") {
                    if (args[2] == "index") cursor.erase_child_index(stoi(args[3]));
                    else if (args[2] == "value") cursor.erase_child_value(stoi(args[3]));
                }
                cursor.print();
            } else if (args[0] == "print") {
                if (args.size() == 1) cursor.print();
                else if (args[1] == "preorder") mytree.print_preorder();
                else if (args[1] == "postorder") mytree.print_postorder();
                else if (args[1] == "depthorder") mytree.print_depthorder();
            } else if (args[0] == "vector") {
                vector<int*> vec;
                if (args[1] == "preorder") vec = mytree.preorder();
                else if (args[1] == "postorder") vec = mytree.postorder();
                else if (args[1] == "depthorder") vec = mytree.depthorder();
                for (auto &v : vec) cout << *v << ' ';
                cout << '\n';
            }
        } catch (const std::exception& e) {
            cout << "Error occurred! \"" << (strcmp(e.what(), "basic_string") == 0 ? "Syntax error of the prompt" : e.what()) << "\"\n";
        }
    }
    cout << "== End of the program ==\n";
}
