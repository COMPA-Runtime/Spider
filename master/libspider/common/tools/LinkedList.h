/**
 * Copyright or © or Copr. IETR/INSA - Rennes (2019) :
 *
 * Antoine Morvan <antoine.morvan@insa-rennes.fr> (2019)
 * Florian Arrestier <florian.arrestier@insa-rennes.fr> (2019)
 *
 * Spider is a dataflow based runtime used to execute dynamic PiSDF
 * applications. The Preesm tool may be used to design PiSDF applications.
 *
 * This software is governed by the CeCILL  license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#ifndef SPIDER_LINKEDLIST_H
#define SPIDER_LINKEDLIST_H

#include <monitor/StackMonitor.h>
#include <stdexcept>
#include <SpiderException.h>

template<class T>
class LinkedList {
public:
    typedef struct Node {
        T val_;
        Node *prev_;
        Node *next_;
    } Node;

    LinkedList(SpiderStack stackId, std::int32_t sizeMax);

    ~LinkedList();

    T &operator[](int ix);

    /**
     * @brief Return first node of the list. Current is unchanged.
     * @return first node of the list
     */
    inline Node *first() const;

    /**
     * @brief Return last node of the list. Current is unchanged.
     * @return last node of the list
     */
    inline Node *last() const;

    /**
     * @brief Return current node of the list
     * @return current node of the list
     */
    inline Node *current() const;

    /**
     * @brief Set on next node of the list and return it
     * @return  next element of the list
     */
    inline Node *next();

    /**
     * @brief Set on previous node of the list and return it
     * @return  previous element of the list
     */
    inline Node *previous();

    /**
     * @brief set current as first
     */
    void setOnFirst();

    /**
     * @brief set current as last
     */
    void setOnLast();

    inline std::int32_t size() const;

    void add(T e);

    void addLast(T e);

    T &del(Node *n);

    bool contains(T item);

private:
    SpiderStack stackId_;
    Node *head_;
    Node *current_;
    Node *tail_;
    std::int32_t size_;
    std::int32_t sizeMax_;
};

template<class T>
inline LinkedList<T>::LinkedList(SpiderStack stackId, std::int32_t sizeMax) {
    stackId_ = stackId;
    size_ = 0;
    sizeMax_ = sizeMax;
    head_ = nullptr;
    current_ = nullptr;
    tail_ = nullptr;
}

template<class T>
inline LinkedList<T>::~LinkedList() {
    if (size_ > 0) {
        setOnFirst();
        auto *n = current_;
        while (n) {
            del(n);
            n = current_;
        }
    }
}

template<class T>
inline T &LinkedList<T>::operator[](int ix) {
    if (ix < 0 || ix >= size_) {
        throwSpiderException("Accesing unitialized element. Ix = %d -- Size = %d", ix, size_);
    }
    std::int32_t i = 0;
    auto *current = head_;
    while (i < ix) {
        current = current->next_;
        i++;
    }
    return current->val_;
}

template<class T>
inline typename LinkedList<T>::Node *LinkedList<T>::first() const {
    return head_;
}

template<class T>
inline typename LinkedList<T>::Node *LinkedList<T>::last() const {
    return tail_;
}

template<class T>
inline typename LinkedList<T>::Node *LinkedList<T>::current() const {
    return current_;
}

template<class T>
inline typename LinkedList<T>::Node *LinkedList<T>::next() {
    current_ = current_->next_;
    return current_;
}

template<class T>
inline typename LinkedList<T>::Node *LinkedList<T>::previous() {
    current_ = current_->prev_;
    return current_;
}

template<class T>
inline void LinkedList<T>::setOnFirst() {
    current_ = head_;
}

template<class T>
inline void LinkedList<T>::setOnLast() {
    current_ = tail_;
}

template<class T>
inline std::int32_t LinkedList<T>::size() const {
    return size_;
}

template<class T>
inline void LinkedList<T>::add(T e) {
    if (size_ >= sizeMax_) {
        throwSpiderException("Can not add element, list is full.");
    }
    auto *newNode = CREATE_NA(stackId_, Node);
    if (!head_) {
        head_ = newNode;
        tail_ = newNode;
    }
    newNode->val_ = e;
    newNode->prev_ = tail_;
    newNode->next_ = head_;
    head_->prev_ = newNode;
    tail_->next_ = newNode;
    head_ = newNode;
    size_++;
}

template<class T>
inline void LinkedList<T>::addLast(T e) {
    if (size_ >= sizeMax_) {
        throwSpiderException("Can not add element, list is full.");
    }
    auto *newNode = CREATE_NA(stackId_, Node);
    if (!head_) {
        head_ = newNode;
        tail_ = newNode;
    }
    newNode->val_ = e;
    newNode->prev_ = tail_;
    newNode->next_ = head_;
    head_->prev_ = newNode;
    tail_->next_ = newNode;
    tail_ = newNode;
    size_++;
}


template<class T>
inline bool LinkedList<T>::contains(T item) {
    auto *current = head_;
    while (current) {
        if (current->val_ == item) {
            return true;
        }
        current = current->next_;
    }
    return false;
}

template<class T>
inline T &LinkedList<T>::del(LinkedList::Node *n) {
    if (n) {
        size_--;
        if (size_ < 0) {
            throwSpiderException("Trying to remove node not in the list.");
        } else if (size_ == 0) {
            head_ = nullptr;
            tail_ = nullptr;
            current_ = nullptr;
        } else {
            auto *previousNode = n->prev_;
            auto *nextNode = n->next_;
            previousNode->next_ = nextNode;
            nextNode->prev_ = previousNode;
            if (n == current_) {
                current_ = nextNode;
            }
            if (n == head_) {
                head_ = nextNode;
            } else if (n == tail_) {
                tail_ = previousNode;
            }
        }
        auto &val = n->val_;
        StackMonitor::free(stackId_, n);
        return val;
    } else {
        throwSpiderException("Trying to remove nullptr node.");
    }
}


#endif/*LIST_H*/
