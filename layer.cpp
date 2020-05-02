#include "layer.h"
#include "weather_cond.h"

#include <iostream>

/// ============== LAYER ABSTRACT CLASS ============== ///
Layer* Layer::make(char type, double thickness, bool fromTransformation)
{
    Layer* p = nullptr;
    if      ('z' == type) p = new Ozone(thickness,fromTransformation);
    else if ('x' == type) p = new Oxygen(thickness,fromTransformation);
    else if ('s' == type) p = new CarbonDioxide(thickness,fromTransformation);
    return p;
}

/// ============== OZONE LAYER ============== ///
Layer* Ozone::transmute(const WeatherCondition& condition)
{
    Layer* newLayer = nullptr;
    condition.transformLayer(*this,*(&thickness),newLayer);
    return newLayer;        
}

/// ============== OXYGEN LAYER ============== ///
Layer* Oxygen::transmute(const WeatherCondition& condition)
{
    Layer* newLayer = nullptr;
    condition.transformLayer(*this,*(&thickness),newLayer);
    return newLayer;
}

/// ============== CARBON DIOXIDE ============== ///
Layer* CarbonDioxide::transmute(const WeatherCondition& condition)
{
    Layer* newLayer = nullptr;
    condition.transformLayer(*this,*(&thickness),newLayer);
    return newLayer;
}