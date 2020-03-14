#include "AccessibleObjectStore.h"
#include <map>
#include "AccessibleObject.h"
#include "SimpleLock.h"


using namespace UiPathTeam;


typedef std::map<Element*, AccessibleObject*> AccessibleObjectMap;
typedef std::pair<Element*, AccessibleObject*> AccessibleObjectEntry;


class InternalAccessibleObjectStore
    : private AccessibleObjectMap
{
public:

    InternalAccessibleObjectStore();
    AccessibleObject* Get(Element*);
    void Release(Element*);

private:

    SimpleLock::Type m_LockedBy;
};


InternalAccessibleObjectStore::InternalAccessibleObjectStore()
    : AccessibleObjectMap()
    , m_LockedBy(0)
{
}


AccessibleObject* InternalAccessibleObjectStore::Get(Element* pElement)
{
    AccessibleObject* pAccessible;
    SimpleLock lock(m_LockedBy);
    AccessibleObjectMap::const_iterator iter = find(pElement);
    if (iter != end())
    {
        pAccessible = iter->second;
    }
    else
    {
        pAccessible = new AccessibleObject(pElement);
        insert(AccessibleObjectEntry(pElement, pAccessible));
    }
    pAccessible->AddRef();
    return pAccessible;
}


void InternalAccessibleObjectStore::Release(Element* pElement)
{
    AccessibleObject* pAccessible;
    {
        SimpleLock lock(m_LockedBy);
        AccessibleObjectMap::iterator iter = find(pElement);
        if (iter != end())
        {
            pAccessible = iter->second;
            erase(iter);
        }
        else
        {
            return;
        }
    }
    pAccessible->Release();
}


static InternalAccessibleObjectStore s_store;


AccessibleObject* AccessibleObjectStore::Get(Element* pElement)
{
    return s_store.Get(pElement);
}


void AccessibleObjectStore::Release(Element* pElement)
{
    s_store.Release(pElement);
}
