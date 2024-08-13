#include "filter.h"

Filter::Filter(float emaAlpha, int smaWindowSize)
    : isFilterOn(false)
    , emaAlpha(emaAlpha)
    , emaOneMinusAlpha(1.0f - emaAlpha)
    , ema(0.0f)
    , smaWindowSize(smaWindowSize)
    , smaSum(0.0f)
{}

float Filter::calculateOutput(float value)
{
    float filteredValue;

    // always store calculated values whether the filter is on or not
    // to not make the speedometer needle jumps when we switch the filter while driving
    filteredValue = this->calculateEma(value);
    filteredValue = this->calculateSma(filteredValue);

    if (this->isFilterOn)
    {
        return filteredValue;
    }
    return value;
}

bool Filter::getIsFilterOn()
{
    return this->isFilterOn;
}

void Filter::setIsFilterOn(bool inOn)
{
    this->isFilterOn = inOn;
}

void Filter::setEma(float ema)
{
    this->ema = ema;
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
