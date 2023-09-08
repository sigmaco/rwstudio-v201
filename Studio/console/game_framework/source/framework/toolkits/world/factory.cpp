/*****************************************************************************
*
* File :     Factory.cpp
*
* Abstract : Factory functions for atomics and clumps
*
*****************************************************************************
*
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on Criterion Software Ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software Ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"

//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "factory.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "helpers/clumphelper.h"
#include "clevel.h"

namespace RWS
{
   /**
   *
   *  \ingroup Factory
   *
   *  Create an RpAtomic from the resource specified within the CAttributePacket, and add it to the main world
   *
   */
   RpAtomic *CreateAtomicInWorldFromResource( const CAttributePacket& attr, CEventHandler* eventHandler)
   {
      RWS_FUNCTION( "RWS::CreateAtomicInWorldFromResource");
      
      RpAtomic *pAtomic = CreateAtomicFromResource( attr, eventHandler);
      
      if ( pAtomic)
      {
         // Check if we need to create/attach a frame to the atomic
         //
         if (RpAtomicGetFrame( pAtomic) == 0)
         {
            RwFrame* pFrame = RwFrameCreate();
            
            RWS_ASSERT( pFrame, "RwFrameCreate failed");
            
            RwFrameSetIdentity( pFrame );
            RpAtomicSetFrame( pAtomic, pFrame );
         }
         CLevel::AddAtomic(pAtomic);
      }
      
      RWS_RETURN( pAtomic);
   }
   
   namespace 
   {
      /*
      *
      * Used to store pointer to atomic with clump obtained by the callback extract_atomic
      *
      */
      typedef struct clumpcallbackdata_tag
      {
         RpAtomic *pAtomic;
      } clumpcallbackdata;
   
      /*
      *
      *  \ingroup Factory
      *
      *  This callback is used to obtain a pointer to the first (and only) atomic in a clump
      *
      */
      RpAtomic *ExtractAtomic_CB(RpAtomic *pAtomic, void *data)
      {
         RWS_FUNCTION("RWS::NULL::ExtractAtomic_CB");

         clumpcallbackdata *pdata = static_cast<clumpcallbackdata*>(data);
      
         RpClump *clump = RpAtomicGetClump( pAtomic);
      
         RpClumpRemoveAtomic( clump, pAtomic);
         RpClumpSetFrame( clump, 0);   // Clump and atomic share frame, 
      
         pdata->pAtomic = pAtomic;
      
         RWS_RETURN( 0 );
      }
   }

   /**
   *
   *  \ingroup Factory
   *
   *  Create an RpAtomic from the resource specified within the CAttributePacket
   *
   */
   RpAtomic *CreateAtomicFromResource( const CAttributePacket& attr, CEventHandler* eventHandler)
   {
      RWS_FUNCTION( "RWS::CreateAtomicFromResource");
      
      RpAtomic *pAtomic = 0;
      
      const RWSGUID * pResourceID = CSystemCommands::FindFirstResource( attr);
      
      RWS_ASSERT( pResourceID, "FindFirstResource failed - could not find resource.");
      
      const RwChar *resourceType;

      const void *pObject = CResourceManager::FindById(pResourceID, &resourceType);
      
      RWS_ASSERT(pObject, "Resource::Find failed - could not find resource ident ( "
            << pResourceID << " )");
      
      if (!rwstrcmp( "rwID_ATOMIC", resourceType))
      {
         // If the resource is an atomic, clone the atomic and return.
         //
         //
         pAtomic = RpAtomicClone( const_cast<RpAtomic*>(static_cast<const RpAtomic*>(pObject)) );
      }
      else
         if (!rwstrcmp( "rwID_CLUMP", resourceType))
         {
            // If the resource is a clump and it only contains one atomic, then clone the clump, extract the atomic
            // and delete the clump.
            //
            //
            RpClump *pClump = RpClumpClone( const_cast<RpClump*>(static_cast<const RpClump*>(pObject)) );
            
            RWS_ASSERT( pClump, "RpClumpClone failed");
            
            if ( RpClumpGetNumAtomics(pClump)>1)
            {
               RWS_ASSERTFAIL( "RpClumpGetNumAtomics == "<<RpClumpGetNumAtomics( pClump)<<
                  " cannot create atomic if > 1");

               RpClumpDestroy( pClump);
            }
            else
            {
               clumpcallbackdata data;
               
               data.pAtomic = 0;
               
               RpClumpForAllAtomics  ( pClump, ExtractAtomic_CB,&data);
               
               pAtomic = data.pAtomic;
               
               RpClumpDestroy( pClump);
            }
         }
         else
         {
            RWS_ASSERTFAIL( "Cannot create a RpAtomic from type "<<resourceType);
         }
         
         if (pAtomic && eventHandler) RpAtomicToEventHandler::SetEventHandler( *pAtomic, eventHandler);
         
         RWS_RETURN( pAtomic);
   }
   
   /**
   *
   *  \ingroup Factory
   *
   *  Create an RpClump from the resource specified within the CAttributePacket, and add it to the main world
   *
   */
   RpClump *CreateClumpInWorldFromResource( const CAttributePacket& attr, CEventHandler* eventHandler)
   {
      RWS_FUNCTION( "RWS::CreateClumpInWorldFromResource");
      
      RpClump *pClump = CreateClumpFromResource( attr, eventHandler);
      
      if ( pClump )
      {
         // Check if we need to create/attach a frame to the clump
         //
         if ( RpClumpGetFrame( pClump) == 0 )
         {
            RwFrame* pFrame = RwFrameCreate();
            
            RWS_ASSERT( pFrame, "RwFrameCreate failed");
            
            RwFrameSetIdentity( pFrame );
            RpClumpSetFrame( pClump, pFrame );
         }
         CLevel::AddClump(pClump);
      }
      
      RWS_RETURN( pClump);
   }
   
   /**
   *
   *  \ingroup Factory
   *
   *  Create an RpClump from the resource specified within the CAttributePacket
   *
   */
   RpClump *CreateClumpFromResource( const CAttributePacket& attr, CEventHandler* eventHandler)
   {
      RWS_FUNCTION( "RWS::CreateClumpFromResource");
      
      RpClump *pClump = 0;
      
      const RWSGUID * pResourceID = CSystemCommands::FindFirstResource( attr);
      
      RWS_ASSERT( pResourceID, "FindFirstResource failed - could not find resource.");
      
      const RwChar *resourceType;
      
      const void *pObject = CResourceManager::FindById(pResourceID, &resourceType);
      
      RWS_ASSERT( pObject, "Resource::Find failed - could not find resource ident ( "
            << pResourceID << " )");
      
      if (!rwstrcmp( "rwID_ATOMIC", resourceType))
      {
         // If the resource is an atomic, create a dummy clump and add a clone of the atomic to it.
         //
         //
         pClump = RpClumpCreate();
         
         RWS_ASSERT( pClump, "RpClumpCreate failed");
         
         RpAtomic *pAtomic = RpAtomicClone( const_cast<RpAtomic*>(static_cast<const RpAtomic*>(pObject)) );
         
         RWS_ASSERT( pAtomic, "RpAtomicClone failed");

         // Need to create a frame which is shared by both the clump and the atomic.
         //
         RwFrame* pFrame = RwFrameCreate();
            
         RWS_ASSERT( pFrame, "RwFrameCreate failed");
            
         RwFrameSetIdentity( pFrame );

         RpAtomicSetFrame( pAtomic, pFrame );

         RpClumpAddAtomic( pClump, pAtomic);

         RpClumpSetFrame( pClump, pFrame );
      }
      else
         if ( !rwstrcmp( "rwID_CLUMP", resourceType))
         {
            // If the resource is a clump, return a clone of the clump
            //
            //
            pClump = RpClumpClone( const_cast<RpClump*>(static_cast<const RpClump*>(pObject)) );
         }
         else
         {
            // Any other resource types not handled
            //
            //
            RWS_ASSERTFAIL( "Cannot create a RpClump from type "<<resourceType);
         }
         
         if (pClump && eventHandler) ClumpHelper::SetEventHandler (*pClump, eventHandler);
         
         RWS_RETURN( pClump);
   }
}
