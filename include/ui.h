// Written by Thurston Yates for team 877J
// 2021-2022

#include "vex.h"

namespace ui {
    enum Shape;

    class Button {
        public:
            Button(Shape s, int x, int y, int w, int h);
            bool pressing();
            void setAnchorPoint(double x, double y);
            void setColor(vex::color c);
            void setOutlineColor(vex::color c);
            void draw();
    };
}