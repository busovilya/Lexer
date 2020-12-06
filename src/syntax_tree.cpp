#include "syntax_tree.h"
#include "token_factories.h"
#include "utils.h"

#include <math.h>
#include <stack>

unsigned int height(Node *node)
{
    if (node == nullptr)
        return 0;
    return node->height;
}

void update_node_height(Node *node)
{
    node->height = std::max(height(node->left), height(node->right)) + 1;
}

int get_balance(Node *node)
{
    return height(node->left) - height(node->right);
}

SyntaxTree::SyntaxTree()
{
    root = nullptr;
}

std::vector<Token *> SyntaxTree::postfix()
{
    std::vector<Token *> result;

    _postfix(root, result);

    return result;
}

std::vector<Token *> SyntaxTree::infix()
{
    std::vector<Token *> result;

    _infix(root, result);

    return result;
}

void SyntaxTree::_postfix(Node *node, std::vector<Token *> &result)
{
    if (node->left != nullptr)
        _postfix(node->left, result);
    if (node->right != nullptr)
        _postfix(node->right, result);
    result.push_back(node->value);
}

void SyntaxTree::_infix(Node *node, std::vector<Token *> &result)
{
    if (node->left != nullptr)
    {
        bool parenthesis = false;
        if (node->value->type == TokenTypes::Operator)
            if (node->left->value->type == TokenTypes::Operator)
                if (((OperatorToken *)node->value)->operator_priority > ((OperatorToken *)node->left->value)->operator_priority)
                {
                    auto left_bracket = createBracket(BracketType::LeftBracket);
                    result.push_back(left_bracket);
                    parenthesis = true;
                }
        _infix(node->left, result);
        if (parenthesis)
        {
            auto right_bracket = createBracket(BracketType::RightBraket);
            result.push_back(right_bracket);
            parenthesis = true;
        }
    }
    result.push_back(node->value);
    if (node->right != nullptr)
    {
        bool parenthesis = false;
        if (node->value->type == TokenTypes::Operator)
            if (node->right->value->type == TokenTypes::Operator)
                if (((OperatorToken *)node->value)->operator_priority > ((OperatorToken *)node->right->value)->operator_priority ||
                    ((OperatorToken *)node->value)->op_type == OperatorType::Minus &&
                        ((OperatorToken *)node->value)->operator_priority >= ((OperatorToken *)node->right->value)->operator_priority ||
                    ((OperatorToken *)node->value)->op_type == OperatorType::Division)
                {
                    auto left_bracket = createBracket(BracketType::LeftBracket);
                    result.push_back(left_bracket);
                    parenthesis = true;
                }
        _infix(node->right, result);
        if (parenthesis)
        {
            auto right_bracket = createBracket(BracketType::RightBraket);
            result.push_back(right_bracket);
            parenthesis = true;
        }
    }
}

void SyntaxTree::buildFromPostfix(std::vector<Token *> tokens)
{
    Node *node;
    std::stack<Node *> s;

    for (Token *token : tokens)
    {
        node = new Node();
        node->value = token;

        if (token->type == TokenTypes::Operator)
        {
            Node *node1 = s.top();
            s.pop();
            Node *node2 = s.top();
            s.pop();
            node->left = node2;
            node->right = node1;
            update_node_height(node);
            s.push(node);
        }
        else
        {
            node->height = 1;
            s.push(node);
        }
    }
    root = node;
}

void SyntaxTree::balanceTree()
{
    root = _balance_tree(root);
}

void SyntaxTree::expandBrackets()
{
    root = _expand_brackets(root);
}

Node *SyntaxTree::_balance_tree(Node *node)
{
    int balance = get_balance(node);

    if (balance > 1)
    {
        OperatorToken *node_operator_token = (OperatorToken *)node->value;
        OperatorToken *left_operator_token = (OperatorToken *)node->left->value;
        OperatorToken *right_operator_token = (OperatorToken *)node->right->value;
        if (node_operator_token->op_type == OperatorType::Plus)
        {
            // (A + B) + C => A + (B + C)
            if (node_operator_token->op_type == OperatorType::Plus && left_operator_token->op_type == OperatorType::Plus)
            {
                _from_left_to_right_rebalancing(node, node->left->left, node->left->right, node->right, OperatorType::Plus);
            }
            // (A - B) + C => A - (B - C)
            else if (node_operator_token->op_type == OperatorType::Plus && left_operator_token->op_type == OperatorType::Minus)
            {
                _from_left_to_right_rebalancing(node, node->left->left, node->left->right, node->right, OperatorType::Minus);
                ((OperatorToken *)node->value)->op_type == OperatorType::Minus;
            }
        }
        else if (node_operator_token->op_type == OperatorType::Minus)
        {
            // (A + B) - C => B - (C - A)
            if (node_operator_token->op_type == OperatorType::Minus && left_operator_token->op_type == OperatorType::Plus)
            {
                _from_left_to_right_rebalancing(node, node->left->right, node->right, node->left->left, OperatorType::Minus);
            }
            // (A - B) - C => A - (B + C)
            else if (node_operator_token->op_type == OperatorType::Minus && left_operator_token->op_type == OperatorType::Minus)
            {
                _from_left_to_right_rebalancing(node, node->left->left, node->left->right, node->right, OperatorType::Plus);
            }
        }
        else if (node_operator_token->op_type == OperatorType::Multiplication)
        {
            // ( A * B ) * C => A * ( B * C )
            if (left_operator_token->op_type == OperatorType::Multiplication)
            {
                _from_left_to_right_rebalancing(node, node->left->left, node->left->right, node->right, OperatorType::Multiplication);
            }
            // ( A / B ) * C =>  A / ( B / C )
            else if (left_operator_token->op_type == OperatorType::Division)
            {
                _from_left_to_right_rebalancing(node, node->left->left, node->left->right, node->right, OperatorType::Division);
                ((OperatorToken *)node->value)->op_type == OperatorType::Division;
            }
        }
        else if (node_operator_token->op_type == OperatorType::Division)
        {
            // ( A * B ) / C => A / ( C / B )
            if (left_operator_token->op_type == OperatorType::Multiplication)
            {
                _from_left_to_right_rebalancing(node, node->left->left, node->right, node->left->right, OperatorType::Division);
            }
            // ( A / B ) / C => A / ( B * C )
            else if (left_operator_token->op_type == OperatorType::Division)
            {
                _from_left_to_right_rebalancing(node, node->left->left, node->left->right, node->right, OperatorType::Multiplication);
            }
        }
        update_node_height(node);
        // if (std::abs(get_balance(node->left)) <- 1 && std::abs(get_balance(node->right)) <= 1)
        if (std::abs(get_balance(node)) == 2 && node_operator_token->operator_priority == left_operator_token->operator_priority)
            node = _balance_tree(node);
    }
    else if (balance < -1)
    {
        OperatorToken *node_operator_token = (OperatorToken *)node->value;
        OperatorToken *left_operator_token = (OperatorToken *)node->left->value;
        OperatorToken *right_operator_token = (OperatorToken *)node->right->value;
        if (node_operator_token->op_type == OperatorType::Plus || node_operator_token->op_type == OperatorType::Multiplication)
        {
            Node *tmp = node->left;
            node->left = node->right;
            node->right = tmp;
            _balance_tree(node);
        }
        else if (node_operator_token->op_type == OperatorType::Minus)
        {
            // A - (B + C) => (A - B) - C
            if (right_operator_token->op_type == OperatorType::Plus)
            {
                _from_right_to_left_rebalancing(node, node->left, node->right->left, node->right->right, OperatorType::Minus);
            }
            // A - (B - C) => (A - B) + C
            else if (right_operator_token->op_type == OperatorType::Minus)
            {
                _from_right_to_left_rebalancing(node, node->left, node->right->left, node->right->right, OperatorType::Minus);
                delete node->value;
                node->value = createOperator(OperatorType::Plus);
            }
        }
        else if (node_operator_token->op_type == OperatorType::Division)
        {
            // A / ( B / C ) => ( A * C ) / B
            if (right_operator_token->op_type == OperatorType::Division)
            {
                _from_right_to_left_rebalancing(node, node->left, node->right->right, node->right->left, OperatorType::Multiplication);
            }
            // A / ( B * C ) => (A / B) / C
            else if (right_operator_token->op_type == OperatorType::Multiplication)
            {
                _from_right_to_left_rebalancing(node, node->left, node->right->left, node->right->right, OperatorType::Division);
            }
        }
        update_node_height(node);
        // if (std::abs(get_balance(node->left)) <= 1 && std::abs(get_balance(node->right)) <= 1)
        if (std::abs(get_balance(node)) == 2 && node_operator_token->operator_priority == left_operator_token->operator_priority)
            node = _balance_tree(node);
    }

    if (node->left != nullptr)
        node->left = _balance_tree(node->left);
    if (node->right != nullptr)
        node->right = _balance_tree(node->right);

    return node;
}

void SyntaxTree::_from_left_to_right_rebalancing(Node *node, Node *new_left, Node *new_right_left, Node *new_right_right, OperatorType new_right_op_type)
{
    Node *left = node->left;
    node->left = new_left;
    Node *new_node = new Node();
    OperatorToken *op_token = (OperatorToken *)left->value;
    op_token->op_type = new_right_op_type;
    new_node->value = left->value;
    new_node->left = new_right_left;
    new_node->right = new_right_right;
    update_node_height(new_node);
    if (new_right_left != node->right && new_right_right != node->right)
        delete node->right;
    delete left;
    node->right = new_node;
}

void SyntaxTree::_from_right_to_left_rebalancing(Node *node, Node *new_left_left, Node *new_left_right, Node *new_right, OperatorType new_left_op_type)
{
    Node *right = node->right;
    node->right = new_right;
    Node *new_node = new Node();
    delete right->value;
    right->value = createOperator(new_left_op_type);
    new_node->value = right->value;
    new_node->left = new_left_left;
    new_node->right = new_left_right;
    update_node_height(new_node);
    if (new_left_left != node->left && new_left_right != node->left)
        delete node->left;
    delete right;
    node->left = new_node;
}

Node *SyntaxTree::_expand_brackets(Node *node)
{
    if (node->value->type == TokenTypes::Operator)
    {
        OperatorToken *node_operator_token = (OperatorToken *)node->value;

        if (node_operator_token->op_type == OperatorType::Multiplication)
        {
            if (node->right->value->type == TokenTypes::Operator)
            {
                OperatorToken *op_token = (OperatorToken *)node->right->value;
                if (op_token->operator_priority < node_operator_token->operator_priority)
                {
                    Node *new_left = new Node();
                    OperatorToken *new_token = createOperator(OperatorType::Multiplication);
                    new_left->value = new_token;
                    new_left->left = node->left;
                    new_left->right = node->right->left;
                    node->right->left = copySubTree(node->left);
                    node->left = new_left;
                    delete node->value;
                    node->value = createOperator(op_token->op_type);
                    delete node->right->value;
                    node->right->value = createOperator(OperatorType::Multiplication);
                    printTokens(this->infix());
                    _expand_brackets(node);
                }
            }
            if (node->left->value->type == TokenTypes::Operator)
            {
                OperatorToken *op_token = (OperatorToken *)node->left->value;
                if (op_token->operator_priority < node_operator_token->operator_priority)
                {
                    Node *old_right = node->right;
                    Node *new_right = new Node();
                    OperatorToken *new_token = createOperator(OperatorType::Multiplication);
                    new_right->value = new_token;
                    new_right->right = node->right;
                    new_right->left = node->left->right;
                    node->left->right = copySubTree(node->right);
                    node->right = new_right;
                    delete node->value;
                    node->value = createOperator(op_token->op_type);
                    delete node->left->value;
                    node->left->value = createOperator(OperatorType::Multiplication);
                    printTokens(this->infix());
                    _expand_brackets(node);
                }
            }
        }
        else if (node_operator_token->op_type == OperatorType::Division)
        {
            if (node->right->value->type == TokenTypes::Operator)
            {
                OperatorToken *op_token = (OperatorToken *)node->right->value;

                if (op_token->op_type == OperatorType::Division)
                {
                    _from_right_to_left_rebalancing(node, node->left, node->right->right, node->right->left, OperatorType::Multiplication);
                    printTokens(this->infix());
                    _expand_brackets(node);
                }
                else if (op_token->op_type == OperatorType::Multiplication)
                {
                    _from_right_to_left_rebalancing(node, node->left, node->right->left, node->right->right, OperatorType::Division);
                    printTokens(this->infix());
                    _expand_brackets(node);
                }
            }
            if (node->left->value->type == TokenTypes::Operator)
            {
                OperatorToken *op_token = (OperatorToken *)node->left->value;

                if (op_token->op_type == OperatorType::Plus || op_token->op_type == OperatorType::Minus)
                {
                    Node *old_right = node->right;
                    Node *new_right = new Node();
                    OperatorToken *new_token = createOperator(OperatorType::Division);
                    new_right->value = new_token;
                    new_right->right = node->right;
                    new_right->left = node->left->right;
                    node->right = new_right;
                    node->left->right = copySubTree(old_right);
                    delete node->value;
                    node->value = createOperator(op_token->op_type);
                    delete node->left->value;
                    node->left->value = createOperator(OperatorType::Division);
                    printTokens(this->infix());
                    _expand_brackets(node);
                }
            }
        }
        else if (node_operator_token->op_type == OperatorType::Minus)
        {
            if (node->right->value->type == TokenTypes::Operator)
            {
                OperatorToken *op_token = (OperatorToken *)node->right->value;

                if (op_token->op_type == OperatorType::Minus)
                {
                    _from_right_to_left_rebalancing(node, node->left, node->right->left, node->right->right, OperatorType::Minus);
                    delete node->value;
                    node->value = createOperator(OperatorType::Plus);
                    printTokens(this->infix());
                    _expand_brackets(node);
                }
                if (op_token->op_type == OperatorType::Plus)
                {
                    _from_right_to_left_rebalancing(node, node->left, node->right->left, node->right->right, OperatorType::Minus);
                    printTokens(this->infix());
                    _expand_brackets(node);
                }
            }
        }
    }
    
    if (node->left != nullptr)
        node->left = _expand_brackets(node->left);
    if (node->right != nullptr)
        node->right = _expand_brackets(node->right);
    return node;
}

Node* SyntaxTree::copySubTree(Node* node)
{
    Node* new_node = new Node();
    if(node->value->type == TokenTypes::Operator)
        new_node->value = new OperatorToken(*(OperatorToken*)node->value);
    else if(node->value->type == TokenTypes::Id)
        new_node->value = new IdToken(*(IdToken*)node->value);
    else if(node->value->type == TokenTypes::Number)
        new_node->value = new NumberToken(*(NumberToken*)node->value);
    if(node->left)
        new_node->left = copySubTree(node->left);
    if(node->right)
        new_node->right = copySubTree(node->right);
    return new_node;
}