#ifndef WEATHER_COND__H
#define WEATHER_COND__H

#include "layer.h"


/// ================================ WEATHER_CONDITION ABSTRACT BASE CLASS ========================================== ///
class WeatherCondition
{
public:

        // ============ Factory function -- return subclass based on @parameter type ============ //
        static WeatherCondition* make(char type);

        // ============ Constructors, Destructor ============ //
        WeatherCondition(const char* type,double zConst, double xConst, double sConst)
                :type(type),OzoneConstant(zConst),OxygenConstant(xConst), CarbonDioxideConstant(sConst)
        { } 
        virtual ~WeatherCondition() { }

        // ============ Pure virtual functions ============ //
        virtual void transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const = 0;
        virtual void transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const = 0;
        virtual void transformLayer(CarbonDioxide&,double& newThickness,Layer*& newLayer) const = 0;

        // ============ public data ============= // 
        const char* type;

protected:
        const double OzoneConstant;
        const double OxygenConstant;
        const double CarbonDioxideConstant;
};

/// ================================ SINGLETON CLASS ========================================== ///
class Storm : public WeatherCondition
{
public:
        // Singleton Interface
        static Storm* instance();
        static void destroy();

        // Overrides
        virtual ~Storm() { }
        void transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(CarbonDioxide&,double& newThickness,Layer*& newLayer) const override;
private:
        Storm() : WeatherCondition("Zivatar",1.0,0.5,1.0) { }
        static Storm* instance_;
};

/// ================================ SINGLETON CLASS ========================================== ///
class Sunny : public WeatherCondition
{
public:
        // Singleton Interface
        static Sunny* instance();
        static void destroy();

        // Overrides
        virtual ~Sunny() { }
        void transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(CarbonDioxide&,double& newThickness,Layer*& newLayer) const override;

private:
        Sunny() : WeatherCondition("Napos",1.0,0.95,0.95) { }
        static Sunny* instance_;
};

/// ================================ SINGLETON CLASS ========================================== ///
class Other : public WeatherCondition
{
public:
        // Singleton Interface
        static Other* instance();
        static void destroy();

        // Overrides
        virtual ~Other() { }
        void transformLayer(Ozone&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(Oxygen&,double& newThickness,Layer*& newLayer) const override;
        void transformLayer(CarbonDioxide&,double& newThickness,Layer*& newLayer) const override;
private:
        Other() : WeatherCondition("Mas",0.95,0.9,1.0) { }
        static Other* instance_;

};
#endif