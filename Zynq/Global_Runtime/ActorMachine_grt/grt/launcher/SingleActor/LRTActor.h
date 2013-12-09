/****************************************************************************
 * Copyright or � or Copr. IETR/INSA (2013): Julien Heulot, Yaset Oliva,	*
 * Maxime Pelcat, Jean-Fran�ois Nezan, Jean-Christophe Prevotet				*
 * 																			*
 * [jheulot,yoliva,mpelcat,jnezan,jprevote]@insa-rennes.fr					*
 * 																			*
 * This software is a computer program whose purpose is to execute			*
 * parallel applications.													*
 * 																			*
 * This software is governed by the CeCILL-C license under French law and	*
 * abiding by the rules of distribution of free software.  You can  use, 	*
 * modify and/ or redistribute the software under the terms of the CeCILL-C	*
 * license as circulated by CEA, CNRS and INRIA at the following URL		*
 * "http://www.cecill.info". 												*
 * 																			*
 * As a counterpart to the access to the source code and  rights to copy,	*
 * modify and redistribute granted by the license, users are provided only	*
 * with a limited warranty  and the software's author,  the holder of the	*
 * economic rights,  and the successive licensors  have only  limited		*
 * liability. 																*
 * 																			*
 * In this respect, the user's attention is drawn to the risks associated	*
 * with loading,  using,  modifying and/or developing or reproducing the	*
 * software by the user in light of its specific status of free software,	*
 * that may mean  that it is complicated to manipulate,  and  that  also	*
 * therefore means  that it is reserved for developers  and  experienced	*
 * professionals having in-depth computer knowledge. Users are therefore	*
 * encouraged to load and test the software's suitability as regards their	*
 * requirements in conditions enabling the security of their systems and/or *
 * data to be ensured and,  more generally, to use and operate it in the 	*
 * same conditions as regards security. 									*
 * 																			*
 * The fact that you are presently reading this means that you have had		*
 * knowledge of the CeCILL-C license and that you accept its terms.			*
 ****************************************************************************/

#ifndef LRTACTOR_H_
#define LRTACTOR_H_

#include "grt_definitions.h"
#include "launcher/launcher.h"
#include "launcher/ActorMachine/AMGraph.h"

class LRTActor {
private:
	UINT32 nbInputFifos;
	UINT32 nbOutputFifos;
	LauncherFIFO* inFIFOs[MAX_NB_FIFO];
	LauncherFIFO* outFIFOs[MAX_NB_FIFO];
//	UINT32 inputFifoId[MAX_NB_FIFO];
//	UINT32 outputFifoId[MAX_NB_FIFO];
//	UINT32 inputFifoAddr[MAX_NB_FIFO];
//	UINT32 outputFifoAddr[MAX_NB_FIFO];
//	UINT32 readDataSize[MAX_NB_FIFO];
//	UINT32 writeDataSize[MAX_NB_FIFO];
public:
	LRTActor(){};
	LRTActor(SRDAGGraph *graph, SRDAGVertex* srvertex, launcher*);
	void prepare(int slave, launcher* launch);
};

#endif /* LRTACTOR_H_ */