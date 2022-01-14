// ================================ Imports ================================ //

#include "vex.h"
#include <math.h>

// =============================== UI Wrapper =============================== //

namespace ui
{
	enum Shape
	{
		Rect,
		Circle
	};

	class Element
	{
	protected:
		int xPos, yPos;
		int width, height;

	public:
		Element() {}
		~Element() {}
		virtual void render() = 0;
	};

	class Textlabel : public Element
	{
	protected:
		std::string text;
		vex::fontType font = vex::fontType::mono20;
		vex::color bgColor = vex::color::transparent;
		vex::color color = vex::color::white;

	public:
		Textlabel(std::string t, int x, int y, double ax = 0, double ay = 0)
		{
			width = Brain.Screen.getStringWidth(t.c_str());
			height = Brain.Screen.getStringHeight(t.c_str());
			xPos = x - (width * ax);
			yPos = y - (height * ay);
			text = t;
		}
		~Textlabel() {}

		void setFont(vex::fontType f)
		{
			font = f;
		}

		void setText(std::string s)
		{
			text = s;
		}

		void setBackgroundColor(vex::color c)
		{
			bgColor = c;
		}

		void setColor(vex::color c)
		{
			color = c;
		}

		void render() override
		{
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
	};

	class Button : public Element
	{
	protected:
		int shape = Shape::Rect;
		vex::color color = vex::color(128, 128, 128);
		vex::color outline = vex::color(0, 0, 0);
		std::string text;

	public:
		Button(int x, int y, int w, int h, double ax = 0, double ay = 0)
		{
			xPos = x - (w * ax);
			yPos = y - (h * ay);
			width = w, height = h;
		}
		~Button() {}

		bool pressing()
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

		void setText(std::string s)
		{
			text = s;
		}

		void setColor(vex::color c)
		{
			color = c;
		}

		void setOutlineColor(vex::color c)
		{
			outline = c;
		}

		void render() override
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
	};
}
