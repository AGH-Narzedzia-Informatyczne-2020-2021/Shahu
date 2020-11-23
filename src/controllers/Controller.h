#ifndef SHAHU_CONTROLLER_H
#define SHAHU_CONTROLLER_H

namespace shahu {
    class Controller {
    public:
        virtual bool update() = 0;
        virtual bool render() = 0;
        virtual bool load() = 0;
    };
}

#endif //SHAHU_CONTROLLER_H
