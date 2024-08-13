#ifndef FILTER_H
#define FILTER_H

#include <deque>

class Filter {
public:
    Filter(float emaAlpha, int smaWindowSize);

    float	calculateOutput(float value);

    bool	getIsFilterOn();

    void	setIsFilterOn(bool isOn);
    void	setEma(float ema);
    void	setSmaWindowSize(int size);

private:
    float	calculateEma(float value);
    float	calculateSma(float value);

    bool	isFilterOn;

    float	emaAlpha;
    float	emaOneMinusAlpha;
    float	ema;

    int					smaWindowSize;
    float				smaSum;
    std::deque<float>	smaWindow;
};

#endif // FILTER_H
