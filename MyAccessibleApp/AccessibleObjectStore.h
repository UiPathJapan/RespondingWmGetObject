#pragma once


#include <Windows.h>


namespace UiPathTeam
{
    class AccessibleObject;
    class Element;


    class AccessibleObjectStore
    {
    public:

        static AccessibleObject* Get(Element*);
        static void Release(Element*);
    };
}
