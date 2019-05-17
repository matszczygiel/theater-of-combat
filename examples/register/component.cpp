#include "component.h"
#include <string>

namespace component 
{
    namespace detail
    {
        ComponentRegistry& getComponentRegistry()
        {
            static ComponentRegistry reg;
            return reg;
        }
    }


    Component* create(const std::string& name)
    {
        detail::ComponentRegistry& reg = detail::getComponentRegistry();
        detail::ComponentRegistry::iterator it = reg.find(name);

        if (it == reg.end()) {
            // This happens when there is no component registered to this
            // name. Here I return a null pointer, but you can handle this
            // error differently if it suits you better.
            return 0;
        }

        return it->second();
    }

    void destroy(const Component* comp)
    {
        delete comp;
    }

} // namespace component