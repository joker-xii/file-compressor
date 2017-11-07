//
// Created by joker on 2017/11/7.
//

#ifndef DATA_STRUCTURE_EXP_BINTREE_NODE_H
#define DATA_STRUCTURE_EXP_BINTREE_NODE_H

#include <functional>
#include <cstdlib>
#include "queue.h"


template <class T,class Status>
struct bintree_node {
    using bnode_ptr=bintree_node *;
    using const_bnode_ptr=const bintree_node const*;
    using visit_func=std::function<void(T &)> &;
    using const_visit_func=std::function<void(const T &)> &;
    T val;
    bintree_node *parent, *lc, *rc;
    int node_height;
    int npl;
    Status status;

    bintree_node()
            : parent(nullptr), lc(nullptr), rc(nullptr),
              node_height(0), npl(1), status() {}

    bintree_node(const T &val, bnode_ptr parent = nullptr,
                 bnode_ptr lchild = nullptr, bnode_ptr rchild = nullptr,
                 int height = 0, int null_path_length = 1, Status status)
            : val(val), parent(parent), lc(lchild), rc(rchild), node_height(height),
              npl(null_path_length), Status(status) {}

    static void move_to_lc(bnode_ptr &ptr) {
        ptr = ptr->lc;
    }

    static void move_to_rc(bnode_ptr &ptr) {
        ptr = ptr->rc;
    }

    static void move_to_parent(bnode_ptr &ptr) {
        ptr = ptr->parent;
    }

    bool has_parent() const {
        return (bool) parent;
    }

    bool is_root()const {
        return !has_parent();
    }

    bool is_lchild() const{
        return !is_root() && (this == parent->lc);
    }

    bool is_rchild() const{
        return !is_root() && (this == parent->rc);
    }

    bool has_lchild() const{
        return (bool) lc;
    }

    bool has_rchild() const{
        return (bool) rc;
    }

    bool has_child() const{
        return has_lchild() || has_rchild();
    }

    bool has_all_children() const{
        return has_rchild() && has_rchild();
    }

    bool is_leaf() const{
        return !has_child();
    }

    bnode_ptr sibling() {
        return is_lchild() ? parent->rc : parent->lc;
    }

    bnode_ptr uncle() {
        return parent->is_lchild() ?
               parent->parent->rc :
               parent->parent->lc;
    }

    bnode_ptr &&from_parent_to() {
        return is_root() ? this : is_lchild() ? parent->lc : parent->rc;
    }

    int stature() {
        return node_height ? node_height : -1;
    }

    int size() {
        int size_now = 1;
        if (lc)size_now += lc->size();
        if (rc)size_now += rc->size();
        return size_now;
    }

    bnode_ptr next() {
        bnode_ptr s = this;
        if (rc) {
            s = rc;
            while (s->has_lchild())move_to_lc(s);
        } else {
            while (s->is_rchild())move_to_parent(s);
            move_to_parent(s);
        }
        return s;
    }
    const_bnode_ptr next()const{
        return (const_cast<bnode_ptr >(this))->next();
    }

    bnode_ptr insert_as_lchild(T const &val) {
        return lc = new bintree_node(val, this);
    }

    bnode_ptr insert_as_rchild(T const &val) {
        return rc = new bintree_node(val, this);
    }

    void trav_in_order(visit_func visit) {
        if (!this)return;
        lc->trav_in_order(visit);
        visit(val);
        rc->trav_in_order(visit);
    }

    void trav_in_order(const_visit_func visit) const {
        if (!this)return;
        lc->trav_in_order(visit);
        visit(val);
        rc->trav_in_order(visit);
    }

    void trav_pre_order(visit_func visit) {
        if (!this)return;
        visit(val);
        lc->trav_pre_order(visit);
        rc->trav_pre_order(visit);
    }

    void trav_pre_order(const_visit_func visit) const {
        if (!this)return;
        visit(val);
        lc->trav_pre_order(visit);
        rc->trav_pre_order(visit);
    }

    void trav_post_order(visit_func visit) {
        if (!this)return;
        lc->trav_post_order(visit);
        rc->trav_post_order(visit);
        visit(val);
    }

    void trav_post_order(const_visit_func visit) const {
        if (!this)return;
        lc->trav_post_order(visit);
        rc->trav_post_order(visit);
        visit(val);
    }

    void trav_level(visit_func visit){
        queue<bnode_ptr> Q;
        Q.push(this);
        while (!Q.empty()){
            bnode_ptr x=Q.front();
            Q.pop();visit(x->val);
            if(x->has_lchild())Q.push(x->lc);
            if(x->has_rchild())Q.push(x->rc);
        }
    }

    void trav_level(const_visit_func visit)const{
        queue<const_bnode_ptr> Q;
        Q.push(this);
        while (!Q.empty()){
            const_bnode_ptr x=Q.front();
            Q.pop();visit(x->val);
            if(x->has_lchild())Q.push(x->lc);
            if(x->has_rchild())Q.push(x->rc);
        }
    }

    bool operator<(bintree_node const &bn) const {
        return val < bn.val;
    }

    bool operator==(bintree_node const &bn) const {
        return val == bn.val;
    }
};

template <class T,class S>
class bintree_iterator{
protected:
    using bnode_ptr= typename
    bintree_node<T,S>::bnode_ptr;
    using const_bnode_ptr= typename
    bintree_node<T,S>::const_bnode_ptr;
    union {
        bnode_ptr ptr;
        const_bnode_ptr const_ptr;
    }node;
public:
    explicit bintree_iterator(bnode_ptr node= nullptr){
        this->node.ptr= node;
    }
    explicit bintree_iterator(const_bnode_ptr node= nullptr){
        this->node.const_ptr= node;
    }

     bintree_iterator&operator++(){
        node.ptr=node.ptr->next();
        return *this;
    }

     bintree_iterator operator++(int){
        auto ret=*this;
        (*this)++;
        return ret;
    }

    virtual T &operator*(){
        return node.ptr->val;
    }
    virtual T*operator->(){
        return &node.ptr->val;
    }
};

template <class T,class S>
class const_bintree_iterator:public bintree_iterator<T,S> {
public:
    explicit const_bintree_iterator(const_bnode_ptr node = nullptr)
            : bintree_iterator<T, S>(node) {}

    const_bintree_iterator &operator++() {
        node.const_ptr = node.const_ptr->next();
        return *this;
    }

    const_bintree_iterator operator++(int) {
        auto ret = *this;
        (*this)++;
        return ret;
    }

    T &operator*() {
        return node.const_ptr->val;
    }

    T *operator->() {
        return &node.const_ptr->val;
    }
};

struct void_status{};

#endif //DATA_STRUCTURE_EXP_BINTREE_NODE_H