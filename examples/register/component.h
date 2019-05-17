#include <map>
#include <string>


class Component
{
};


namespace component
{    
    Component* create(const std::string& name);
    void destroy(const Component* comp);

    // This namespace contains implementation details.
    namespace detail
    {
        template<class T>
        Component* createComponent()
        {
            return new T;
        }

        typedef std::map<std::string, Component* (*)()> ComponentRegistry;
        ComponentRegistry& getComponentRegistry();

        template<class T>
        struct RegistryEntry
        {
            RegistryEntry(const std::string& name)
            {
                ComponentRegistry& reg = getComponentRegistry();
                reg[name] = createComponent<T>;
            }
        };

    } // namespace detail

} // namespace component


#define COMPONENT_REGISTER(TYPE, NAME) \
    namespace { \
        static const component::detail::RegistryEntry<TYPE> reg_ent_##TYPE(NAME); \
    }