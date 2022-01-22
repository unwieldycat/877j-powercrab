// ================================ Imports ================================ //

#include "ui.h"
#include "vex.h"
#include <math.h>

// =============================== UI Wrapper =============================== //

ui::Element::Element() {}
ui::Element::~Element() {}

ui::Textlabel::Textlabel(std::string t, int x, int y, double ax, double ay)
{
	color = vex::color(255, 255, 255);
	font = vex::fontType::mono20;
	width = Brain.Screen.getStringWidth(text.c_str());
	height = Brain.Screen.getStringHeight(text.c_str());
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

	// Get width and height
	width = Brain.Screen.getStringWidth(text.c_str());
	height = Brain.Screen.getStringHeight(text.c_str());

	// Draw text
	Brain.Screen.printAt(xPos - (width / 2), yPos + (height / 2), text.c_str());

	// Reset colors
	Brain.Screen.setFillColor(transparent);
	Brain.Screen.setPenColor(white);
	Brain.Screen.setFont(vex::fontType::mono20);
}
		
ui::Button::Button(int s, int x, int y, int w, int h, double ax, double ay)
{
	xPos = x - (w * ax);
	yPos = y - (h * ay);
	width = w, height = h;
	shape = s;
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
	Brain.Screen.setPenColor(outline);
	Brain.Screen.setFillColor(color);
	switch (shape)
	{
	case Shape::Rect:
		Brain.Screen.drawRectangle(
			xPos,
			yPos,
			width,
			height);
		break;
	case Shape::Circle:
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
		Brain.Screen.printAt(xPos + width / 2 - textOffsetX / 2, yPos + height / 2 + textOffsetY / 2, text.c_str());
	}
}
