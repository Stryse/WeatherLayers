#include "weather_cond.h"

// =============== STORM =============== //
void Storm::transformLayer(Ozone&,double&,Layer*&) const
{}

void Storm::transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new Ozone((1-OxygenConstant) * newThickness);
    newThickness *= OxygenConstant;
}

void Storm::transformLayer(CarbonDioxide&, double&,Layer*&) const
{}

// =============== Sunny =============== //

void Sunny::transformLayer(Ozone&,double&,Layer*&) const
{}

void Sunny::transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new Ozone((1-OxygenConstant) * newThickness);
    newThickness *= OxygenConstant;
}

void Sunny::transformLayer(CarbonDioxide&, double& newThickness,Layer*& newLayer) const
{
    newLayer = new Oxygen((1-CarbonDioxideConstant) * newThickness);
    newThickness *= CarbonDioxideConstant;
}

// =============== Other =============== //

void Other::transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new Oxygen((1-OzoneConstant) * newThickness);
    newThickness *= OzoneConstant;
}

void Other::transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const
{
    newLayer = new CarbonDioxide((1-OxygenConstant) * newThickness);
    newThickness *= OxygenConstant;
}

void Other::transformLayer(CarbonDioxide&, double&,Layer*&) const
{}