#include "filter.h"

Filter::Filter(float emaAlpha, int smaWindowSize)
    : isFilterOn(false)
    , emaAlpha(emaAlpha)
    , emaOneMinusAlpha(1.0f - emaAlpha)
    , ema(0.0f)
    , smaWindowSize(smaWindowSize)
    , smaSum(0.0f)
{}

float Filter::calculateFilteredOutput(float value)
{
    float filteredValue;

    filteredValue = this->calculateEma(value);
    filteredValue = this->calculateSma(filteredValue);

    return filteredValue;
}

bool Filter::getIsFilterOn()
{
    return this->isFilterOn;
}

void Filter::setIsFilterOn(bool inOn)
{
    this->isFilterOn = inOn;
}

void Filter::setEmaAlpha(float alpha)
{
    this->emaAlpha = alpha;
    this->emaOneMinusAlpha = 1.0f - alpha;
}

void Filter::setEma(float ema)
{
    this->ema = ema;
}

void Filter::setSmaWindowSize(int size)
{
    this->smaWindowSize = size;
}

float Filter::calculateEma(float value)
{
    this->ema = this->emaAlpha * value + this->emaOneMinusAlpha * ema;

    return this->ema;
}

float Filter::calculateSma(float value)
{
    smaWindow.push_back(value);
    this->smaSum += value;

    if (smaWindow.size() > this->smaWindowSize) {
        smaSum -= smaWindow.front();
        smaWindow.pop_front();
    }

    return this->smaSum / this->smaWindow.size();
}
