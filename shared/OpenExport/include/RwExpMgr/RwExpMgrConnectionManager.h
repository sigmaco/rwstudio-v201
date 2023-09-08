// RwExpMgrConnectionManager.h
// This header defines an interface for a singleton object that handles connections for the visualizer

#pragma once
#include "RwExpMgr.h"

#if (!defined(DOXYGEN))

// This is the virtual class that 
class RwExpMgrConnectionManagerImpl
{
public:
	// Interface for detecting whether a connection exists for the visualizer
    virtual bool ValidConnectionExists() = 0;
};

class RWEXPMGR_API RwExpMgrConnectionManager
{

	// Manage as a singleton
	static RwExpMgrConnectionManagerImpl *s_pRegisteredManager;

public:

	// Manage as a singleton
    static void RegisterInstance(RwExpMgrConnectionManagerImpl *pInstance);
    static RwExpMgrConnectionManagerImpl *GetInstance();

    
};

#endif