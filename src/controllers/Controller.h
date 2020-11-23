#ifndef SHAHU_CONTROLLER_H
#define SHAHU_CONTROLLER_H

namespace shahu {
    class Controller {
    public:
        virtual double update() = 0;
        virtual double render() = 0;
        virtual void load() = 0;
    };
}

#endif //SHAHU_CONTROLLER_H
