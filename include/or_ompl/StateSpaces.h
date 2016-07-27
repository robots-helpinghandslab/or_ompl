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

#ifndef OR_OMPL_STATESPACES_H_
#define OR_OMPL_STATESPACES_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <openrave/openrave.h>
#include <ompl/base/StateSpace.h>
#include <ompl/base/spaces/SO2StateSpace.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <ompl/base/StateValidityChecker.h>
#include <or_ompl/config.h>

namespace or_ompl {

/**
 * Implements a state space for an OpenRAVE robot
 */
class ContinuousJointsStateSpace : public ompl::base::CompoundStateSpace {

public:
    /**
     * Constructor
     * @param dof_indices An ordered list of indices this state corresponds to
     */
    ContinuousJointsStateSpace(const std::vector<bool>& is_continuous);

    /** \brief Register the projections for this state space. Usually, this is at least the default
        projection. These are implicit projections, set by the implementation of the state space. This is called by setup(). */
    virtual void registerProjections();

    /**
     * Set the upper/lower bounds of the state space.
     */
    void setBounds(const ompl::base::RealVectorBounds& bounds);

private:
    std::vector<bool> _isContinuous;
    ompl::base::ProjectionEvaluatorPtr _projectionEvaluator;

};

class ContinuousJointsProjectionEvaluator : public ompl::base::ProjectionEvaluator {
    public:
        ContinuousJointsProjectionEvaluator(ompl::base::StateSpace* stateSpace);
        ContinuousJointsProjectionEvaluator(ompl::base::StateSpacePtr stateSpace);
        virtual ~ContinuousJointsProjectionEvaluator();

        /** \brief Return the dimension of the projection defined by this evaluator */
        virtual unsigned int getDimension() const;

        /** \brief Compute the projection as an array of double values */
        virtual void project(const ompl::base::State *state, ompl::base::EuclideanProjection &projection) const;

        virtual void defaultCellSizes();

        virtual void setup();
    protected:
        or_ompl::ContinuousJointsStateSpace* _robotStateSpace;
        ompl::base::ProjectionMatrix _projectionMatrix;
};

/**
 * This is like ompl::base::StateValidityChecker,
 * except it also knows how to compute forward kinematics
 * to match the state
 * 
 * this should work for general state spaces (e.g. CompoundStateSpaces)
 */
class OrStateValidityChecker: public ompl::base::StateValidityChecker
{
public:
    OrStateValidityChecker(const ompl::base::SpaceInformationPtr &si,
        OpenRAVE::RobotBasePtr robot, std::vector<int> const &indices);
    virtual bool computeFk(const ompl::base::State *state, uint32_t checklimits) const;
    virtual bool isValid(const ompl::base::State *state) const;
    void resetStatistics() { m_numCollisionChecks = 0; m_totalCollisionTime = 0.0; }
    int getNumCollisionChecks() { return m_numCollisionChecks; }
    double getTotalCollisionTime() { return m_totalCollisionTime; }
    const std::vector<int> & getIndices() { return m_indices; }
protected:
    ompl::base::StateSpace * m_stateSpace;
    OpenRAVE::EnvironmentBasePtr m_env;
    OpenRAVE::RobotBasePtr m_robot;
    std::vector<int> const m_indices;
    mutable int m_numCollisionChecks;
    mutable double m_totalCollisionTime;
};

/**
 * StateRobotSetter for RealVectorStateSpaces
 */
class RealVectorOrStateValidityChecker: public OrStateValidityChecker
{
public:
    RealVectorOrStateValidityChecker(const ompl::base::SpaceInformationPtr &si,
        OpenRAVE::RobotBasePtr robot, std::vector<int> const &indices);
    virtual bool computeFk(const ompl::base::State *state, uint32_t checklimits) const;
private:
    const std::size_t m_num_dof;
};

#ifdef OR_OMPL_HAS_BOOSTSMARTPTRS
typedef boost::shared_ptr<ContinuousJointsStateSpace> ContinuousJointsStateSpacePtr;
typedef boost::shared_ptr<ContinuousJointsProjectionEvaluator> ContinuousJointsProjectionEvaluatorPtr;
typedef boost::shared_ptr<OrStateValidityChecker> OrStateValidityCheckerPtr;
#else
typedef std::shared_ptr<ContinuousJointsStateSpace> ContinuousJointsStateSpacePtr;
typedef std::shared_ptr<ContinuousJointsProjectionEvaluator> ContinuousJointsProjectionEvaluatorPtr;
typedef std::shared_ptr<OrStateValidityChecker> OrStateValidityCheckerPtr;
#endif

} // namespace or_ompl

#endif // OR_OMPL_STATESPACES_H_
