#include "token.h"
#include <vector>

struct Node
{
    Token* value;
    Node *left, *right;
    unsigned int height;
};

class SyntaxTree
{
public: 
    SyntaxTree();
    void buildFromPostfix(std::vector<Token*>);
    void balanceTree();
    std::vector<Token*> postfix();
private:
    void _postfix(Node*, std::vector<Token*>&);
    Node* _balance_tree(Node*);
    void _from_left_to_right_rebalancing(Node*, Node*, Node*, Node*, OperatorType);
    void _from_right_to_left_rebalancing(Node*, Node*, Node*, Node*, OperatorType);
    Node* root;
};

unsigned int height(Node*);
void update_node_height(Node*);
int get_balance(Node*);