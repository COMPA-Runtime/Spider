/**
 * Copyright or © or Copr. IETR/INSA - Rennes (2014 - 2019) :
 *
 * Antoine Morvan <antoine.morvan@insa-rennes.fr> (2018 - 2019)
 * Florian Arrestier <florian.arrestier@insa-rennes.fr> (2018 - 2019)
 * Hugo Miomandre <hugo.miomandre@insa-rennes.fr> (2017)
 * Julien Heulot <julien.heulot@insa-rennes.fr> (2014 - 2018)
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
#include <launcher/Launcher.h>
#include "Scheduler.h"
#include "graphs/SRDAG/SRDAGGraph.h"

int Scheduler::computeScheduleLevel(SRDAGVertex *vertex) {
    int lvl = 0;
    if (vertex->getSchedLvl() == -1) {
        for (int i = 0; i < vertex->getNConnectedOutEdge(); i++) {
            SRDAGVertex *succ = vertex->getOutEdge(i)->getSnk();
            if (succ && succ->getState() != SRDAG_NEXEC) {
                Time minExecTime = (unsigned int) -1;
                for (std::uint32_t j = 0; j < archi_->getNPE(); j++) {
                    if (succ->isExecutableOn(j)) {
                        Time execTime = succ->executionTimeOn(archi_->getPEFromSpiderID(j)->getHardwareType());
                        if (execTime == 0) {
                            throwSpiderException("Vertex: %s -- NULL execution time.", succ->toString());
                        }
                        minExecTime = std::min(minExecTime, execTime);
                    }
                }
                lvl = std::max(lvl, computeScheduleLevel(succ) + (int) minExecTime);
            }
        }
        vertex->setSchedLvl(lvl);
        return lvl;
    }
    return vertex->getSchedLvl();
}


void Scheduler::addPrevActors(SRDAGVertex *vertex, List<SRDAGVertex *> *list) {
    for (int i = 0; i < vertex->getNConnectedInEdge(); i++) {
        SRDAGVertex *prevVertex = vertex->getInEdge(i)->getSrc();
        if (!list->isPresent(prevVertex) && prevVertex->getState() == SRDAG_EXEC) {
            list->add(prevVertex);
            computeScheduleLevel(prevVertex);
            addPrevActors(prevVertex, list);
        }
    }
}

void Scheduler::scheduleOnlyConfig(
        SRDAGGraph *graph,
        MemAlloc *memAlloc,
        SRDAGSchedule *schedule,
        Archi *archi) {
    srdag_ = graph;
    schedule_ = schedule;
    archi_ = archi;

    list_ = CREATE(TRANSFO_STACK, List<SRDAGVertex *>)(TRANSFO_STACK, srdag_->getNExecVertex());

    for (int i = 0; i < srdag_->getNVertex(); i++) {
        SRDAGVertex *vertex = srdag_->getVertex(i);
        if (vertex->getState() == SRDAG_EXEC && vertex->getNOutParam() > 0) {
            list_->add(vertex);
            vertex->setSchedLvl(-1);
            addPrevActors(vertex, list_);
        }
    }

    memAlloc->alloc(list_);

    for (int i = 0; i < list_->size(); i++) {
        computeScheduleLevel((*list_)[i]);
    }

    list_->sort(compareScheduleLevel);

    schedule_->setAllMinReadyTime(Platform::get()->getTime());
    schedule_->setReadyTime(
            /* Spider Pe */      archi->getSpiderGRTID(),
            /* End of Mapping */ Platform::get()->getTime() +
                                 archi->getScheduleTimeRoutine()(list_->size(), archi_->getNPE()));


    for (int i = 0; i < list_->size(); i++) {
        this->mapVertex((*list_)[i]);
    }

    /** Send Broadcast notification **/
    Launcher::get()->sendBroadCastNotification(true);


    list_->~List();
    StackMonitor::free(TRANSFO_STACK, list_);
}


void Scheduler::schedule(SRDAGGraph *graph, MemAlloc *memAlloc, SRDAGSchedule *schedule, Archi *archi) {
    srdag_ = graph;
    schedule_ = schedule;
    archi_ = archi;

    list_ = CREATE(TRANSFO_STACK, List<SRDAGVertex *>)(TRANSFO_STACK, srdag_->getNExecVertex());

    // Fill the list_ with SRDAGVertices in scheduling order
    for (int i = 0; i < srdag_->getNVertex(); i++) {
        SRDAGVertex *vertex = srdag_->getVertex(i);
        if (vertex->getState() == SRDAG_EXEC) {
            list_->add(vertex);
            vertex->setSchedLvl(-1);
        }
    }

    memAlloc->alloc(list_);

    for (int i = 0; i < list_->size(); i++) {
        computeScheduleLevel((*list_)[i]);
    }

    list_->sort(Scheduler::compareScheduleLevel);

    schedule_->setAllMinReadyTime(Platform::get()->getTime());
    schedule_->setReadyTime(
            /* Spider Pe */        archi->getSpiderGRTID(),
            /* End of Mapping */Platform::get()->getTime() +
                                archi->getScheduleTimeRoutine()(list_->size(), archi_->getNPE()));

    for (int i = 0; i < list_->size(); i++) {
        this->mapVertex((*list_)[i]);
    }

    list_->~List();
    StackMonitor::free(TRANSFO_STACK, list_);
}

Time Scheduler::computeMinimumStartTime(SRDAGVertex *vertex) {
    Time minimumStartTime = 0;
    auto *job = vertex->getScheduleJob();
    auto *jobConstrains = job->getScheduleConstrain();

    for (int i = 0; i < vertex->getNConnectedInEdge(); i++) {
        auto *edge = vertex->getInEdge(i);
        if (edge->getRate() != 0) {
            auto *srcVertex = edge->getSrc();
            auto *srcJob = srcVertex->getScheduleJob();
            auto pe = srcJob->getMappedPE();
            auto currentValue = jobConstrains[pe].jobId_;
            minimumStartTime = std::max(minimumStartTime, srcJob->getMappingEndTime());
            if (srcJob->getJobID() > currentValue) {
                job->setScheduleConstrain(pe, srcVertex->getSetIx(), srcJob->getJobID());
            }
        }
    }

    return minimumStartTime;
}


