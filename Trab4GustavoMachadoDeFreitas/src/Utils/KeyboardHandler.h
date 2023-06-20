#ifndef KEYBOARDHANDLER_H_INCLUDED
#define KEYBOARDHANDLER_H_INCLUDED

#include <functional>
#include <unordered_map>

using namespace std;

// Classe que gerencia os callbacks de teclado
class KeyboardHandler
{
public:
    KeyboardHandler(void)
    {
    }

    void RegisterCallbacks(int key, function<void()> pressCallback, function<void()> releaseCallback)
    {
        pressCallbacks[key] = pressCallback;
        releaseCallbacks[key] = releaseCallback;
    }

    void KeyPressed(int key)
    {
        auto entry = pressCallbacks.find(key);
        if (entry != pressCallbacks.end())
        {
            if (entry->second != nullptr)
            {
                entry->second();
            }
        }
    }

    void KeyReleased(int key)
    {
        auto entry = releaseCallbacks.find(key);
        if (entry != releaseCallbacks.end())
        {
            if (entry->second != nullptr)
            {
                entry->second();
            }
        }
    }

private:
    unordered_map<int, function<void()>> pressCallbacks;
    unordered_map<int, function<void()>> releaseCallbacks;
};

#endif // KEYBOARDHANDLER_H_INCLUDED
