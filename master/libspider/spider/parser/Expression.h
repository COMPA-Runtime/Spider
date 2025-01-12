/**
 * Copyright or © or Copr. IETR/INSA - Rennes (2014 - 2019) :
 *
 * Antoine Morvan <antoine.morvan@insa-rennes.fr> (2018 - 2019)
 * Florian Arrestier <florian.arrestier@insa-rennes.fr> (2018 - 2019)
 * Julien Heulot <julien.heulot@insa-rennes.fr> (2014 - 2016)
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
#ifndef PARSER_EXPRESSION_H
#define PARSER_EXPRESSION_H

#include <graphs/PiSDF/PiSDFParam.h>
//#include "spider.h"

struct transfoJob;

class Expression {
public:
    Expression(const char *expr, const PiSDFParam *const *params, int nParam);

    virtual ~Expression();

    Param evaluate(const PiSDFParam *const *paramList, transfoJob *job, bool *ok = nullptr) const;

    Param evaluate(const Param *vertexParamValues, int nParam) const;

    Param evaluate() const;

    void toString(
            const PiSDFParam *const *params, int nParam,
            char *out, size_t outSizeMax);

    inline const char *toString();

private:
    typedef enum {
        OPERATOR,
        VALUE,
        PARAMETER,
        LEFT_PAR,
        RIGHT_PAR
    } Type;

    typedef enum {
        ADD,
        SUB,
        MUL,
        DIV,
        CEIL,
        FLOOR,
        POW
    } OpType;

    typedef struct {
        Type type;
        OpType opType;
        int value;
        int paramIx;
        PiSDFParam *param = nullptr;
    } Token;

    std::string stringExpr_;
    Token *stack_;
    int nElt_;
    bool isStatic_;
    Param value_;

    bool getNextToken(
            Token *token,
            const char **expr,
            const PiSDFParam *const *params, int nParam);

    int evaluateNTokens(const char *expr);

};

inline const char *Expression::toString() {
    return stringExpr_.c_str();
}

#endif/*PARSER_EXPRESSION_H*/
