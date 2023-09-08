#ifndef __RENGRAPHNOTIFYOBJECT_H_
#define __RENGRAPHNOTIFYOBJECT_H_

struct IRENGraphItem;
struct IRENNodeItem;
struct IRENPathItem;

/** 
*  \ingroup RENGraphObjects 
*  \class CRENGraphNotifyObject
*  \brief Implement this class to handle modifications to graphs in your application.
*/
class CRENGraphNotifyObject
{
public:
   /**    
   *  A node has been added to a graph.
   * \param pGraph  An IDispatch pointer to the graph to which the node has been added.
   * \param pNode   An IDispatch pointer to the node which has been added.
   */                
   virtual void NodeAdded(IRENGraphItem *pGraph, IRENNodeItem *pNode) = 0;
   /**    
   *  A node has been removed from a graph.
   * \param pGraph  An IDispatch pointer to the graph to which the node has been removed.
   * \param pNode   An IDispatch pointer to the node which has been removed.
   */                
   virtual void NodeRemoved(IRENGraphItem *pGraph, IRENNodeItem *pNode) = 0;
   /**    
   *  A node has been selected to a graph.
   * \param pGraph  An IDispatch pointer to the graph to which the node has been selected.
   * \param pNode   An IDispatch pointer to the node which has been selected.
   */                
   virtual void NodeSelected(IRENGraphItem *pGraph, IRENNodeItem *pNode) = 0;
   /**    
   *  A node has been unselected in a graph.
   * \param pGraph  An IDispatch pointer to the graph to which the node has been unselected.
   * \param pNode   An IDispatch pointer to the node which has been unselected.
   */                
   virtual void NodeUnselected(IRENGraphItem *pGraph, IRENNodeItem *pNode) = 0;
   /**    
   *  The transformation matrix of a node has been updated.
   * \param pGraph  An IDispatch pointer to the graph to which the node has been modified.
   * \param pNode   An IDispatch pointer to the node which has been added.
   */                
   virtual void NodeUpdated(IRENGraphItem *pGraph, IRENNodeItem *pNode) = 0;
   /**    
   *  A node has been added to a path.
   * \param pGraph  An IDispatch pointer to the graph containing the path.
   * \param pPath   An IDispatch pointer to the path to which the node has been added.
   * \param pNode   An IDispatch pointer to the node which has been added.
   */                
   virtual void NodeAddedToPath(IRENGraphItem *pGraph, IRENPathItem *pPath, IRENNodeItem *pNode) = 0;
   /**    
   *  A node has been removed from a path.
   * \param pGraph  An IDispatch pointer to the graph containing the path.
   * \param pPath   An IDispatch pointer to the path to which the node has been removed.
   * \param pNode   An IDispatch pointer to the node which has been removed.
   */                
   virtual void NodeRemovedFromPath(IRENGraphItem *pGraph, IRENPathItem *pPath, IRENNodeItem *pNode) = 0;

   /**    
   *  A path has been added to a graph.
   * \param pGraph  An IDispatch pointer to the graph to which the path has been added.
   * \param pPath   An IDispatch pointer to the path which has been added.
   */                
   virtual void PathAdded(IRENGraphItem *pGraph, IRENPathItem *pPath) = 0;
   /**    
   *  A path has been removed from a graph.
   * \param pGraph  An IDispatch pointer to the graph to which the path has been removed.
   * \param pPath   An IDispatch pointer to the path which has been removed.
   */                
   virtual void PathRemoved(IRENGraphItem *pGraph, IRENPathItem *pPath) = 0;

   /**    
   *  A path has been selected to a graph.
   * \param pGraph  An IDispatch pointer to the graph to which the path has been selected.
   * \param pPath   An IDispatch pointer to the path which has been selected.
   */                
   virtual void PathSelected(IRENGraphItem *pGraph, IRENPathItem *pPath) = 0;
   /**    
   *  A path has been unselected in a graph.
   * \param pGraph  An IDispatch pointer to the graph to which the path has been unselected.
   * \param pPath   An IDispatch pointer to the path which has been unselected.
   */                
   virtual void PathUnselected(IRENGraphItem *pGraph, IRENPathItem *pPath) = 0;
};

#endif
