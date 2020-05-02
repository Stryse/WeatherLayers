#include "weather_cond.h"

// =============== WEATHER_CONDITIONS ABSTRACT CLASS =============== //
WeatherCondition* WeatherCondition::make(char type)
{
    WeatherCondition* p = nullptr;
    if      ('z' == type)   p = new Storm();
    else if ('n' == type)   p = new Sunny();
    else if ('m' == type)   p = new Other();
    
    return p;
}

// =============== STORM =============== //
void Storm::transformLayer(Ozone&,double&,Layer*&) const
{}

void Storm::transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new Ozone((1-OxygenConstant) * newThickness,true);
    newThickness *= OxygenConstant;
}

void Storm::transformLayer(CarbonDioxide&, double&,Layer*&) const
{}

// =============== Sunny =============== //

void Sunny::transformLayer(Ozone&,double&,Layer*&) const
{}

void Sunny::transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new Ozone((1-OxygenConstant) * newThickness,true);
    newThickness *= OxygenConstant;
}

void Sunny::transformLayer(CarbonDioxide&, double& newThickness,Layer*& newLayer) const
{
    newLayer = new Oxygen((1-CarbonDioxideConstant) * newThickness,true);
    newThickness *= CarbonDioxideConstant;
}

// =============== Other =============== //

void Other::transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new Oxygen((1-OzoneConstant) * newThickness,true);
    newThickness *= OzoneConstant;
}

void Other::transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new CarbonDioxide((1-OxygenConstant) * newThickness,true);
    newThickness *= OxygenConstant;
}

void Other::transformLayer(CarbonDioxide&, double&,Layer*&) const
{}