/**
 * Copyright or © or Copr. IETR/INSA - Rennes (2013 - 2019) :
 *
 * Antoine Morvan <antoine.morvan@insa-rennes.fr> (2018 - 2019)
 * Clément Guy <clement.guy@insa-rennes.fr> (2014)
 * Florian Arrestier <florian.arrestier@insa-rennes.fr> (2018 - 2019)
 * Hugo Miomandre <hugo.miomandre@insa-rennes.fr> (2017)
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
#include "PThreadSpiderCommunicator.h"

#include <platform.h>

PThreadSpiderCommunicator::PThreadSpiderCommunicator(ControlMessageQueue<JobInfoMessage *> *spider2LrtJobQueue,
                                                     ControlMessageQueue<ParameterMessage *> *lrt2SpiderParamQueue,
                                                     NotificationQueue<NotificationMessage> **notificationQueue,
                                                     ControlMessageQueue<TraceMessage *> *traceQueue) {
    spider2LrtJobQueue_ = spider2LrtJobQueue;
    lrt2SpiderParamQueue_ = lrt2SpiderParamQueue;
    notificationQueue_ = notificationQueue;
    traceQueue_ = traceQueue;
}

void PThreadSpiderCommunicator::push_notification(int lrtID, NotificationMessage *msg) {
    notificationQueue_[lrtID]->push(msg);
}

bool PThreadSpiderCommunicator::pop_notification(int lrtID, NotificationMessage *msg, bool blocking) {
    return notificationQueue_[lrtID]->pop(msg, blocking);
}

std::int32_t PThreadSpiderCommunicator::push_job_message(JobInfoMessage **message) {
    return spider2LrtJobQueue_->push(message);
}

void PThreadSpiderCommunicator::pop_job_message(JobInfoMessage **msg, std::int32_t id) {
    spider2LrtJobQueue_->pop(msg, id);
}

std::int32_t PThreadSpiderCommunicator::push_parameter_message(ParameterMessage **message) {
    return lrt2SpiderParamQueue_->push(message);
}

void PThreadSpiderCommunicator::pop_parameter_message(ParameterMessage **msg, std::int32_t id) {
    lrt2SpiderParamQueue_->pop(msg, id);
}


std::int32_t PThreadSpiderCommunicator::push_trace_message(TraceMessage **message) {
    return traceQueue_->push(message);
}

void PThreadSpiderCommunicator::pop_trace_message(TraceMessage **message, std::int32_t id) {
    traceQueue_->pop(message, id);
}
