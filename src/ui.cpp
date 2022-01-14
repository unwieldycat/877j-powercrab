// ================================ Imports ================================ //

#include "ui.h"
#include "vex.h"
#include <math.h>

// =============================== UI Wrapper =============================== //

ui::Element::Element() {}
ui::Element::~Element() {}

ui::Textlabel::Textlabel(std::string t, int x, int y, double ax, double ay)
{
	width = Brain.Screen.getStringWidth(t.c_str());
	height = Brain.Screen.getStringHeight(t.c_str());
	xPos = x - (width * ax);
	yPos = y - (height * ay);
	text = t;
}


ui::Textlabel::~Textlabel(){}

void ui::Textlabel::setFont(vex::fontType f)
{
	font = f;
}

void ui::Textlabel::setText(std::string s)
{
	text = s;
}

void ui::Textlabel::setBackgroundColor(vex::color c)
{
	bgColor = c;
}

void ui::Textlabel::setColor(vex::color c)
{
	color = c;
}

void ui::Textlabel::render() {
	// Set color of pen and fill
	Brain.Screen.setFillColor(bgColor);
	Brain.Screen.setPenColor(color);
	Brain.Screen.setFont(font);

	// Draw text
	Brain.Screen.printAt(xPos + width / 2 - width, yPos + height / 2 - height, text.c_str());

	// Reset colors
	Brain.Screen.setFillColor(transparent);
	Brain.Screen.setPenColor(white);
	Brain.Screen.setFont(vex::fontType::mono20);
}
		
ui::Button::Button(int x, int y, int w, int h, double ax, double ay)
{
	xPos = x - (w * ax);
	yPos = y - (h * ay);
	width = w, height = h;
}

ui::Button::~Button(){}

bool ui::Button::pressing()
{
	if (
		Brain.Screen.pressing() &&
		Brain.Screen.xPosition() >= xPos &&
		Brain.Screen.xPosition() < xPos + width &&
		Brain.Screen.yPosition() >= yPos &&
		Brain.Screen.yPosition() < yPos + height)
		return true;

	return false;
}

void ui::Button::setText(std::string s)
{
	text = s;
}

void ui::Button::setColor(vex::color c)
{
	color = c;
}

void ui::Button::setOutlineColor(vex::color c)
{
	outline = c;
}

void ui::Button::render()
{
	switch (shape)
	{
	case Shape::Rect:
		Brain.Screen.setPenColor(outline);
		Brain.Screen.setFillColor(color);
		Brain.Screen.drawRectangle(
			xPos,
			yPos,
			width,
			height);
		break;
	case Shape::Circle:
		Brain.Screen.setPenColor(outline);
		Brain.Screen.drawCircle(
			xPos,
			yPos,
			width,
			color);
		break;
	}
	if (text.length() > 0)
	{
		int textOffsetX = Brain.Screen.getStringWidth(text.c_str());
		int textOffsetY = Brain.Screen.getStringHeight(text.c_str());
		Brain.Screen.printAt(xPos + width / 2 - textOffsetX, yPos + height / 2 - textOffsetY, text.c_str());
	}
}
