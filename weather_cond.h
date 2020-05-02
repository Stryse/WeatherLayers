#ifndef WEATHER_COND__H
#define WEATHER_COND__H

#include "layer.h"

class WeatherCondition
{
public:
        virtual void transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const = 0;
        virtual void transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const = 0;
        virtual void transformLayer(CarbonDioxide&,double& newThickness,Layer*& newLayer) const = 0;

protected:
        const double OzoneConstant;
        const double OxygenConstant;
        const double CarbonDioxideConstant;

        WeatherCondition(double zConst, double xConst, double sConst); 
};

class Storm : public WeatherCondition
{
public:
        Storm() : WeatherCondition(1.0,0.5,1.0){}
        void transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(CarbonDioxide&,double& newThickness,Layer*& newLayer) const override;
};

class Sunny : public WeatherCondition
{
public:
        Sunny() : WeatherCondition(1.0,0.95,0.95){}
        void transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(CarbonDioxide&,double& newThickness,Layer*& newLayer) const override;
};

class Other : public WeatherCondition
{
public:
        Other() : WeatherCondition(0.95,0.9,1.0){}
        void transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(CarbonDioxide&,double& newThickness,Layer*& newLayer) const override;
};
#endif