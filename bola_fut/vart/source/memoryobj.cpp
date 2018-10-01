/// \file memoryobj.cpp
/// \brief Implementation file for V-ART class "MemoryObj".
/// \version $Revision: 1.2 $

#include "vart/memoryobj.h"

//#include <iostream>
//using namespace std;

VART::MemoryObj::MemoryObj()
{
    autoDelete = false;
}

VART::MemoryObj::MemoryObj(const VART::MemoryObj &obj)
: autoDelete( obj.autoDelete )
{
}
