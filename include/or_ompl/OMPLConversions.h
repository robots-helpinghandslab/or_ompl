/***********************************************************************

Copyright (c) 2014, Carnegie Mellon University
All rights reserved.

Authors: Michael Koval <mkoval@cs.cmu.edu>
         Matthew Klingensmith <mklingen@cs.cmu.edu>
         Christopher Dellin <cdellin@cs.cmu.edu>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

  Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*************************************************************************/
#ifndef OMPLCONVERSIONS_H_
#define OMPLCONVERSIONS_H_
#include <ompl/util/Console.h>
#include <or_ompl/RobotStateSpace.h>
#include <openrave/openrave.h>
#include "OMPLPlannerParameters.h"

typedef boost::shared_ptr<ompl::base::RealVectorStateSpace> RealVectorSpacePtr;

namespace or_ompl {

struct OpenRAVEHandler : public ompl::msg::OutputHandler {
public:
    virtual void log(std::string const &text, ompl::msg::LogLevel level,
                     char const *filename, int line);
};

RobotStateSpacePtr CreateStateSpace(OpenRAVE::RobotBasePtr const robot,
                                    OMPLPlannerParameters const &params);

}

#endif
