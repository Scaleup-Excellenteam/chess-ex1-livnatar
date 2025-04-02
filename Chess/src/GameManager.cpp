
#include "GameManager.h"


//------------------------------------------------------------------------

GameManager::GameManager() {};

/*
//------------------------------------------------------------------------
//constructor for when the file already exists

Controller::Controller(Board& board, int col, int windowLength)
	:m_board(board),
	m_window(sf::VideoMode(((1 + col)* PIXEL) + (3 * DISTANCE_ROW), (windowLength* PIXEL) + (2 * DISTANCE_ROW)), "Create level")
{
	updateVector();
	m_toolBar.setToolBar(m_icons);
}

//------------------------------------------------------------------------

Controller::Controller(int col, int row, int windowLength)
	: m_board(row, col), m_window(sf::VideoMode(((1 + col)* PIXEL) + (3 * DISTANCE_ROW), (windowLength* PIXEL) + (2 * DISTANCE_ROW)), "Create level")
{
	updateVector();
	m_toolBar.setToolBar(m_icons);

}
//-----------------------------------------------------------------------

void Controller::run(bool& levelExists, bool& pressX)
{
	ButtonType pressed = B_BLANK;	//saves the button we pressed on in the toolbar

	bool mouseExist = false;
	int xMouse = -1, yMouse = -1;		//intializing to default values

	if (levelExists)
	{
		mouseExist = true;
		m_board.findMouseCoords(xMouse, yMouse);
	}

	sf::Texture backgroundTexture;
	sf::Sprite background;
	setBackground(backgroundTexture, background);

	while (m_window.isOpen())
	{
		print(background, pressed);

		if (auto event = sf::Event{}; m_window.waitEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				m_window.close();
				pressX = true;
				break;
			case sf::Event::MouseButtonReleased:
				handleClick(event.mouseButton, pressed, levelExists, mouseExist, xMouse, yMouse);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------
//This function loads the background picture, creates the sprite, and
//sets it as the background of the window

void Controller::setBackground(sf::Texture& backgroundTexture, sf::Sprite& background)
{
	backgroundTexture.loadFromFile("landscape.jpg");
	background.setTexture(backgroundTexture);
	sf::Vector2u textureSize = backgroundTexture.getSize();

	// Scale the background sprite to fit the window
	background.setScale(static_cast<float>(m_window.getSize().x) / textureSize.x,
		static_cast<float>(m_window.getSize().y) / textureSize.y);
}

//----------------------------------------------------------------------

void Controller::print(sf::Sprite& background, ButtonType pressed)
{
	m_window.clear();
	m_window.draw(background);
	m_board.printBoard(m_window, m_icons);
	m_toolBar.printToolBar(m_window, pressed);

	if (pressed != B_SAVE && pressed != B_NEWPAGE && pressed != B_BLANK)
	{
		sf::RectangleShape showButton({ PIXEL,PIXEL });
		auto position = sf::Mouse::getPosition(m_window);
		showButton.setPosition({ 1.f * position.x, 1.f * position.y });
		showButton.setTexture(&m_icons[pressed]);
		showButton.setOrigin(showButton.getSize() / 2.f);	//centering the object to mouse
		showButton.setFillColor(sf::Color(255, 255, 255, 90));	//drawing the object half transparent
		m_window.draw(showButton);
	}

	m_window.display();
}
//----------------------------------------------------------------------

void Controller::updateVector()
{
	m_icons.resize(11);
	m_icons[B_SAVE].loadFromFile("save.jpg");
	m_icons[B_NEWPAGE].loadFromFile("newpage.jpg");
	m_icons[B_CAT].loadFromFile("cat.jpg");
	m_icons[B_MOUSE].loadFromFile("mouse.jpg");
	m_icons[B_KEY].loadFromFile("key.jpg");
	m_icons[B_GIFT].loadFromFile("gift.jpg");
	m_icons[B_CHEESE].loadFromFile("cheese.jpg");
	m_icons[B_DOOR].loadFromFile("door.jpg");
	m_icons[B_WALL].loadFromFile("wall.jpg");
	m_icons[B_ERASER].loadFromFile("eraser.jpg");
	m_icons[B_BLANK].loadFromFile("blank.jpg");
}

//------------------------------------------------------------------------

void Controller::handleClick(sf::Event::MouseButtonEvent& event, ButtonType& pressed,
	bool& levelExists, bool& mouseExist, int& xMouse, int& yMouse)
{
	auto location = m_window.mapPixelToCoords({ event.x,event.y });


	handleClickToolBar(levelExists, pressed, location);
	handleClickBoard(location, pressed, mouseExist, xMouse, yMouse);


}
//----------------------------------------------------------------------------
void Controller::handleClickToolBar(bool& levelExists, ButtonType& pressed, const sf::Vector2f& location)
{

	//checking if we're in the tool bar
	if (location.y >= DISTANCE_ROW && location.y <= DISTANCE_ROW + (TOOLBAR_SIZE * PIXEL) &&	//the range of Y in the toolbar
		location.x >= DISTANCE_ROW && location.x <= DISTANCE_ROW + PIXEL)						//the range of X in the toolbar
	{
		pressed = ButtonType((location.y - DISTANCE_ROW) / PIXEL);

		if (pressed == B_SAVE || pressed == B_NEWPAGE)
		{
			performPress(pressed, levelExists);
		}
	}

}
//---------------------------------------------------------------------------
void Controller::handleClickBoard(const sf::Vector2f& location, ButtonType& pressed,
	bool& mouseExist, int& xMouse, int& yMouse)
{
	//checking if we're in the board
	int col = m_board.getCol(),
		row = m_board.getRow();

	if (location.x > DISTANCE_COL && location.x < (DISTANCE_COL + (col * PIXEL)) &&				//the range of X in the board
		location.y > DISTANCE_ROW && location.y < (DISTANCE_ROW + (PIXEL * row)) &&				//the range of X in the board
		pressed != B_BLANK)																		//means we have a wanted texture to draw
	{
		int colBoard = (int)((location.x - DISTANCE_COL) / PIXEL),
			rowBoard = (int)((location.y - DISTANCE_ROW) / PIXEL);

		if (pressed == B_MOUSE && !mouseExist)
		{
			xMouse = colBoard;
			yMouse = rowBoard;
			mouseExist = true;
		}
		else if (pressed == B_MOUSE && mouseExist)
		{
			return;
		}
		if (pressed != B_MOUSE && pressed != B_BLANK && pressed != B_NEWPAGE &&
			pressed != B_SAVE && colBoard == xMouse && rowBoard == yMouse)
		{
			mouseExist = false;
		}

		m_board.setTileIcon(rowBoard, colBoard, pressed);
	}
}

//----------------------------------------------------------------------------
void Controller::performPress(ButtonType& pressed, bool& levelExists)
{
	std::ofstream board;

	if (pressed == B_SAVE)
	{
		board.open("Board.txt");

		if (board.is_open())
		{
			m_board.saveBoardAndCopyToText(board);
			levelExists = true;
		}
	}
	else if (pressed == B_NEWPAGE)
	{
		levelExists = false;
		std::remove("Board.txt");
		m_window.close();
		return;
	}
}
*/