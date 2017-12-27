#include "task.hpp"
#include "logger.hpp"
#include "trie.hpp"
#include <string>

using mstd::logger;
using std::string;
using mstd::vector;

/*
 * Search task implementation
 */
search_task::search_task(trie *t, query &q, int i, std::string *r) : _t(t), _q(q), _index(i), _results(r) { }

void search_task::run() {

    auto *st = dynamic_cast<static_trie *>(_t);

    string result;
    if (st == nullptr) {
        // t was a trie
        result = _t->search(_q.v, _q.version);
    } else {
        // t was a static trie
        result = st->search(_q.v);
    }

    _results[_index] = std::move(result);
    delete this;
}

/*
 * Clean up task implementation
 */
clean_up_task::clean_up_task(trie *t, trie_node *branch) : _t(t), _branch(branch) { }

void clean_up_task::run() {
    _t->clean_up(_branch);
}

/*
 * Insertion task implementation
 */

insert_task::insert_task(trie *t, vector<string> &v, int version) : _t(t), _v(std::move(v)), _version(version) { }

insert_task::insert_task(insert_task &&other) noexcept {
    _t = other._t;
    _v = std::move(other._v);
    _version = other._version;
}

void insert_task::run() {
    _t->add(_v, _version);
}

/*
 * Deletion task implementation
 */

deletion_task::deletion_task(trie *t, vector<string> &v, int version) : _t(t), _v(std::move(v)), _version(version) { }

deletion_task::deletion_task(deletion_task &&other) noexcept {
    _t = other._t;
    _v = std::move(other._v);
    _version = other._version;
}

void deletion_task::run() {
    _t->delete_ngram(_v, _version);
}