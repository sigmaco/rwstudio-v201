#if !defined RW_OBSERVER_AND_TARGET_DEFINED
#define      RW_OBSERVER_AND_TARGET_DEFINED

template<class ParentClass, class ArgumentType> class RwObserverSubject ;

#include <assert.h>

// Note About linking 
// If you are having problems linking to these classes please checking 
// the following things
// 1. RW_PATTERN_EXPORT is defined in the dll exporting the class
// 2. RW_IMPLEMENT_OBSERVER_LINKING(className, ArgumentClass) is defined in the class deriving from the observer

// this turns off a template multiply instantiated warning - this is safe (apparantly) 
#pragma warning (disable : 4231)
#pragma warning (disable : 4660)

#if defined DECLSPECIFIER
    #undef DECLSPECIFIER
#endif

#if defined EXPIMP_TEMPLATE
    #undef EXPIMP_TEMPLATE
#endif 


#ifdef RW_PATTERN_EXPORT
     #    define DECLSPECIFIER __declspec(dllexport)
     #    define EXPIMP_TEMPLATE
     #else
     #    define DECLSPECIFIER __declspec(dllimport)
     #    define EXPIMP_TEMPLATE extern
#endif

template <class ObservedClass, class ArgumentType = void*> class DECLSPECIFIER RwObserver
{
// we're all friends here
friend class RwObserverSubject <ObservedClass, ArgumentType>;
    
public:

    // destructor tells all of my observee's that I'm being destroyed and no longer required 
    // update notifications
    virtual ~RwObserver( void )
    {
        // inform all of our observer targets that were going to be destroyed and no longer 
        // require updates
        for (RwVector<RwObserverSubject <ObservedClass, ArgumentType> *>::iterator i = m_ObservedTarget.begin(); i!=m_ObservedTarget.end(); i++)
        {
            assert((*i) && "pointer is NULL");
            (*i)->_RemoveObserver(this);
        }
    }
        
public:

    // this defines the interface to the observer
    // these methods should be over-ridden where required 
    // remember to remplace ObserverdClass with the correct name
    // i.e. an instance of RwObserver<MyClass> can implement 
    // NotifyObservedModified(MyClass *,..);
    // NotifyObservedDestroyed(MyClass *,...);

    virtual void NotifyObservedModified(ObservedClass *Observed, int ID, ArgumentType Data = 0){};
    virtual void NotifyObservedDestroyed(ObservedClass *Observed){};

    
    // this removes all of the observations were currently using
    virtual void RemoveAllObservations( void )
    {
        for (RwVector<RwObserverSubject <ObservedClass, ArgumentType> *>::iterator i = m_ObservedTarget.begin(); i!=m_ObservedTarget.end(); i++)
        {
            _RemoveObserved(static_cast<ObservedClass *>(*i));
        }
    }

private:

     // this is a private method called by the target class to
     // inform this class of it's destruction
     void _NotifyObservedDestroyed(ObservedClass *Observed)
     {
        //NotifyObservedDestroyed(Observed);
        
        RwVector<RwObserverSubject <ObservedClass, ArgumentType> *>::iterator FindIt =
            _STL::find(m_ObservedTarget.begin(), m_ObservedTarget.end(), Observed);
            
         if (FindIt != m_ObservedTarget.end())
         {
             m_ObservedTarget.erase(FindIt);
         }
     }

    // this is another private method used
     // by the target class to establish the link between the two
    void _Observe( ObservedClass *NewTarget)
    {
        m_ObservedTarget.push_back(NewTarget);

        assert(NewTarget && "pointer is NULL");
        NewTarget->_AddObserver(this);
    }

    // when we no longer wish to watch the class
        // we can call this
    virtual void _RemoveObserved(ObservedClass *Observed)
    {
        RwVector<RwObserverSubject <ObservedClass, ArgumentType> *>::iterator FindIt =
            _STL::find(m_ObservedTarget.begin(), m_ObservedTarget.end(), Observed);
        
        if (FindIt != m_ObservedTarget.end())
        {
            m_ObservedTarget.erase(FindIt);
        }

        assert(Observed && "pointer is NULL");

        Observed->_RemoveObserver(this);
    }

    
private:        // Data

    RwVector<RwObserverSubject <ObservedClass, ArgumentType> *> m_ObservedTarget;
};


// RwObserverSubject  Template
// When you wish to have a class observeable (i.e. will notify it's clients when it
// is changed, 
template<class ParentClass, class ArgumentType = void*> class DECLSPECIFIER RwObserverSubject 
{
public:
    friend class RwObserver<ParentClass, ArgumentType>;

    // let all of our observes know were about to die
    virtual ~RwObserverSubject ( void )
    {
        for (RwVector <RwObserver<ParentClass, ArgumentType> *>::iterator i = m_MyObservers.begin(); i != m_MyObservers.end(); i++)
        {
            assert((*i) && "pointer is NULL");
            (*i)->_NotifyObservedDestroyed(static_cast<ParentClass *>(this));
        }
    }

public:
    
    // this is used to notify observers that something has changed
    void Update(int ID = 0, ArgumentType Data = 0)
    {
        for (RwVector <RwObserver<ParentClass, ArgumentType> *>::iterator i = m_MyObservers.begin(); i != m_MyObservers.end(); i++)
        {
            assert((*i) && "pointer is NULL");
            (*i)->NotifyObservedModified(static_cast<ParentClass *>(this), ID, Data);
        }
    }

    // add an observer
    void AddObserver( RwObserver<ParentClass, ArgumentType> *Observer)
    {
        assert(Observer && "pointer is NULL");
        Observer->RwObserver<ParentClass, ArgumentType>::_Observe(static_cast<ParentClass *>(this));
    }

    // remove an observer
    void RemoveObserver(RwObserver<ParentClass, ArgumentType> *Observer)
    {
        assert(Observer && "pointer is NULL");
        Observer->RwObserver<ParentClass, ArgumentType>::_RemoveObserved(static_cast<ParentClass *>(this));

    }

private:

    // no implementation of this at the moment as I kinda figure that
    // an observer should have no authority as to who observes them
    void _RemoveObserver( RwObserver<ParentClass, ArgumentType> *OldObs)
    {
        RwVector <RwObserver<ParentClass, ArgumentType> *>::iterator FindIt =
            _STL::find(m_MyObservers.begin(), m_MyObservers.end(), OldObs);

        if (FindIt != m_MyObservers.end() )
        {
            m_MyObservers.erase(FindIt);
        }
    }

    // add an observer, internal version
    void _AddObserver(RwObserver<ParentClass, ArgumentType> *NewObs)
    {
        m_MyObservers.push_back(static_cast<RwObserver<ParentClass, ArgumentType> *>(NewObs));
    }

private:    // data 

    RwVector <RwObserver<ParentClass, ArgumentType> *> m_MyObservers;

};



#define RW_IMPLEMENT_OBSERVER_LINKING(ParentClass, ArgumentClass)\
    EXPIMP_TEMPLATE template class DECLSPECIFIER RwObserverSubject <ParentClass, ArgumentClass>;\
    EXPIMP_TEMPLATE template class DECLSPECIFIER RwObserver<ParentClass, ArgumentClass>;\
    EXPIMP_TEMPLATE template class DECLSPECIFIER RwVector<RwObserverSubject <ParentClass, ArgumentClass> *>;

#endif 
