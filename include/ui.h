#ifndef _UI_
#define _UI_

// ================================ Imports ================================ //

#include "vex.h"
#include <math.h>

// ============================== UI (Headers) ============================== //

namespace ui
{
	enum Shape
	{
		Rect,
		Circle
	};

	/**
	 * @brief Base class for all UI elements
	 */
	class Element
	{
	protected:
		int xPos, yPos;
		int width, height;

	public:
		Element();
		~Element();
		virtual void render();
	};

	/**
	 * @brief TextLabel UI element
	 */
	class Textlabel : public Element
	{
	protected:
		std::string text;
		vex::fontType font;
		vex::color bgColor;
		vex::color color;

	public:
		/**
		 * @brief Construct a new Textlabel object
		 *
		 * @param t Text
		 * @param x X coordinate of origin
		 * @param y Y coordinate of origin
		 * @param ax X coordinate of anchor point
		 * @param ay Y coordinate of anchor point
		 */
		Textlabel(std::string t, int x, int y, double ax = 0, double ay = 0);

		/**
		 * @brief Set font type
		 * @param f Vex font type
		 */
		void setFont(vex::fontType f);

		/**
		 * @brief Change the text of the TextLabel
		 * @param s Text to change it to
		 */
		void setText(std::string s);

		/**
		 * @brief Set the background color
		 * @param c Vex color
		 */
		void setBackgroundColor(vex::color c);

		/**
		 * @brief Set the text color
		 * @param c Vex color
		 */
		void setColor(vex::color c);

		/**
		 * @brief Render element to screen
		 */
		void render() override;
	};

	/**
	 * @brief Button UI element
	 */
	class Button : public Element
	{
	protected:
		int shape;
		vex::color color;
		vex::color outline;
		std::string text;

	public:
		/**
		 * @brief Construct a new Button object
		 * @param x X coordinate of origin
		 * @param y Y coordinate of origin
		 * @param w Width of button
		 * @param h Height of button
		 * @param ax X coordinate of anchor point
		 * @param ay Y coordinate of anchor point
		 */
		Button(int x, int y, int w, int h, double ax = 0, double ay = 0);

		/**
		 * @brief Check if button is pressing
		 * @return boolean
		 */
		bool pressing();

		/**
		 * @brief Set text inside button
		 * @param s Text string
		 */
		void setText(std::string s);

		/**
		 * @brief Set the button color
		 * @param c Vex color
		 */
		void setColor(vex::color c);

		/**
		 * @brief Set the outline color
		 * @param c Vex color
		 */
		void setOutlineColor(vex::color c);

		/**
		 * @brief Render element to screen
		 */
		void render() override;
	};
}

// ========================================================================= //

#endif // _UI_