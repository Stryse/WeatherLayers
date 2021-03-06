#ifndef LAYER__H
#define LAYER__H

#include <vector>
#include <string>

class WeatherCondition;

/// LAYER ABSTRACT BASE CLASS
class Layer
{
public:
        /// ========== Constructor, Destructor ========== ///
        explicit Layer(const std::string& type,double thickness, bool fromTransformation = false) 
            : type(type),thickness(thickness), fromTransformation(fromTransformation) {}
        virtual ~Layer() {}

        /// ========== Factory function -- return subclass based on @parameter type ========== ///
        static Layer* make(char type, double thickness, bool fromTransformation = false);

        /// ========== Non-Const Member function ========== ///
        void merge(Layer* other);    // Thickness of layers added
        void solidify();             // Layer is no longer freshly transformed

        /// ========== Pure virtual functions ========== ///
        virtual Layer* transmute(const WeatherCondition& condition) = 0;

        /// ========== Getters ========== ///
        std::string getType() const { return type; }
        double getThickness() const { return thickness; }
        bool getFromTransformation() const { return fromTransformation; }

protected:
        std::string type;            // Layer type eg. Ozone,Oxygen,CarbonDioxide
        double thickness;            // In Km
        bool fromTransformation;     // Layer created from another layer's transformation
};


/// OZONE LAYER
class Ozone : public Layer
{
public:
        /// ========== Constructor, Destructor ========== ///
        Ozone(double thickness = 0.5, bool fromTransformation = false) 
                : Layer("Ozon",thickness,fromTransformation) { }
        virtual ~Ozone() {}

        /// ========== Overrides ========== ///
        Layer* transmute(const WeatherCondition& condition) override;
};

/// OXYGEN LAYER
class Oxygen : public Layer
{
public:
        /// ========== Constructor, Destructor ========== ///
        Oxygen(double thickness = 0.5, bool fromTransformation = false) 
                : Layer("Oxigen",thickness,fromTransformation) { }
        virtual ~Oxygen() {}

        /// ========== Overrides ========== ///
        Layer* transmute(const WeatherCondition& condition) override;
};

/// CARBON-DIOXIDE LAYER
class CarbonDioxide : public Layer
{
public:
        /// ========== Constructor, Destructor ========== ///
        CarbonDioxide(double thickness = 0.5, bool fromTransformation = false)
                : Layer("Szendioxid",thickness,fromTransformation) { }
        virtual ~CarbonDioxide() {}

        /// ========== Overrides ========== ///
        Layer* transmute(const WeatherCondition& condition) override;
};

#endif