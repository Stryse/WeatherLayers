#ifndef LAYER__H
#define LAYER__H

#include <vector>

class WeatherCondition;

/// LAYER ABSTRACT BASE CLASS
class Layer
{
public:
        /// ========== Constructor, Destructor ========== ///
        explicit Layer(char type,double thickness) : type(type),thickness(thickness) {}
        virtual ~Layer() {}

        /// ========== Factory function -- return subclass based on @parameter type ========== ///
        static Layer* make(char type, double thickness);

        /// ========== Pure virtual functions ========== ///
        virtual void transmute(const WeatherCondition& condition) = 0;
        virtual void merge(std::vector<Layer*>& layers) {}

protected:
        char type;          // Layer type eg. Ozone,Oxygen,CarbonDioxide
        double thickness;   // In Km
};


/// OZONE LAYER
class Ozone : public Layer
{
public:
        /// ========== Constructor, Destructor ========== ///
        Ozone(double thickness = 0.5) : Layer('z',thickness){}
        virtual ~Ozone() {}

        /// ========== Overrides ========== ///
        void transmute(const WeatherCondition& condition) override;
};

/// OXYGEN LAYER
class Oxygen : public Layer
{
public:
        /// ========== Constructor, Destructor ========== ///
        Oxygen(double thickness = 0.5) : Layer('x',thickness){}
        virtual ~Oxygen() {}

        /// ========== Overrides ========== ///
        void transmute(const WeatherCondition& condition) override;
};

/// CARBON-DIOXIDE LAYER
class CarbonDioxide : public Layer
{
public:
        /// ========== Constructor, Destructor ========== ///
        CarbonDioxide(double thickness = 0.5) : Layer('s',thickness){}
        virtual ~CarbonDioxide() {}

        /// ========== Overrides ========== ///
        void transmute(const WeatherCondition& condition) override;
};

#endif