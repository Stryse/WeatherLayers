#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <cstdlib>

#include "layer.h"
#include "weather_cond.h"

#define NORMAL_MODE
#ifdef  NORMAL_MODE


void printState(const std::vector<const Layer*>& v, std::ostream& os = std::cout)
{
    std::cout << "============== Legkor adatok ============" << std::endl;
    std::cout << "Legretegek szama: " << v.size() << " db" << std::endl;
    for(auto& layer : v)
    {
        os << layer->getType() << "\t" << layer->getThickness() << std::endl; 
    }
}

bool getFile(/*out*/ std::ifstream& ifs, std::istream& is = std::cin)
{
    std::cout << "Adja meg az input file nevet: ";
    std::string filePath; is >> filePath;
    ifs.open(filePath);
    if(ifs.fail())
    {
        std::cerr << "Hiba tortent a \"" << filePath << "\" megnyitasakor." << std::endl;
        return false;
    }
    return true;
}

void create(std::istream& ifs,std::vector<Layer*>& layers, std::vector<WeatherCondition*>& conds)
{
    // ============ Populating layers ============ //
    int layerCount; ifs >> layerCount;
    for(int i = 0; i < layerCount; ++i)
    {
        char layerType;
        double layerThickness;
        ifs >> layerType >> layerThickness;
        layers.emplace_back(Layer::make(layerType,layerThickness));
    }

    // ============ Populating atmosphere conditions ============ //
    std::string conditionRawData; ifs >> conditionRawData;
    for(char c : conditionRawData)
        conds.emplace_back(WeatherCondition::make(c));
}

int main()
{
    // ============ GetFileName ============ //
    std::ifstream ifs;
    if(!getFile(ifs)) return 1;

    // ============ Main variables ============ //
    std::vector<Layer*> layers;
    std::vector<WeatherCondition*> conditions;

    // ============ Reading data and populating main variables ============ //
    create(ifs,layers,conditions);
    ifs.close();

    // ============ Simulation ============ //
    for(size_t i = 0; i < layers.size(); ++i)
    {
        if(Layer* newL = layers[i]->transmute(*conditions[0]))
        {
            layers.insert(layers.begin()+i,newL);
            ++i;
        }
    }

    for(auto& l : layers)
        std::cout << l->getType() << "\t" << l->getThickness() << std::endl;
}

#else
#include "tests.h"
#endif