/**
 * Copyright or © or Copr. IETR/INSA - Rennes (2013 - 2019) :
 *
 * Antoine Morvan <antoine.morvan@insa-rennes.fr> (2018 - 2019)
 * Clément Guy <clement.guy@insa-rennes.fr> (2014)
 * Florian Arrestier <florian.arrestier@insa-rennes.fr> (2018 - 2019)
 * Julien Heulot <julien.heulot@insa-rennes.fr> (2013 - 2018)
 * Yaset Oliva <yaset.oliva@insa-rennes.fr> (2013 - 2014)
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
#ifndef SET_H
#define SET_H

#include <monitor/StackMonitor.h>
#include <tools/SetElement.h>
#include <stdexcept>
#include <SpiderException.h>

template<typename TYPE>
class Set {
public:
    Set(int nbmax, SpiderStack stackId) {
        if (nbmax > 0) {
            array = CREATE_MUL(stackId, nbmax, TYPE);
        } else {
            array = nullptr;
        }
        nb = 0;
        nbMax = nbmax;
        stackId_ = stackId;
    }

    ~Set() {
        if (nbMax != 0)
            StackMonitor::free(stackId_, array);
    }

    inline void add(TYPE value);

    inline void del(TYPE value);

    inline bool contains(TYPE value);

    inline TYPE operator[](int ix);

    inline int size() const;

    inline int sizeMax() const;

    inline TYPE const *getArray() const;

private:
    SpiderStack stackId_;
    TYPE *array;
    int nb;
    int nbMax;

//	friend class SetIterator;
};

template<typename TYPE>
inline int Set<TYPE>::size() const {
    return nb;
}

template<typename TYPE>
inline int Set<TYPE>::sizeMax() const {
    return nbMax;
}

template<typename TYPE>
inline void Set<TYPE>::add(TYPE value) {
    if (nb >= nbMax) {
        throwSpiderException("Can not add element to set. Requested: %d -- Max: %d", nb + 1, nbMax);
    }
    ((SetElement *) value)->setSetIx(nb);
    array[nb++] = value;
}

template<typename TYPE>
inline bool Set<TYPE>::contains(TYPE value) {
    for (int i = 0; i < nb; ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

template<typename TYPE>
inline void Set<TYPE>::del(TYPE value) {
    int ix = ((SetElement *) value)->getSetIx();
    array[ix] = array[--nb];
    ((SetElement *) array[ix])->setSetIx(ix);
}

template<typename TYPE>
inline TYPE Set<TYPE>::operator[](int ix) {
    if (ix < 0 || ix >= nb) {
        throwSpiderException("operator[] got bad index: %d -- Set size: %d", ix, nb);
    } else {
        return array[ix];
    }
}

template<typename TYPE>
inline TYPE const *Set<TYPE>::getArray() const {
    return array;
}

#endif // SET_H
