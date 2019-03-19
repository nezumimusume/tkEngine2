#include "PlayerView.h"
#include "ViewConstants.h"

using namespace cocos2d;
using namespace ExitGames::Common;

PlayerView::PlayerView(int number, const JString& name, int cellSize, float borderWidth)
	: number(number)
	, name(name)
	, cellSize(cellSize)
	, borderWidth(borderWidth)
	, x(0)
	, y(0)
	, label(NULL)
	, opacity(255)
{
	updateView(cellSize);
	setPos(0, 0);
	setColor(ccBLUE);
}

const JString& PlayerView::getName(void)
{
	return name;
}

void PlayerView::setPos(int x, int y)
{
	this->x = x;
	this->y = y;
	setPosition(ccp(x*cellSize, -y*cellSize));
}

void PlayerView::setColor(int color)
{
	setColor(ccc3(color >> 0x10, color >> 8, color));
}

void PlayerView::setColor(const ccColor3B& color)
{
	this->color = color;
	sprite->setColor(color);
}

int PlayerView::getColor(void)
{
	return (color.r<<16) | (color.g<<8) | color.b;
}

void PlayerView::setNumber(int number)
{
	this->number = number;
}

void PlayerView::fadeOpacity(float v)
{
	opacity -= v * 256;
	if(opacity < 0) opacity = 0;
	sprite->setOpacity((GLbyte)opacity);
	label->setOpacity((GLbyte)opacity);
}

void PlayerView::resetOpacity(void)
{
	opacity = 255;
	sprite->setOpacity((GLbyte)opacity);
	label->setOpacity((GLbyte)opacity);
}

int PlayerView::getX(void)
{
	return x;
}

int PlayerView::getY(void)
{
	return y;
}

int PlayerView::getNumber(void)
 {
	 return number;
}

void PlayerView::updateView(int size)
{
	removeAllChildrenWithCleanup(true);
	cellSize = size;
	sprite = CCSprite::create();
	sprite->setTextureRect(CCRectMake(0, 0, cellSize, cellSize));
	sprite->setAnchorPoint(ccp(0, 1));
	addChild(sprite);

	if(borderWidth)
	{
		CCDrawNode* b = CCDrawNode::create();
		CCPoint pp[] = {ccp(0, 0), ccp(cellSize, 0), ccp(cellSize, -cellSize), ccp(0, -cellSize)};
		b->drawPolygon(pp, 4, TRANSPARENT_COLOR, (float)borderWidth, BLACK_COLOR);
		addChild(b);
	}

	label = CCLabelBMFont::create((name + "/" + number).UTF8Representation().cstr(), "arial20.fnt");
	label->setAnchorPoint(ccp(0, 1));
	label->setColor(ccBLACK);
	addChild(label);
	setColor(color);
}