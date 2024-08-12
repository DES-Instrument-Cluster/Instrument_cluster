#include "emafilter.h"

EmaFilter::EmaFilter(float alpha) : alpha(alpha), oneMinusAlpha(1 - alpha), ema(0.0)
{}

EmaFilter::~EmaFilter()
{}

float EmaFilter::getEma() const
{
    return this->ema;
}

void EmaFilter::setEma(float value)
{
    this->ema = value;
}

void EmaFilter::setAlpha(float value)
{
    this->alpha = value;
    this->oneMinusAlpha = 1 - value;
}

float EmaFilter::calculateFilteredOutput(float value)
{
    this->ema = this->alpha * value + this->oneMinusAlpha * ema;

    return this->ema;
}
