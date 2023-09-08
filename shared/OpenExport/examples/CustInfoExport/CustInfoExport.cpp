#include "stdafx.h"

#include <fstream>

class  RwCommBuilder;
class  RwRf3Builder;

#include "RwExpBspExporter.h"
#include "RwCommMessage.h"
#include "RwRf3Exporter.h"
#include "RwExpMgr.h"
#include "RwExpMgrExportManager.h"
#include "RwCommVersion.h"
#include "RwCommTransform.h"
#include "RwCommMesh.h"
#include "RwCommScene.h"


class RwCustInfoExport : public RwExpBspExporter
{
public:

    virtual 
    ~RwCustInfoExport( void )
    {
        // just in case
        CloseLogFile();  
    }

    bool 
    InitaliseLogging(const RwString &FileName)
    {
        fileName = FileName;

        // try and open the file to make sure it is valid

        bool Result = OpenLogFile();
        CloseLogFile();

        return Result;
    }

public:     // some log file methods

    bool 
    OpenLogFile( void )
    {
        // cannot call InitaliseLogging Twice
        if(outFile.is_open())
        {
            return false;
        }

        // open the file
        outFile.open(fileName.c_str());

        if(outFile.is_open())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void 
    CloseLogFile( void )
    {
        outFile.close();
    }

private:

    // After the export is perform, we want to dump some information 
    // to our log file. 
    virtual RwCommError     
    PostExport(RwCommScene* pScene, const RwVector<RwCommObject*>& vSelection)
    {
        RwVector<RwCommObject*> vObjects;

        if( !OpenLogFile() ) 
        {
            // we can't open the file for logging
            PRINT_MESSAGE(RwString("Cannot Open Custom Log File :") + fileName,"",0);
            return RwExpBspExporter::PostExport(pScene, vSelection);
        }

        PRINT_MESSAGE("Performing Custom Data Export", "", 1);

        // Get all the meshes in the scene
        pScene->GetAllObjectsOfType(RwCommMeshType, vObjects);

        // for all of the meshes
        for(RwVector<RwCommObject*>::const_iterator  i =  vObjects.begin();
                                                      i != vObjects.end();
                                                      ++i)
        {
            // Check if we've expored this mesh
            RwExpBspGeometry *pBspGeometry = dynamic_cast<RwExpBspGeometry*>(GetExportObject(*i));
            if (pBspGeometry)
            {
                // dump the name of the object
                outFile << "    Name    : " << RwUtilW2A((*i)->GetName()) << endl;
                
                // dump the number of exported primitives
                outFile << "    Number Of Primitives :" << pBspGeometry->GetVertexCollection().GetPrimitives().size() << endl;

                // dump the number of vertices in the exported mesh
                outFile << "    Number of Vertices :" << pBspGeometry->GetVertexCollection().GetVertices().size() << endl; 
                
                outFile << endl;
            }
        }

        CloseLogFile();

        return RwExpBspExporter::PostExport(pScene, vSelection);
    }

private:

    ofstream outFile;
    RwString   fileName;

};

BOOL APIENTRY 
DllMain( HANDLE hModule, 
         DWORD  ul_reason_for_call, 
         LPVOID lpReserved)
{
    return TRUE;
}

__declspec(dllexport) bool 
InitRwPlugin()
{
    return true;
}

__declspec(dllexport) void 
InitExportManager(RwExpMgrExportManager *pExportManager)
{
    // Create my own custom exporter and set it to export manager
    
    RwString FileName = "C:\\CustomDataLog.txt";

    RwCustInfoExport *NewExporter = new RwCustInfoExport;

    if(!NewExporter->InitaliseLogging(FileName))
    {
        // cannot initalise the loggin, so don't install the exporter
        delete NewExporter;
        return;
    }

    pExportManager->SetBspExporter(NewExporter);
}





