/**
 * Copyright or © or Copr. IETR/INSA - Rennes (2013 - 2019) :
 *
 * Antoine Morvan <antoine.morvan@insa-rennes.fr> (2018 - 2019)
 * Clément Guy <clement.guy@insa-rennes.fr> (2014)
 * Florian Arrestier <florian.arrestier@insa-rennes.fr> (2018 - 2019)
 * Hugo Miomandre <hugo.miomandre@insa-rennes.fr> (2017)
 * Julien Heulot <julien.heulot@insa-rennes.fr> (2013 - 2015)
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
#include <monitor/TimeMonitor.h>
#include <launcher/Launcher.h>
#include <lrt.h>
#include "PiSDFSchedule.h"
#include <graphs/PiSDF/PiSDFGraph.h>


PiSDFSchedule::PiSDFSchedule(int nPE, int nJobMax) : Schedule(nPE, nJobMax) {
    jobs_.reserve((unsigned long) nJobMax);
}

PiSDFSchedule::~PiSDFSchedule() {
    jobs_.clear();
}

void PiSDFSchedule::addJob(PiSDFScheduleJob *job, int instance) {
    jobs_.push_back(job);
    int pe = job->getMappedPE(instance);
    if (pe < 0 || pe >= nPE_) {
        throwSpiderException("Bad pe value: %d", pe);
    }
    auto *jobConstrains = job->getScheduleConstrain(instance);
    for (int i = 0; i < nPE_; ++i) {
        /** Set lrt dependency of previous job **/
        auto *vertex = jobConstrains[i].vertex_;
        auto vertexInstance = jobConstrains[i].vertexInstance_;
        if (vertex) {
            auto *vertexScheduleJob = vertex->getScheduleJob();
            vertexScheduleJob->setInstancePEDependency(vertexInstance, pe, true);
        }
    }
    job->setJobID(instance, nJobPerPE_[pe]++);
    readyTimeOfPEs_[pe] = std::max(readyTimeOfPEs_[pe], job->getMappingEndTime(instance));
    nJobs_++;
}

void PiSDFSchedule::print(const char *path) {
    FILE *file = Platform::get()->fopen(path);

    // Writing header
    Platform::get()->fprintf(file, "<data>\n");

    // Exporting for gantt display
    for (int i = 0; i < nJobs_; ++i) {
        jobs_[i]->resetLaunchInstances();
    }
    for (int i = 0; i < nJobs_; ++i) {
        jobs_[i]->print(file, jobs_[i]->getNumberOfLaunchedInstances());
        jobs_[i]->launchNextInstance();
    }
    Platform::get()->fprintf(file, "</data>\n");

    Platform::get()->fclose(file);
}

bool PiSDFSchedule::check() {

//    /* Check core concurrency */
//    for (int pe = 0; pe < nPE_; ++pe) {
//        auto &jobsOfPE = jobs_[pe];
//        auto *currentJob = jobsOfPE.front();
//        auto *nextJob = currentJob->getNextJob();
//        while (nextJob) {
//            if (currentJob->getEndTime() > nextJob->getStartTime()) {
//                auto *currentJobVertex = currentJob->getVertex();
//                auto *nextJobVertex = nextJob->getVertex();
//                throwSpiderException("Superposition of tasks [%s] and [%s] on PE [%d].", currentJobVertex->toString(),
//                                     nextJobVertex->toString(), pe);
//            }
//            currentJob = nextJob;
//            nextJob = currentJob->getNextJob();
//        }
//    }
//
//    /* Check Communications */
//    for (int pe = 0; pe < nPE_; ++pe) {
//        for (auto &job : jobs_[pe]) {
//            for (auto &precJob : job->getPredecessors()) {
//                auto *currentJobVertex = job->getVertex();
//                auto *precJobVertex = precJob->getVertex();
//                if (precJob->getEndTime() > job->getStartTime()) {
//                    throwSpiderException("Task [%s] (PE %d) depending on task [%s] (PE %d) overlaps in time.",
//                                         currentJobVertex->toString(),
//                                         job->getPE(), precJobVertex->toString(), precJob->getPE());
//                }
//            }
//        }
//    }
    return true;
}

void PiSDFSchedule::execute() {
    TimeMonitor::startMonitoring();
    int startJob = nSentJobs_;
    for (int i = startJob; i < nJobs_; ++i) {
        auto *job = jobs_[i];
        Launcher::get()->sendJob(job);
        nSentJobs_++;
    }
    TimeMonitor::endMonitoring(TRACE_SPIDER_SCHED);
}



