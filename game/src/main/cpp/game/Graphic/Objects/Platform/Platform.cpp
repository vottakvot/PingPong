#include "Platform.h"

Platform::Platform(float step,
                   float x,
                   float y,
                   float width,
                   float height,
                   GLuint textureID,
                   GLuint programID,
                   GLint positionAttr,
                   GLint textureAttr,
                   GLint transformationAttr,
                   GLfloat * verticesCoords,
                   GLfloat * textureCoords,
                   GLfloat * matrixCoords) : Object(step,
                                                    x,
                                                    y,
                                                    width,
                                                    height,
                                                    textureID,
                                                    programID,
                                                    positionAttr,
                                                    textureAttr,
                                                    transformationAttr,
                                                    verticesCoords,
                                                    textureCoords,
                                                    matrixCoords)
{
    LOGI("Platform::Platform()");
    m_nScore = 0;
}

Platform::~Platform() {
    LOGI("Platform::~Platform()");
}

Object::CROSS_SIDE Platform::collision(Object * object) {
    // Move and check collision, If true change m_fDX and do m_fStep back
    move();

    // Cross points
    std::vector<GLfloat> * crossPoint = getCrossPoints();

    // Left wall and down/up platform side
    if (Intersect::intersectSegments(&object->getRectangle()->left, &getRectangle()->down, crossPoint)) {
        if (Intersect::intersectSegments(&object->getRectangle()->left, &getRectangle()->up, crossPoint)) {
            if (crossPoint->size() >= 4) {
                setDx(-1.0f * m_fDX);
                move();
                return Object::LEFT;
            }
        }
    }

    crossPoint->clear();

    // Right wall and down/up platform side
    if (Intersect::intersectSegments(&object->getRectangle()->right, &getRectangle()->down, crossPoint)) {
        if (Intersect::intersectSegments(&object->getRectangle()->right, &getRectangle()->up, crossPoint)) {
            if (crossPoint->size() >= 4) {
                setDx(-1.0f * m_fDX);
                move();
                return Object::RIGHT;
            }
        }
    }

    crossPoint->clear();
    return Object::NONE;
}

Platform::REBOUND_AREA Platform::getRebound(float x, float y, float width) {
    // Find line of rectangle across with point
    for (int i = 0; i < 4; i++) {
        // If point belongs line
        if (Intersect::betweenLine(getRectangle()->iterateSides.at(i), x, y)) {
            GLfloat startX;

            if (getRectangle()->getCenter().y > 0) {
                startX = *getRectangle()->iterateSides.at(i)->x2;
            } else {
                startX = *getRectangle()->iterateSides.at(i)->x1;
            }

            if (Intersect::between(startX, startX + width, x)) {
                return Platform::LAST_LEFT;
            }

            if (Intersect::between(startX + width, startX + 2.0f * width, x)) {
                return Platform::LEFT;
            }

            if (Intersect::between(startX + 2.0f * width, startX + 3.0f * width, x)) {
                return Platform::CENTER;
            }

            if (Intersect::between(startX + 3.0f * width, startX + 4.0f * width, x)) {
                return Platform::RIGHT;
            }

            if (Intersect::between(startX + 4.0f * width, *getRectangle()->iterateSides.at(i)->x2, x)) {
                return Platform::LAST_RIGHT;
            }
        }
    }

    return Platform::NONE;
}