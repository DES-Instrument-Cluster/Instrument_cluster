#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <deque>

class Filter : public QObject {
    Q_OBJECT

public:
    Filter(float emaAlpha, int smaWindowSize);

    Q_INVOKABLE float	calculateOutput(float value);

    Q_INVOKABLE bool	getIsFilterOn() const;

    Q_INVOKABLE void	setIsFilterOn(bool isOn);
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
