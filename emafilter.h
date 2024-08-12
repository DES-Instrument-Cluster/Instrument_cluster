#ifndef EMAFILTER_H
#define EMAFILTER_H

class EmaFilter // Exponention Moving Average Filter
{
public:
    EmaFilter(float alpha);
    ~EmaFilter();

    float	getEma() const;
    void	setEma(float value);
    void    setAlpha(float value);
    float   calculateFilteredOutput(float value);


private:
    float	alpha;
    float	oneMinusAlpha;

    float	ema;
};

#endif // EMAFILTER_H
