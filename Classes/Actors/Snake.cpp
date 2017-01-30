#include "Snake.h"
//#include "Constant.h"
#include "cocos2d.h"
#include <functional>
#include "Constant.h"

//static const char *scheduleRunningBodyName = "runningBody";
static const char *scheduleRunningHeadName = "runningHead";
static const char *pathSnakeBlock = "circle.png";

static const int stepAddLength = 2;

USING_NS_CC;
static bool isFirstInit = true;

static const Vec2 DescriptionWay[] = { Vec2(-1,  0),
                                       Vec2( 1,  0),
                                       Vec2( 0,  1),
                                       Vec2( 0, -1),
                                       Vec2::ZERO };

using namespace std;

//void Snake::setRunningFunc(const std::function<void (float)> &func)
//{
//    runningFunc = func;
//}



Snake::Snake() : Layer(),
    directionSnakeHead(Direction::Zero),
    lenghtSnake(0),
    speedSnake(8),
    isMovingHeadSnake(false),
    colorBlockSnake(Color3B::WHITE),
    maxLengthSnake(5000),
    opponent(nullptr)
{
    if (isFirstInit) {
        TextureCache::getInstance()->addImage(pathSnakeBlock);
        isFirstInit = false;
    }

    snakeBlocks.reserve(128);

}

bool Snake::init()
{
    if (!Layer::init())
        return false;

    log("initialize Snake");

    addSnakeBlock(10);
    head = snakeBlocks[0];

    indexBackBlock = snakeBlocks.size()-1;

    schedule(schedule_selector(Snake::movingSnakeBody), updateTimeMSec);

    initPhysicsBodyHead();

    initLight();

    schedule([=](float){
        Snake::setSpeed(getSpeed() + 1);
        log("speed: %f", getSpeed());
    }, 20, "addSpeed");

    return true;
}

void Snake::movingSnakeBody(float f)
{
    if (indexBackBlock <= 0 || indexBackBlock >= snakeBlocks.size())
        indexBackBlock = snakeBlocks.size()-1;

    if (snakeBlocks.size())
        snakeBlocks[indexBackBlock]->setPosition(/*head->*/getPosition());

    indexBackBlock--;
}

void Snake::setName(const string &name)
{
    head->setName(name);
}

const string& Snake::getName() const
{
    return head->getName();
}

int Snake::getTag() const
{
    return head->getTag();
}

void Snake::setTag(int tag)
{
    head->setTag(tag);
}

void Snake::start()
{
    isMovingHeadSnake = true;
    schedule(schedule_selector(Snake::movingHead), 1.f / speedSnake);
}

void Snake::stop()
{
    isMovingHeadSnake = false;
    unschedule(schedule_selector(Snake::movingHead));
}

void Snake::setSpeed(float speed)
{
    if (speed >= MAX_SNAKE_SPEED)
        speedSnake = MAX_SNAKE_SPEED;

    else if (speed <= MIN_SNAKE_SPEED)
        speedSnake = MIN_SNAKE_SPEED;

    else speedSnake = speed;

    if (isMovingHeadSnake){
        stop();
        start();
    }
}

void Snake::setColor(const Color3B &color)
{
    colorBlockSnake = color;

    for (const auto &i : snakeBlocks)
        i->setColor(colorBlockSnake);
}

void Snake::setPosition(const Vec2 &newPosition)
{
    head->setPosition(newPosition);
}

const Vec2 &Snake::getPosition() const
{
    return head->getPosition();
}

void Snake::addSnakeBlock(size_t add)
{
    if ((lenghtSnake + add) > maxLengthSnake) {
        add = maxLengthSnake - lenghtSnake;
        if (add <= 0)
            return;
    }

    lenghtSnake += add;

    for (int i = 0; i < add*stepAddLength; i++) {

        auto block = createBlockSnake();
        block->setColor(colorBlockSnake);

        block->setOpacity(0xff * .10f);

        block->setCameraMask((unsigned int)CameraFlag::USER1);

        this->addChild(block);

        if (snakeBlocks.empty()) {
            block->setPosition(100, 100);
        }

        else block->setPosition(snakeBlocks[snakeBlocks.size()-1]->getPosition());

        snakeBlocks.push_back(block);

    }

}

bool Snake::isContainer(const Point &point, float maxDistance) const
{
    for (const auto &i : snakeBlocks)
        if (i->getPosition().distance(point) <= maxDistance)
            return true;

    return false;
}

void Snake::subSnake(int sublen)
{
    sublen *= stepAddLength;
    for (int i = snakeBlocks.size()-1; i > 0 && sublen-- > 0; i--){
        removeChild(snakeBlocks[i]);
        snakeBlocks.pop_back();
    }

}

void Snake::initLight()
{
    static int light_id = 1;

    Sprite *tmp = Sprite::create("light.png");

    tmp->setScale(0.35);
    tmp->setCameraMask((unsigned int)CameraFlag::USER1);

    addChild(tmp, LLight);

    schedule([=](float){
        tmp->setPosition(head->getPosition());
    }, updateTimeMSec, (std::string("snakeLight") + StringUtils::toString(light_id)).c_str());

    light_id++;
}

void Snake::initPhysicsBodyHead()
{
    // max supported snakes, at 32
    static int8_t count(0);

    auto body = PhysicsBody::createCircle(getHeadWidth() * 0.40, PhysicsMaterial(0, 0, 0));

    body->setMass(SnakeMass);
    body->setDynamic(true);
    body->setRotationEnable(false);

    body->setContactTestBitmask (TestBitmask::SnakeBitMask << count);
    body->setCollisionBitmask   (TestBitmask::SnakeBitMask << count);
    body->setCategoryBitmask    (TestBitmask::SnakeBitMask << count);

    head->setPhysicsBody(body);
    count++;
}

Sprite *Snake::createBlockSnake()
{
    return Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey(pathSnakeBlock));
}