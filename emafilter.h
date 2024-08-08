#ifndef EMAFILTER_H
#define EMAFILTER_H

class EmaFilter // Exponention Moving Average Filter
{
public:
    EmaFilter(float alpha);
    ~EmaFilter();

    float	getEma() const;
    void	setEma(float value);

    float	calculateFilteredOutput(float value);

private:
    const float	alpha;
    const float	oneMinusAlpha;

    float	ema;
};

#endif // EMAFILTER_H
