#include "layer.h"
#include "weather_cond.h"

#include <iostream>

/// ============== LAYER ABSTRACT CLASS ============== ///
Layer* Layer::make(char type, double thickness)
{
    Layer* p = nullptr;
    if      ('z' == type) p = new Ozone(thickness);
    else if ('x' == type) p = new Oxygen(thickness);
    else if ('s' == type) p = new CarbonDioxide(thickness);
    return p;
}

/// ============== OZONE LAYER ============== ///
void Ozone::transmute(const WeatherCondition& condition)
{
    Layer* newLayer = nullptr;
    condition.transformLayer(*this,*(&thickness),newLayer);
    if(newLayer)
        std::cout << newLayer->getType() << "\t" << newLayer->getThickness() << std::endl;
}

/// ============== OXYGEN LAYER ============== ///
void Oxygen::transmute(const WeatherCondition& condition)
{
    Layer* newLayer = nullptr;
    condition.transformLayer(*this,*(&thickness),newLayer);
    if(newLayer)
        std::cout << newLayer->getType() << "\t" << newLayer->getThickness() << std::endl;
}

/// ============== CARBON DIOXIDE ============== ///
void CarbonDioxide::transmute(const WeatherCondition& condition)
{
    Layer* newLayer = nullptr;
    condition.transformLayer(*this,*(&thickness),newLayer);
    if(newLayer)
        std::cout << newLayer->getType() << "\t" << newLayer->getThickness() << std::endl;
}