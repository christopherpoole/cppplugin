C++ Shared Object Plugin
========================

A template for loading objects of type T from a dynamically loaded
library. The shared object, or plugin, must have entry and exit
functions designated extern "C" which manage the construction
and destruction of T.

For an advanced usage example have a look at G4Plugin.

Author:     Christopher M Poole
Email:      mail@christopherpoole.net


Basic Example Usage
-------------------
 
Target Object:

    class Thing;


User Plugin:

    extern "C" Thing* load() {
        Thing * thing = new Thing();
        return thing;
    }

    extern "C" void unload(Thing * thing) {
        delete thing;
    }


Application:

    Plugin<Thing> * plugin = new Plugin<Thing>("./shared.so",
                                               "load", "unload");
    Thing * thing = plugin->Load();

    /** use thing **/

    plugin->UnLoad();

