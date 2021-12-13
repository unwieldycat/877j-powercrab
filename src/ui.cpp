// Written by Thurston Yates for team 877J
// 2021-2022

#include "vex.h"
#include "ui.h"
#include <stdlib.h>
#include <math.h>

namespace ui {
    enum Shape { Rect, Circle };

    class Button {
        private:
            int shape;
            int xPos, yPos;
            int width, height;
            int anchorX, anchorY;
            vex::color color;
            vex::color outline;

        public:
            Button(Shape s, int x, int y, int w, int h) {
                shape = s;
                xPos = x, yPos = y;
                width = w, height = h;
            }

            bool pressing() {
                if (
                    Brain.Screen.pressing() &&
                    Brain.Screen.xPosition() >= xPos + (width * anchorX) &&
                    Brain.Screen.xPosition() < xPos + (width * anchorX) + width &&
                    Brain.Screen.yPosition() >= yPos + (height * anchorY) &&
                    Brain.Screen.yPosition() < yPos + (height * anchorY) + height
                ) return true;

                return false;
            }

            void setAnchorPoint(double x, double y) {
                anchorX = x, anchorY = y;
            }

            void setColor(vex::color c) {
                color = c;
            }

            void setOutlineColor(vex::color c) {
                outline = c;
            }

            void draw() {
                switch (shape) {
                    case Shape::Rect:
                        Brain.Screen.setPenColor(outline);
                        Brain.Screen.setFillColor(color);
                        Brain.Screen.drawRectangle(
                            xPos + (width * anchorX),
                            yPos + (height * anchorY), 
                            width - (width * anchorX),
                            height - (height * anchorY)
                        );
                        break;
                    case Shape::Circle:
                        Brain.Screen.setPenColor(outline);
                        Brain.Screen.drawCircle(
                            xPos + (width * anchorX),
                            yPos + (height * anchorY),
                            width - (width * anchorX), 
                            color
                        );
                        break;
                }
            }
    };
}