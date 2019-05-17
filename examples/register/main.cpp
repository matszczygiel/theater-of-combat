#include "component.h"
// Every time you write a new component class you have to register it.
// For that you use the `COMPONENT_REGISTER` macro.
class RenderingComponent : public Component
{
    // Bla, bla
};
COMPONENT_REGISTER(RenderingComponent, "RenderingComponent")

int main()
{
    // To then create an instance of a registered component all you have
    // to do is call the `create` function like so...
    Component* comp = component::create("RenderingComponent");

    // I found that if you have a special `create` function that returns a
    // pointer, it's best to have a corresponding `destroy` function
    // instead of using `delete` directly.
    component::destroy(comp);
}