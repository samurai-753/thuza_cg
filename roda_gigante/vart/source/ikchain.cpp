/// \file ikchain.cpp
/// \brief Implementation file for V-ART class "IKChain".
/// \version $Revision$

#include "vart/ikchain.h"
#include "vart/collector.h"
#include "vart/joint.h"
#include <list>
//#include <iostream>
using namespace std;

VART::IKChain::IKChain(SGPath path, Point4D eePos, Point4D eeOri) :
    eePosition(eePos), eeOrientation(eeOri)
{
    Collector<Joint> jointCollector;
    path.Traverse(&jointCollector);
    list<const Joint*>::const_iterator iter = jointCollector.begin();
    for(; iter != jointCollector.end(); ++iter)
    {
        // get dofs from joint
        const_cast<Joint*>(*iter)->GetDofs(&dofChain);
    }
}

void VART::IKChain::SetEEPosition(const Point4D& eePos)
{
    eePosition = eePos;
}

void VART::IKChain::MoveTowardsSolution()
{
    // Compute every dof position in world coordinates
    unsigned int i = dofChain.size();
    vector<Point4D> dofPosVec(i);
    --i; // i is now the index of the last dof
    Transform lim; // local  instance matrix
    Transform gim; // global instance matrix (accumulation of LIMs)
    gim.MakeIdentity();
    list<Dof*>::iterator iter = dofChain.begin();
    for (; iter != dofChain.end(); ++iter)
    {
        (*iter)->GetLim(&lim);
        gim.CopyMatrix(lim * gim);
        dofPosVec.push_back(gim * (*iter)->GetPosition());
    }
    // For each dof inwards to the base
    Point4D vectorToEE;
    Point4D vectorToTarget;
    list<Dof*>::reverse_iterator rIter = dofChain.rbegin();
    for (; rIter != dofChain.rend(); ++rIter)
    {
        // rotate dof so that the end effector gets close to target by ...
        // 1) computing the vector from dof to end effector
        vectorToEE =    eePosition - dofPosVec[i];
        // 2) computing the vector from dof to target position
        vectorToTarget = targetPos - dofPosVec[i];
        --i;
        // 3) computing the new dof configuration to make (1) close to (2).
        (*rIter)->Reconfigure(vectorToEE, vectorToTarget);
        // 4) update end effector position
        eePosition = 
    }
}