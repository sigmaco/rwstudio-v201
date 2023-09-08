/*****************************************************************************
*
* File :     SkinSafe.cpp
*
* Abstract : Helper functions for making RpSkins 'safe' w.r.t animation input.
*
*            Skins normally crash if they're rendered without having had
*            animation data applied so there are functions that can use the
*            default pose of the skin to 'fake' appropriate input.
*
*            Also provides helpers for verifying the compatibility of
*            animation input.
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
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "stdafx.h"
#include "skinsafe.h"
#include "debugmacros.h"

namespace RWS
{
   namespace
   {
      /**
      *
      *  \ingroup Animation
      *
      *  \param hierarchy A reference to a RpHAnimHierarchy object.
      *  \param flagMask Flag mask that specifies the RpHAnimHierarchyFlag's to check.
      *
      *  \returns true if all the flags in 'flagMask' are set on the given hierarchy
      *
      */
      bool FlagsAreSet( const RpHAnimHierarchy& hierarchy, RpHAnimHierarchyFlag flagMask )
      {
         RWS_FUNCTION("RWS::NULL::FlagsAreSet");
         RWS_RETURN(flagMask == (flagMask & RpHAnimHierarchyGetFlags(const_cast<RpHAnimHierarchy*>(&hierarchy))));
      }

      /**
      *
      *  \ingroup Animation
      *
      *  Inverts each matrix in the 'pSrcMatrices' into the 'pDstMatrices' array.
      *
      *  \param pSrcMatrices Pointer to an array of RwMatrix objects, source of matrices to invert.
      *  \param pDstMatrices Pointer to an array of RwMatrix objects, destination of matrices to invert.
      *  \param nMatrices The number of matrices to invert.
      *
      */
      void InvertMatrices( const RwMatrix *pSrcMatrices, RwMatrix *pDstMatrices, RwInt32 nMatrices )
      {
         RWS_FUNCTION("RWS::NULL::InvertMatrices")

         RWS_PRE(pSrcMatrices);
         RWS_PRE(pDstMatrices);

         while(nMatrices)
         {
            RwMatrixInvert(pDstMatrices,pSrcMatrices);

            --nMatrices;
            ++pSrcMatrices;
            ++pDstMatrices;
         }

         RWS_RETURNVOID()
      }

      /**
      *
      *  \ingroup Animation
      *
      *  Data for FindChildWithHAnimID callbacks
      *
      */
      struct FindChildWithHAnimIDCallbackData
      {
         FindChildWithHAnimIDCallbackData(RwInt32 id)
            : id_(id), pFoundFrame_(0) {}

         RwInt32 id_;
         RwFrame* pFoundFrame_;
      };

      /**
      *
      *  \ingroup Animation
      *
      *  Callback for FindChildWithHAnimID.
      *
      *  \param pFrame A pointer to a RwFrame object.
      *  \param pData A pointer to custom data, FindChildWithHAnimIDCallbackData
      *
      *  \return A pointer to the RwFrame object unless the correct object has been found where it returns 0, to
      *  terminate the search.
      *
      */
      RwFrame* FindChildWithHAnimIDCallback(RwFrame* pFrame, void* pData)
      {
         RWS_FUNCTION("RWS::NULL::FindChildWithHAnimIDCallback")

         RWS_PRE(pFrame);
         RWS_PRE(pData);

         FindChildWithHAnimIDCallbackData& cbData = *static_cast<FindChildWithHAnimIDCallbackData*>(pData);
         if(cbData.id_ == RpHAnimFrameGetID(pFrame))
         {
            cbData.pFoundFrame_ = pFrame;
            pFrame = 0;
         }

         RWS_RETURN(pFrame);
      }

      /**
      *
      *  \ingroup Animation
      *
      *  Callback for FindChildWithHAnimIDRecursive
      *
      *  \param pFrame A pointer to a RwFrame object.
      *  \param pData A pointer to custom data, FindChildWithHAnimIDCallbackData
      *
      *  \return A pointer to the RwFrame object unless the correct object has been found where it returns 0, to
      *  terminate the search.
      */
      RwFrame* FindChildWithHAnimIDRecursiveCallback(RwFrame* pFrame, void* pData)
      {
         RWS_FUNCTION("RWS::NULL::FindChildWithHAnimIDRecursiveCallback")

         RWS_PRE(pFrame);
         RWS_PRE(pData);

         FindChildWithHAnimIDCallbackData& cbData = *static_cast<FindChildWithHAnimIDCallbackData*>(pData);
         if(cbData.id_ == RpHAnimFrameGetID(pFrame))
         {
            cbData.pFoundFrame_ = pFrame;
            pFrame = 0;
         }
         else
         {
            // Recurse children
            RwFrameForAllChildren(pFrame,FindChildWithHAnimIDRecursiveCallback,pData);

            // Stop if a matching child was found
            if(cbData.pFoundFrame_) pFrame = 0;
         }

         RWS_RETURN(pFrame);
      }

      /**
      *
      *  \ingroup Animation
      *
      *  Finds the first child of 'frame' whose RpHAnim id matches 'id'. Returns NULL
      *  if no such RwFrame is found.
      *
      *  \param frame A reference to a RwFrame object.
      *  \param id The Id of the RwFrame to locate.
      *  \return a pointer to the RwFrame object with the Id of (id), if no frame is found returns 0.
      */
      const RwFrame* FindChildWithHAnimID(const RwFrame& frame, RwInt32 id)
      {
         RWS_FUNCTION("RWS::NULL::FindChildWithHAnimID")

         FindChildWithHAnimIDCallbackData cbData(id);
         RwFrameForAllChildren(const_cast<RwFrame*>(&frame),FindChildWithHAnimIDCallback,&cbData);

         RWS_RETURN(cbData.pFoundFrame_);
      }

      /**
      *
      *  \ingroup Animation
      *
      *  Recursively searches an RwFrame hierarchy for the first RwFrame whose RpHAnim
      *  id matches 'id'. Returns NULL if no such RwFrame is found.
      *
      *  \param frame A reference to a RwFrame object.
      *  \param id The Id of the RwFrame to locate.
      *  \return a pointer to the RwFrame object with the Id of (id), if no frame is found returns 0.
      */
      const RwFrame* FindChildWithHAnimIDRecursive(const RwFrame& frame, RwInt32 id)
      {
         RWS_FUNCTION("RWS::NULL::FindChildWithHAnimIDRecursive")

         FindChildWithHAnimIDCallbackData cbData(id);
         RwFrameForAllChildren(const_cast<RwFrame*>(&frame),FindChildWithHAnimIDRecursiveCallback,&cbData);

         RWS_RETURN(cbData.pFoundFrame_);
      }

      /**
      *
      *  \ingroup Animation
      *
      *  Similar to the STL's 'for_each' algorithm, with adaptations to allow
      *  for the hierarchical nature of the structure described by the array of
      *  RpHAnimNodeInfos. Instead of a functor, a 'descender' is used - each
      *  time a child is encountered, the descender is asked to descend into
      *  the child by creating a new instance of itself. The descender also
      *  has the opportunity to refuse to descend, in which case iteration
      *  stops short. 'pCurrInfo' is incremented each time an RpHAnimNodeInfo
      *  is successfully processed. Returns true if the entire hierarchy is
      *  successfully processed.
      *
      */
      template< class Desc >
      bool ForEachInHierarchy( const RpHAnimNodeInfo*& pCurrInfo, Desc& desc )
      {
         RWS_FUNCTION("RWS::NULL::ForEachInHierarchy")

         RWS_PRE( pCurrInfo );
         RWS_PRE( desc.IsValid(pCurrInfo) );

         // Find out if the next frame is a child of this one, then move right on to it
         bool nextFrameIsChild = (0 == (pCurrInfo->flags & rpHANIMPOPPARENTMATRIX));
         ++pCurrInfo;
      
         // If the last frame had children, subsequent frames will be child frames for as long
         // as there are 'push' flags...
         bool keepIterating = true;
         while(nextFrameIsChild && keepIterating)
         {
            // Can we descend into this child?
            if( desc.CanDescend(pCurrInfo) )
            {
               // Yes - remember whether the one after is a child and descend...
               nextFrameIsChild = (0 != (pCurrInfo->flags & rpHANIMPUSHPARENTMATRIX));
               Desc childDescender( desc.Descend(pCurrInfo) );
               keepIterating = ForEachInHierarchy<Desc>( pCurrInfo, childDescender ); 
            }
            else
            {
               // No - stop right here
               keepIterating = false;
            }
         }

         RWS_RETURN(keepIterating)
      }

      /**
      *
      *  \ingroup Animation
      *
      *  A version of 'ForEachInHierarchy' that works with const descenders
      *
      */
      template< class Desc >
      bool ConstForEachInHierarchy( const RpHAnimNodeInfo*& pCurrInfo, const Desc& desc )
      {
         RWS_FUNCTION("RWS::NULL::ConstForEachInHierarchy")
         RWS_RETURN(ForEachInHierarchy<const Desc>( pCurrInfo, desc ));
      }

      /**
      *
      *  \ingroup Animation
      *
      *  A descender that only descends if a child RwFrame with a matching
      *  frame ID can be found.
      *
      */
      class CFrameIsAttachable
      {
      public:
         CFrameIsAttachable( const RwFrame& frame ) : frame_(frame)
         {
            RWS_FUNCTION("RWS::NULL::CFrameIsAttachable::CFrameIsAttachable")
            RWS_RETURNVOID()
         }

         bool CanDescend( const RpHAnimNodeInfo* pChildInfo ) const
         {
            RWS_FUNCTION("RWS::NULL::CFrameIsAttachable::CanDescend")

            RWS_PRE(pChildInfo);

            RWS_RETURN(0 != FindChildWithHAnimID(frame_,pChildInfo->nodeID));
         }

         bool IsValid( const RpHAnimNodeInfo* pCurrInfo ) const
         {
            RWS_FUNCTION("RWS::NULL::CFrameIsAttachable::IsValid")

            RWS_PRE(pCurrInfo);

            RWS_RETURN(pCurrInfo->nodeID == RpHAnimFrameGetID(const_cast<RwFrame*>(&frame_)));
         }

         CFrameIsAttachable Descend( const RpHAnimNodeInfo* pChildInfo ) const
         {
            RWS_FUNCTION("RWS::NULL::CFrameIsAttachable::Descend")

            RWS_PRE( pChildInfo );
            RWS_PRE( CanDescend(pChildInfo) );

            const RwFrame* pFoundFrame = FindChildWithHAnimID(frame_,pChildInfo->nodeID);
            RWS_ASSERT( pFoundFrame, "No frame" );

            const CFrameIsAttachable result(*pFoundFrame);
            RWS_POST( result.IsValid(pChildInfo) );

            RWS_RETURN( result );
         }

      private:
         const RwFrame& frame_;
      };

      /**
      *
      *  \ingroup Animation
      *
      *  A descender that only descends if the RpHAnimNodeInfo iterator is
      *  within bounds.
      *
      */
      class CIteratorIsWithinBounds
      {
      public:
         CIteratorIsWithinBounds( const RpHAnimNodeInfo* pEndInfo )
            :  pEndInfo_(pEndInfo)
         {
            RWS_FUNCTION("RWS:NULL::CIteratorIsWithinBounds::CIteratorIsWithinBounds")

            RWS_PRE(pEndInfo);

            RWS_RETURNVOID()
         }

         bool CanDescend( const RpHAnimNodeInfo* pChildInfo ) const
         {
            RWS_FUNCTION("RWS:NULL::CIteratorIsWithinBounds::CanDescend")

            RWS_PRE(pChildInfo);

            RWS_RETURN(pChildInfo < pEndInfo_);
         }

         bool IsValid( const RpHAnimNodeInfo* pCurrInfo ) const
         {
            RWS_FUNCTION("RWS:NULL::CIteratorIsWithinBounds::IsValid")

            RWS_PRE(pCurrInfo);

            RWS_RETURN(pCurrInfo < pEndInfo_);
         }

         const CIteratorIsWithinBounds& Descend( const RpHAnimNodeInfo* pChildInfo ) const
         {
            RWS_FUNCTION("RWS:NULL::CIteratorIsWithinBounds::Descend")

            RWS_PRE( pChildInfo );
            RWS_PRE( CanDescend(pChildInfo) );

            const CIteratorIsWithinBounds& result = *this;
            RWS_POST( result.IsValid(pChildInfo) );

            RWS_RETURN(result);
         }

      private:
         const RpHAnimNodeInfo* pEndInfo_;
      };

      /**
      *
      *  \ingroup Animation
      *
      *  A descender that always descends, and populates the modelling matrices
      *  of an RwFrame hierarchy with a skin's default pose as it goes
      *
      */
      class CPopulateMatrices
      {
      public:
         CPopulateMatrices( const RwMatrix& invParentBone, const RwMatrix*& pBoneMatrix )
            : invParentBone_(invParentBone), pBoneMatrix_(pBoneMatrix)
         {
            RWS_FUNCTION("RWS:NULL::CPopulateMatrices::CPopulateMatrices")

            RWS_PRE(pBoneMatrix);

            RWS_RETURNVOID()
         }

         bool CanDescend( const RpHAnimNodeInfo* ) const
         {
            RWS_FUNCTION("RWS:NULL::CPopulateMatrices::CanDescend")
            RWS_RETURN(true);
         }

         bool IsValid( const RpHAnimNodeInfo* ) const
         {
            RWS_FUNCTION("RWS:NULL::CPopulateMatrices::IsValid")
            RWS_RETURN(true);
         }

         CPopulateMatrices Descend( const RpHAnimNodeInfo* pChildInfo )
         {
            RWS_FUNCTION("RWS:NULL::CPopulateMatrices::Descend")

            RWS_PRE( pChildInfo );
            RWS_PRE( CanDescend(pChildInfo) );

            // We're descending from a parent into a child, so move from the parent
            // bone matrix to the child bone matrix (assumes structures are identical)
            ++pBoneMatrix_;

            // Bone matrices are absolute within the skin (i.e. they're relative to
            // the root of the skin, not each other). So if Mc is a child bone's
            // modelling matrix (i.e. relative to its parent) and Bp and Bc are parent
            // and child bone matrices respectively, then Bc = Bp Mc and thus
            // Mc = Bp' Bc
            RwFrame* pDstFrame = pChildInfo->pFrame;
            RWS_ASSERT( pDstFrame, "No RwFrame" );
            RwMatrixInvert( RwFrameGetMatrix(pDstFrame), pBoneMatrix_ );
            RwFrameTransform( pDstFrame, &invParentBone_, rwCOMBINEPOSTCONCAT );
            // NB: no need for RwFrameUpdateObjects() - RwFrameTransform() will have
            // already done that

            const CPopulateMatrices result( *pBoneMatrix_, pBoneMatrix_ );
            RWS_POST( result.IsValid(pChildInfo) );

            RWS_RETURN( result );
         }

      private:
         const RwMatrix& invParentBone_;
         const RwMatrix*& pBoneMatrix_;
      };
   }

   /**
   *
   *  \ingroup Animation
   *
   *  \param hierarchy A reference to an RpHAnimHierarchy object.
   *  \param skin A reference to a RpSkin object.
   *  \return true if the RpHAnimHierarchy can be set with the skin's default pose
   *
   */
   bool HierarchyCanAcceptDefaultPose( const RpHAnimHierarchy& hierarchy, const RpSkin& skin )
   {
      RWS_FUNCTION("RWS::HierarchyCanAcceptDefaultPose")

      bool result = false;

      // Is the hierarchy internally consistent?
      if( HierarchyStructureIsValid(hierarchy) )
      {
         // Yes. Is it compatible with our skin?
         if( HierarchyIsCompatibleWithSkin(hierarchy, skin) )
         {
            // Yes. Does the hierarchy have local matrices?
            if( FlagsAreSet(hierarchy,rpHANIMHIERARCHYNOMATRICES) )
            {
               // No. We'll need to splurge the default pose into an RwFrame hierarchy. Do
               // we have one?
               if( hierarchy.parentFrame )
               {
                  // Yes. Would we be able to attach to it?
                  if( HierarchyIsAttachable( hierarchy, *hierarchy.parentFrame ) )
                  {
                     // Yes. That's all we need to know
                     result = true;
                  }
               }
            }
            else
            {
               // Yes. We can splurge the default pose directly into the matrices. No
               // further info required.
               result = true;
            }
         }
      }

      RWS_RETURN(result);
   }

   /**
   *  
   *  \ingroup Animation
   *
   *  Populates an RpHAnimHierarchy's matrix array or RwFrame hierarchy with 
   *  an RpSkin's default pose. Sets the 'local space matrices' flag when 
   *  populating the matrix array (don't confuse this with the 'no local 
   *  matrices' flag, it's subtly different - see RenderWare Graphics docs). 
   *  Attaches to and populates the RwFrame hierarchy when there's no matrix
   *  array.
   *
   *  \param hierarchy A reference to a RpHAnimHierarchy object.
   *  \param skin A reference to a RpSkin object.
   *
   */
   void ApplyDefaultPoseToHierarchy( RpHAnimHierarchy& hierarchy, const RpSkin& skin )
   {
      RWS_FUNCTION("RWS::ApplyDefaultPoseToHierarchy")

      RWS_PRE( HierarchyCanAcceptDefaultPose( hierarchy, skin ) );

      // If the hierarchy was created with no local matrices, we need to apply
      // the default pose to the RwFrame hierarchy instead
      if( FlagsAreSet(hierarchy,rpHANIMHIERARCHYNOMATRICES) )
      {
         // Attach the RpHAnimHierarchy to the RwFrame hierarchy - this should ensure that
         // each RpHAnimNodeInfo refers to a valid RwFrame (particularly in view of the
         // pre-conditi on)
         RpHAnimHierarchyAttach(&hierarchy);

         // Populate the RwFrames, starting with the root. Note that we set
         // modelling matrices, not LTMs (regardless of what happens to be flagged
         // in the hierarchy). If we just set the LTM's, the pose information would
         // be lost whenever an RwFrame near the root of the hierarchy was modified.
         const RpHAnimNodeInfo* pCurrNodeInfo = hierarchy.pNodeInfo;
         const RwMatrix* pBoneMatrix = RpSkinGetSkinToBoneMatrices(const_cast<RpSkin*>(&skin));
         RwFrame* pDstFrame = pCurrNodeInfo->pFrame;
         RWS_ASSERT( pDstFrame, "No RwFrame" );
         RwMatrixInvert( RwFrameGetMatrix(pDstFrame), pBoneMatrix );
         RwFrameUpdateObjects( pDstFrame );

         CPopulateMatrices descender( *pBoneMatrix, pBoneMatrix );
         ForEachInHierarchy( pCurrNodeInfo, descender );
      }
      else
      {
         // Set the 'local space matrices' flag - this ensures that
         RwUInt32 hierarchyFlags = RpHAnimHierarchyGetFlags(&hierarchy);

         RWS_TRACEIF((hierarchyFlags&rpHANIMHIERARCHYSUBHIERARCHY),"This hierarchy is a sub-hierarchy");
         RWS_TRACEIF((hierarchyFlags&rpHANIMHIERARCHYNOMATRICES),"This hierarchy has no local matrices");
         RWS_TRACEIF((hierarchyFlags&rpHANIMHIERARCHYUPDATEMODELLINGMATRICES),"This hierarchy updates modeling matrices");
         RWS_TRACEIF((hierarchyFlags&rpHANIMHIERARCHYUPDATELTMS),"This hierarchy updates LTMs");
         RWS_TRACEIF((hierarchyFlags&rpHANIMHIERARCHYLOCALSPACEMATRICES),"This hierarchy calculates matrices in a space relative to its root");

         hierarchyFlags |= rpHANIMHIERARCHYLOCALSPACEMATRICES;

         RpHAnimHierarchySetFlags(&hierarchy, RpHAnimHierarchyFlag(hierarchyFlags) );

         // Set the matrices
         InvertMatrices(
            RpSkinGetSkinToBoneMatrices(const_cast<RpSkin*>(&skin)),
            hierarchy.pMatrixArray,
            RpSkinGetNumBones(const_cast<RpSkin*>(&skin))
            );
      }

      RWS_RETURNVOID()
   }

   /**
   *
   *  \ingroup Animation
   *
   *  \param hierarchy A reference to a RpHAnimHierarchy object.
   *
   *  \return true if the RpHAnimHierarchy defines a valid frame structure, otherwise false.
   *
   */
   bool HierarchyStructureIsValid( const RpHAnimHierarchy& hierarchy )
   {
      RWS_FUNCTION("RWS::HierarchyStructureIsValid")

      bool isValid = false;

      // The structure is valid if we can 'swallow' the RpHAnimNodeInfo hierarchy with no 'leftovers'
      const RpHAnimNodeInfo* pCurrNodeInfo = hierarchy.pNodeInfo;
      const RpHAnimNodeInfo* pEndNodeInfo = pCurrNodeInfo + hierarchy.numNodes;
      if( pCurrNodeInfo < pEndNodeInfo && ConstForEachInHierarchy(pCurrNodeInfo, CIteratorIsWithinBounds(pEndNodeInfo)) )
      {
         isValid = (pCurrNodeInfo == pEndNodeInfo);
      }

      RWS_RETURN(isValid);
   }

   /**
   *
   *  \ingroup Animation
   *
   *  \param hierarchy A reference to a RpHAnimHierarchy object.
   *  \param rootFrame A reference to a RwFrame object, which is the root of the hierarchy.
   *
   *  \return true if every frame in the RpHAnimHierarchy could successfully be attached to
   *  a structurally equivalent counterpart in the RwFrame hierarchy, otherwise false.
   */
   bool HierarchyIsAttachable( const RpHAnimHierarchy& hierarchy, const RwFrame& rootFrame )
   {
      RWS_FUNCTION("RWS::HierarchyIsAttachable")

      RWS_PRE( HierarchyStructureIsValid(hierarchy) );

      bool isAttachable = false;

      // Find the RwFrame that is equivalent to the RpHAnimNodeInfo hierarchy root and
      // determine whether their descendant structures correspond
      const RpHAnimNodeInfo* pCurrNodeInfo = hierarchy.pNodeInfo;
      const RwFrame* pRootEquivalent = FindChildWithHAnimIDRecursive(rootFrame,pCurrNodeInfo->nodeID);
      if( pRootEquivalent )
      {
         isAttachable = ConstForEachInHierarchy( pCurrNodeInfo, CFrameIsAttachable(*pRootEquivalent) );
      }

      RWS_RETURN(isAttachable);
   }

   /**
   *
   *  \ingroup Animation
   *
   *  \param anim A reference to a RpHAnimAnimation object.
   *  \param skin A reference to a RpSkin object.
   *
   *  \return true if the RpHAnimHierarchy could successfully be used to drive the RpSkin
   *
   */
   bool HierarchyIsCompatibleWithSkin( const RpHAnimHierarchy& hierarchy, const RpSkin& skin )
   {
      RWS_FUNCTION("RWS::HierarchyIsCompatibleWithSkin")

      // The RpSkin API has been thinned out so all we can check now is that the number of nodes
      // in the hierarchy matches the number of bones in the skin
      bool result = (RpSkinGetNumBones(const_cast<RpSkin*>(&skin)) == RwUInt32(hierarchy.numNodes));

      RWS_RETURN(result);
   }

   /**
   *
   *  \ingroup Animation
   *
   *  \param anim A reference to a RpHAnimAnimation object.
   *  \param hierarchy A reference to a RpHAnimHierarchy object.
   *
   *  \return true if the RpHAnimAnimation could successfully be used to drive the RpHAnimHierarchy, otherwise
   *  false.
   *
   */
   bool AnimationIsCompatibleWithHierarchy( const RpHAnimAnimation& anim, const RpHAnimHierarchy& hierarchy )
   {
      RWS_FUNCTION("RWS::AnimationIsCompatibleWithHierarchy")

      // Count the number of keyframes in the RpHAnimAnimation with time == 0.0. This tells us
      // the total number of animation 'strands'...
      bool result = false;
      if(anim.interpInfo)
      {
// RenderWare Version less than 3.5
//
#if (rwLIBRARYCURRENTVERSION < 0x35000)
         const RwInt32 keyFrameSize = anim.interpInfo->keyFrameSize;
// RenderWare Version 3.5 and above
//
#elif (rwLIBRARYCURRENTVERSION >= 0x35000)
         const RwInt32 keyFrameSize = anim.interpInfo->animKeyFrameSize;
#endif

         const RwChar* pRawAnimData = const_cast<const RwChar*>(reinterpret_cast<RwChar*>(anim.pFrames)); // Assume keyframe size is in units of RwChar

         RwInt32 strandsCount = 0;  // A RwUInt32 would be better - however, hierarchy.numNodes is RwInt32

// RenderWare Version Precedes 3.4
//      
#if (rwLIBRARYCURRENTVERSION < 0x34002)
         while( RwReal(0) == reinterpret_cast<const RpHAnimKeyFrameHeader*>(pRawAnimData)->time )
#elif (rwLIBRARYCURRENTVERSION >= 0x34002)
// RenderWare Version Exceeds 3.4
//      
         while( RwReal(0) == reinterpret_cast<const RtAnimKeyFrameHeader*>(pRawAnimData)->time )
#endif
         {
            ++strandsCount;
            pRawAnimData += keyFrameSize;
         }

         // Compatible if the number of animation strands is the same as the number of hierarchy frames
         result = (strandsCount == hierarchy.numNodes);
      }

      RWS_RETURN(result);
   }
}