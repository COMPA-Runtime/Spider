/**
 * Copyright or © or Copr. IETR/INSA - Rennes (2014 - 2019) :
 *
 * Antoine Morvan <antoine.morvan@insa-rennes.fr> (2018 - 2019)
 * Florian Arrestier <florian.arrestier@insa-rennes.fr> (2018 - 2019)
 * Hugo Miomandre <hugo.miomandre@insa-rennes.fr> (2017)
 * Julien Heulot <julien.heulot@insa-rennes.fr> (2014 - 2015)
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
#ifndef SRDAG_VERTEX_H
#define SRDAG_VERTEX_H

#include <tools/SetElement.h>

#include <graphs/PiSDF/PiSDFVertex.h>
#include <graphs/SRDAG/SRDAGCommon.h>
#include <graphs/SRDAG/SRDAGEdge.h>
#include <scheduling/SRDAGScheduleJob.h>

#include <platform.h>


class SRDAGVertex : public SetElement {
public:
    friend class SRDAGEdge;

    SRDAGVertex(
            int globalId,
            SRDAGType type, SRDAGGraph *graph,
            PiSDFVertex *reference,
            int refId, int iterId,
            int nInEdge, int nOutEdge,
            int nInParam, int nOutParam);

    ~SRDAGVertex() override;

    /** Parameters getters */
    inline int getNInParam() const;

    inline int getNOutParam() const;

    inline Param getInParam(int ix) const;

    inline Param *getOutParam(int ix);

    inline const Param *getInParams() const;

    inline const Param *const *getOutParams() const;

    /** Data edge getters */
    inline int getNInEdge() const;

    inline int getNOutEdge() const;

    inline int getNConnectedInEdge() const;

    inline int getNConnectedOutEdge() const;

    inline SRDAGEdge *getInEdge(int ix);

    inline SRDAGEdge *getOutEdge(int ix);

    inline SRDAGEdge *const *getInEdges();

    inline SRDAGEdge *const *getOutEdges();

    /** Add Param Fcts */
    inline void addInParam(int ix, Param param);

    inline void addOutParam(int ix, Param *param);

    /** General getters */
    inline int getId() const;

    inline int getFctId() const;

    inline SRDAGType getType() const;

    inline SRDAGState getState() const;

    inline SRDAGGraph *getGraph() const;

    inline PiSDFGraph *getSubGraph() const;

    inline PiSDFVertex *getReference() const;

    inline bool isHierarchical() const;

    inline Time getStartTime() const;

    inline Time getEndTime() const;

    inline int getSlave() const;

    inline int getSlaveJobIx() const;

    inline int getRefId() const;

    inline int getIterId() const;

    inline void setState(SRDAGState state);

    void updateState();

    inline void setStartTime(Time start);

    inline void setEndTime(Time end);

    inline void setSlave(int slave);

    inline void setSlaveJobIx(int slaveJobIx);

    /** Comparison fcts */
    inline bool isEqual(SRDAGVertex *v2);

    inline bool match(SRDAGVertex *v2);

    void toString(char *name, int sizeMax) const;

    const char *toString();


    /** Constraints/Timings Fcts */
    inline bool isExecutableOn(int pe) const;

    inline Time executionTimeOn(int peType) const;

    inline int getSchedLvl() const;

    void setSchedLvl(int schedLvl);

    inline SRDAGScheduleJob *getScheduleJob() {
        return scheduleJob_;
    }

protected:
    /** Connect Fcts */
    inline void connectInEdge(SRDAGEdge *edge, int ix);

    inline void connectOutEdge(SRDAGEdge *edge, int ix);

    inline void disconnectInEdge(int ix);

    inline void disconnectOutEdge(int ix);

private:
    //static int globalId;

    int id_;
    SRDAGType type_;
    SRDAGState state_;
    SRDAGGraph *graph_;
    PiSDFVertex *reference_;
    int refId_;
    int iterId_;

    int nMaxInEdge_, nMaxOutEdge_;
    int nCurInEdge_, nCurOutEdge_;
    SRDAGEdge **inEdges_, **outEdges_;

    int nInParam_, nOutParam_;
    Param *inParams_;
    Param **outParams_;

    Time start_, end_;
    int schedLvl_;

    int slaveJobIx_;

    int slave_;

    SRDAGScheduleJob *scheduleJob_;
};

/** Inlines Fcts */
/** Parameters getters */
inline int SRDAGVertex::getNInParam() const {
    return nInParam_;
}

inline int SRDAGVertex::getNOutParam() const {
    return nOutParam_;
}

inline Param SRDAGVertex::getInParam(int ix) const {
    if (ix < nInParam_ && ix >= 0) {
        return inParams_[ix];
    }
    throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nInParam_);
}

inline Param *SRDAGVertex::getOutParam(int ix) {
    if (ix < nOutParam_ && ix >= 0) {
        return outParams_[ix];
    }
    throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nOutParam_);
}

inline const Param *SRDAGVertex::getInParams() const {
    return inParams_;
}

inline const Param *const *SRDAGVertex::getOutParams() const {
    return outParams_;
}

/** Data edge getters */
inline int SRDAGVertex::getNInEdge() const {
    return nMaxInEdge_;
}

inline int SRDAGVertex::getNOutEdge() const {
    return nMaxOutEdge_;
}

inline int SRDAGVertex::getNConnectedInEdge() const {
    return nCurInEdge_;
}

inline int SRDAGVertex::getNConnectedOutEdge() const {
    return nCurOutEdge_;
}

inline SRDAGEdge *SRDAGVertex::getInEdge(int ix) {
    if (ix < nMaxInEdge_ && ix >= 0) {
        return inEdges_[ix];
    }
    throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nMaxInEdge_);
}

inline SRDAGEdge *SRDAGVertex::getOutEdge(int ix) {
    if (ix < nMaxOutEdge_ && ix >= 0) {
        return outEdges_[ix];
    }
    throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nMaxOutEdge_);
}

inline SRDAGEdge *const *SRDAGVertex::getInEdges() {
    return inEdges_;
}

inline SRDAGEdge *const *SRDAGVertex::getOutEdges() {
    return outEdges_;
}


/** Connect Fcts */
inline void SRDAGVertex::connectInEdge(SRDAGEdge *edge, int ix) {
    if (ix >= nMaxInEdge_ || ix < 0) {
        throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nMaxInEdge_);
    } else if (inEdges_[ix] != nullptr) {
        PiSDFVertex *reference = this->getReference();
        if (reference) {
            throwSpiderException("Vertex [%s] --> Trying to overwrite already connected input edge.",
                                 reference->getName());
        } else {
            throwSpiderException("Trying to overwrite already connected input edge.");
        }
    } else {
        inEdges_[ix] = edge;
        nCurInEdge_++;
    }
}

inline void SRDAGVertex::connectOutEdge(SRDAGEdge *edge, int ix) {
    if (ix >= nMaxOutEdge_ || ix < 0) {
        throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nMaxOutEdge_);
    } else if (outEdges_[ix] != nullptr) {
        PiSDFVertex *reference = this->getReference();
        if (reference) {
            throwSpiderException("Vertex [%s] --> Trying to overwrite already connected output edge.",
                                 reference->getName());
        } else {
            throwSpiderException("Trying to overwrite already connected output edge.");
        }
    } else {
        outEdges_[ix] = edge;
        nCurOutEdge_++;
    }
}

inline void SRDAGVertex::disconnectInEdge(int ix) {
    if (ix >= nMaxInEdge_ || ix < 0) {
        throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nMaxInEdge_);
    } else if (!inEdges_[ix]) {
        throwSpiderException("Trying to disconnect NULL input edge.");
    } else {
        inEdges_[ix] = nullptr;
        nCurInEdge_--;
    }
}

inline void SRDAGVertex::disconnectOutEdge(int ix) {
    if (ix >= nMaxOutEdge_ || ix < 0) {
        throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nMaxOutEdge_);
    } else if (outEdges_[ix] == nullptr) {
        throwSpiderException("Trying to disconnect NULL output edge.");
    } else {
        outEdges_[ix] = nullptr;
        nCurOutEdge_--;
    }
}

/** Add Param Fcts */
inline void SRDAGVertex::addInParam(int ix, Param param) {
    if (ix >= nInParam_ || ix < 0) {
        throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nInParam_);
    } else if (inParams_[ix] != 0) {
        throwSpiderException("Trying to erase already connected input param.");
    } else {
        inParams_[ix] = param;
    }
}

inline void SRDAGVertex::addOutParam(int ix, Param *param) {
    if (ix >= nOutParam_ || ix < 0) {
        throwSpiderException("Bad index. Value: %d -- Max: %d", ix, nOutParam_);
    } else if (outParams_[ix] != nullptr) {
        throwSpiderException("Trying to erase already connected output param.");
    } else {
        outParams_[ix] = param;
    }
}

/** General getters */
inline int SRDAGVertex::getId() const {
    return id_;
}

inline int SRDAGVertex::getFctId() const {
    switch (type_) {
        case SRDAG_NORMAL:
            return reference_->getFctId();
        case SRDAG_BROADCAST:
            return BROADCAST_F_IX;
        case SRDAG_JOIN:
            return JOIN_F_IX;
        case SRDAG_FORK:
            return FORK_F_IX;
        case SRDAG_ROUNDBUFFER:
            return ROUNDBUFFER_F_IX;
        case SRDAG_INIT:
            return INIT_F_IX;
        case SRDAG_END:
            return END_F_IX;
        default:
            throwSpiderException("Unhandled case.");
    }
}

inline SRDAGType SRDAGVertex::getType() const {
    return type_;
}

inline SRDAGState SRDAGVertex::getState() const {
    return state_;
}

inline SRDAGGraph *SRDAGVertex::getGraph() const {
    return graph_;
}

inline PiSDFGraph *SRDAGVertex::getSubGraph() const {
    if (type_ == SRDAG_NORMAL) {
        return reference_->getSubGraph();
    }
    return nullptr;
}

inline PiSDFVertex *SRDAGVertex::getReference() const {
    return reference_;
}

inline bool SRDAGVertex::isHierarchical() const {
    return type_ == SRDAG_NORMAL && reference_->isHierarchical();
}

inline Time SRDAGVertex::getStartTime() const {
    return start_;
}

inline Time SRDAGVertex::getEndTime() const {
    return end_;
}

inline int SRDAGVertex::getSlave() const {
    return slave_;
}

inline int SRDAGVertex::getRefId() const {
    return refId_;
}

inline int SRDAGVertex::getIterId() const {
    return iterId_;
}

inline void SRDAGVertex::setState(SRDAGState state) {
    state_ = state;
}

inline void SRDAGVertex::setStartTime(Time start) {
    start_ = start;
}

inline void SRDAGVertex::setEndTime(Time end) {
    end_ = end;
}

inline void SRDAGVertex::setSlave(int slave) {
    slave_ = slave;
}

inline void SRDAGVertex::setSlaveJobIx(int slaveJobIx) {
    slaveJobIx_ = slaveJobIx;
}

inline int SRDAGVertex::getSlaveJobIx() const {
    return slaveJobIx_;
}

inline bool SRDAGVertex::isEqual(SRDAGVertex *v2) {
    bool equal = true;
    equal = equal && (this->type_ == v2->type_);
    equal = equal && (this->getFctId() == v2->getFctId());

//	equal = equal && (this->getNInEdge() == v2->getNInEdge());
    equal = equal && (this->getNConnectedInEdge() == v2->getNConnectedInEdge());
//	equal = equal && (this->getNOutEdge() == v2->getNOutEdge());
    equal = equal && (this->getNConnectedOutEdge() == v2->getNConnectedOutEdge());

    equal = equal && (this->nInParam_ == v2->nInParam_);
    for (int i = 0; i < this->nInParam_; i++)
        equal = equal && (this->inParams_[i] == v2->inParams_[i]);

    return equal;
}

inline bool SRDAGVertex::match(SRDAGVertex *v2) {
    bool match = this->isEqual(v2);

    for (int i = 0; i < this->getNConnectedInEdge(); i++)
        match = match && this->getInEdge(i)->getSrc()->isEqual(v2->getInEdge(i)->getSrc())
                && this->getInEdge(i)->getRate() == v2->getInEdge(i)->getRate()
                && (this->getInEdge(i)->getSrcPortIx() == v2->getInEdge(i)->getSrcPortIx()
                    || this->getInEdge(i)->getSrc()->getType() == SRDAG_BROADCAST);

    for (int i = 0; i < this->getNConnectedOutEdge(); i++)
        match = match && this->getOutEdge(i)->getSnk()->isEqual(v2->getOutEdge(i)->getSnk())
                && this->getOutEdge(i)->getRate() == v2->getOutEdge(i)->getRate()
                && (this->getOutEdge(i)->getSnkPortIx() == v2->getOutEdge(i)->getSnkPortIx()
                    || this->getOutEdge(i)->getSnk()->getType() == SRDAG_BROADCAST);

    return match;
}

inline bool SRDAGVertex::isExecutableOn(int pe) const {
    switch (type_) {
        case SRDAG_NORMAL:
            return reference_->canExecuteOn(pe);
        case SRDAG_BROADCAST:
        case SRDAG_JOIN:
        case SRDAG_FORK:
        case SRDAG_ROUNDBUFFER:
        case SRDAG_INIT:
        case SRDAG_END:
            return true;
        default:
            throwSpiderException("Unhandled case.");
    }
}

inline Time SRDAGVertex::executionTimeOn(int peType) const {
    switch (type_) {
        case SRDAG_NORMAL:
            return reference_->getTimingOnType(peType, inParams_, nInParam_);
        case SRDAG_BROADCAST:
        case SRDAG_JOIN:
        case SRDAG_FORK:
        case SRDAG_ROUNDBUFFER:
        case SRDAG_INIT:
        case SRDAG_END:
            // TODO: update execution time based on model and measure values
//            if (reference_) {
//                return 50;
//            }
            return 50;
        default:
            throwSpiderException("Unhandled case.");
    }
}

inline int SRDAGVertex::getSchedLvl() const {
    return schedLvl_;
}

inline void SRDAGVertex::setSchedLvl(int schedLvl) {
    schedLvl_ = schedLvl;
}

#endif/*SRDAG_VERTEX_H*/
