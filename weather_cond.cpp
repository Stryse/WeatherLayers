#include "weather_cond.h"

// =============== WEATHER_CONDITIONS ABSTRACT CLASS =============== //
WeatherCondition* WeatherCondition::make(char type)
{
    WeatherCondition* p = nullptr;
    if      ('z' == type)   p = Storm::instance();
    else if ('n' == type)   p = Sunny::instance();
    else if ('m' == type)   p = Other::instance();
    
    return p;
}

// =============== STORM =============== //

/// Overrides ///
void Storm::transformLayer(Ozone&,double&,Layer*&) const
{}

void Storm::transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new Ozone((1-OxygenConstant) * newThickness,true);
    newThickness *= OxygenConstant;
}

void Storm::transformLayer(CarbonDioxide&, double&,Layer*&) const
{}

/// Singleton related ///
Storm* Storm::instance_ = nullptr;
Storm* Storm::instance()
{
    if(instance_ == nullptr)
        instance_ = new Storm();
    
    return instance_;
}

void Storm::destroy()
{
    if(instance_ == nullptr)
        delete instance_;
}
// =============== Sunny =============== //

/// Overrides ///
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

/// Singleton related ///
Sunny* Sunny::instance_ = nullptr;
Sunny* Sunny::instance()
{
    if(instance_ == nullptr)
        instance_ = new Sunny();
    
    return instance_;
}

void Sunny::destroy()
{
    if(instance_ == nullptr)
        delete instance_;
}
// =============== Other =============== //

/// Overrides ///
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

/// Singleton related ///
Other* Other::instance_ = nullptr;
Other* Other::instance()
{
    if(instance_ == nullptr)
        instance_ = new Other();
    
    return instance_;
}

void Other::destroy()
{
    if(instance_ == nullptr)
        delete instance_;
}