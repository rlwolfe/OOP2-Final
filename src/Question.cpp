#pragma once

#include "Question.h"

std::ifstream Question::m_questionText;
bool Question::m_nextLevel = false;
bool Question::m_gameRestart = false;
int Question::m_questionsAnswered = 0;

Question::Question(sf::View view, int level, int difficulty) : m_level(level), m_difficulty(difficulty)
{
	setQuestions();

	m_scroll.setSize(sf::Vector2f(SCROLL_WIDTH, SCROLL_HEIGHT));
	if (SCROLL_WIDTH * 2 < WINDOW_WIDTH && SCROLL_HEIGHT * 2 < WINDOW_HEIGHT)
		m_scroll.setScale(2, 2);
	m_scroll.setTexture(&SingleLoatData::getInstance()->get_texture(SCROLL_TEXTURE));

	for (int i = 0; i <= ANSWERS; i++)
	{
		if (i == 0) {
			if (m_scroll.getScale() == sf::Vector2f(1, 1))
				m_textT[i].setCharacterSize(m_scroll.getSize().x / 35);
			else
				m_textT[i].setCharacterSize(m_scroll.getSize().x / 15);
			m_textT[i].setFont(SingleLoatData::getInstance()->getFonts(RIDDLE_FONT));
			m_textT[i].setFillColor(sf::Color::White);
			m_textT[i].setString(m_riddle);
		}
		else {
			if (i == 1)
				m_textT[i].setFillColor(sf::Color::Red);
			else
				m_textT[i].setFillColor(sf::Color::White);

			if (m_scroll.getScale() == sf::Vector2f(1, 1))
				m_textT[i].setCharacterSize(m_scroll.getSize().x / 20);
			else
				m_textT[i].setCharacterSize(m_scroll.getSize().x / 10);

			m_textT[i].setFont(SingleLoatData::getInstance()->getFonts(ANSWER_FONT));
		}

		m_textT[i].setOutlineThickness(5);

	}
	setPosition(view);
}

void Question::setQuestions()
{
	m_textV.clear();

	if (m_gameRestart) {
		m_difficulty = 0;
		m_level = 1;
		m_gameRestart = false;
	}

	if (m_nextLevel) {
		m_riddle = "";
		return;
	}
	if (m_questionText.eof())
		throw std::runtime_error("\nException thrown:\nFile ended\n");

	std::string currentLine;
	std::getline(m_questionText, currentLine);
	m_answer = currentLine;

	for (int i = 0; i < ANSWERS + 1; i++) {
		std::getline(m_questionText, currentLine);
		m_textV.push_back(currentLine);
	}
	m_riddle = m_textV[0];

	std::random_device seed;
	std::default_random_engine eng{ seed() };
	std::uniform_int_distribution<int> dist{ 1, 4 };
	m_textV.insert(m_textV.begin() + dist(eng), m_answer);

	for (int i = 0; i < ANSWERS + 1; i++) {
		m_textT[i].setString(m_textV[i]);
	}
}

void Question::setPosition(sf::View view)
{
	for (int i = 0; i <= ANSWERS; i++)
	{
		if (m_scroll.getScale() == sf::Vector2f(1, 1))
			m_textT[i].setPosition(sf::Vector2f(view.sf::View::getCenter().x - (SCROLL_WIDTH / 2) + (SCROLL_WIDTH / 20),
			(view.sf::View::getCenter().y - (SCROLL_HEIGHT / 2)) + SCROLL_HEIGHT / (ANSWERS * 2) * (1.5 + i)));
		else
			m_textT[i].setPosition(sf::Vector2f(view.sf::View::getCenter().x - (m_scroll.getSize().x) + (m_scroll.getSize().x / 10),
			(view.sf::View::getCenter().y - (m_scroll.getSize().y)) + m_scroll.getSize().y / (ANSWERS) * (1.5 + i)));
	}
	if (m_scroll.getScale() == sf::Vector2f(1, 1))
		m_scroll.setPosition(view.sf::View::getCenter().x - (SCROLL_WIDTH / 2), view.sf::View::getCenter().y - (SCROLL_HEIGHT / 2));
	else
		m_scroll.setPosition(view.sf::View::getCenter().x - (m_scroll.getSize().x), view.sf::View::getCenter().y - (m_scroll.getSize().y));
}

void Question::setSelected(int i)
{
	m_selected = m_textT[i].getString();
}


void Question::checkAnswer()
{
	if (m_answer == m_selected)
		setAnswerCorrect();
}

void Question::setAnswerCorrect()
{
	m_difficulty++;
	m_questionsAnswered++;
	m_correct = true;
	resetAnswer();

	if (m_difficulty == DIFFICULTY) {
		m_nextLevel = true;
		m_difficulty = 0;
		m_level++;
	}
}

void Question::resetAnswer()
{
	m_correct = false;
	m_riddle = m_answer = "";

	for (int i = 0; i < ANSWERS + 1; i++)
		m_textV[i] = "";
}

void Question::newLevel()
{
	setKey();
	m_questionsAnswered = 0;
}

void Question::resetQuestions()
{
	newLevel();
	Fire::resetCount();
	m_gameRestart = true;
}

void Question::draw(sf::RenderWindow& window, int numImages)
{
	if (m_gameRestart)
		m_difficulty = 0;

	if (m_riddle == "" && m_difficulty < DIFFICULTY && !m_nextLevel)
		setQuestions();

	window.draw(m_scroll);
	for (int i = 0; i <= ANSWERS; i++)
	{
		window.draw(m_textT[i]);
	}
}

void Question::moveDown()
{
	if (m_selectedItem < ANSWERS)		//move down in menu
	{
		m_textT[m_selectedItem].setFillColor(sf::Color::White);
		m_selectedItem++;
		m_textT[m_selectedItem].setFillColor(sf::Color::Red);
	}
}

void Question::moveUp()
{
	if (m_selectedItem > 1)		//move up in menu
	{
		m_textT[m_selectedItem].setFillColor(sf::Color::White);
		m_selectedItem--;
		m_textT[m_selectedItem].setFillColor(sf::Color::Red);
	}
}
