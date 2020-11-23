#ifndef SHAHU_VIEW_H
#define SHAHU_VIEW_H

namespace shahu {
    class View {
    public:
        virtual bool render() = 0;
        virtual bool load() = 0;
    };
}

#endif //SHAHU_VIEW_H
