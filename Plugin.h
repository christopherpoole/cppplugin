//////////////////////////////////////////////////////////////////////////
// License & Copyright
// ===================
// 
// Copyright 2011 Christopher M Poole
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// 
// C++ Shared Object Plugin 
// ========================
// 
// File:      Plugin.h
// 
// Author:    Christopher M Poole
// Email:     mail@christopherpoole.net
// Date:      30th December, 2011
//
// 
// Change Log
// ==========
// 
// 
//////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <dlfcn.h>


class PluginException {
  public:
    PluginException(const char * message="Exception whilst loading plugin.") {
        message_ = message;
    };

    const char* what() {
        return message_;
    };

  private:
    const char* message_;
};


template<class T>
class Plugin {
  public:
    Plugin(const char * shared_object,
           const char * entry_symbol,
           const char * exit_symbol)
    {
        plugin_ = NULL;
        target_ = NULL;
 
        plugin_ = dlopen(shared_object, RTLD_LAZY);
        const char * dlopen_error = dlerror();
        if (!plugin_) {
            throw PluginException(dlopen_error);
        }

        // Flush errors
        dlerror();

        *(void **) (&PluginEntry) = dlsym(plugin_, entry_symbol);
        const char * dlsym_error = dlerror();
        if (dlsym_error) {
            throw PluginException(dlsym_error);
        }

        *(void **) (&PluginExit) = dlsym(plugin_, exit_symbol);
        dlsym_error = dlerror();
        if (dlsym_error) {
            throw PluginException(dlsym_error);
        }
    };

    ~Plugin() {
        Unload();
        dlclose(plugin_);
    };
  
  public:  
    T* Load() {
        if (!target_) {
            target_ = PluginEntry();
        }
        return target_;
    };  

    void Unload() {
        if (target_) {
            PluginExit(target_);
        }
    }; 

  private:
    void * plugin_;
    T* target_;

    // Entry/Exit function pointers
    T* (*PluginEntry)();
    void (*PluginExit)(T*);      
};

