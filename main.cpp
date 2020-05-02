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


void printState(const std::vector<std::unique_ptr<Layer>>& v,const std::string& iteration = "kezdoallapot",std::ostream& os = std::cout)
{
    os << "============== Legkor adatok " << iteration << " =============" << std::endl;
    os << "Legretegek szama: " << v.size() << " db" << std::endl;
    for(const auto& layer : v)
    {
        os << layer->getType() << "\t" << layer->getThickness() << std::endl; 
    }
    os << "=======================================================" << std::endl;
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

void create(std::istream& ifs,std::vector<std::unique_ptr<Layer>>& layers, std::vector<std::unique_ptr<WeatherCondition>>& conds)
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

void transformLayers(std::vector<std::unique_ptr<Layer>>& layers,const WeatherCondition& cond)
{
    for(size_t i = 0; i < layers.size(); ++i)
    {
        std::unique_ptr<Layer> newL(layers[i]->transmute(cond)); 
        if(newL)
        {
            layers.insert(layers.begin()+i,std::move(newL));
            ++i;
        }
    }
}



void mergeLayers(std::vector<std::unique_ptr<Layer>>& layers)
{
    for(std::vector<std::unique_ptr<Layer>>::reverse_iterator it = layers.rbegin(); it != layers.rend(); it++)
    {
        //if((*it)->)
    }
}

int main()
{
    // ============ GetFileName ============ //
    std::ifstream ifs;
    if(!getFile(ifs)) return 1;

    // ============ Main variables ============ //
    std::vector<std::unique_ptr<Layer>> layers;
    std::vector<std::unique_ptr<WeatherCondition>> conditions;

    // ============ Reading data and populating main variables ============ //
    create(ifs,layers,conditions);
    ifs.close();

    // ============ Print Start state ============ //
    printState(layers);

    // ============ Simulation ============ //
    for(size_t i = 0; i < 1; ++i) // TODO range based for
    {
        transformLayers(layers,*conditions[i]);
        //mergeLayers(layers);
        printState(layers,std::to_string(i+1) + std::string(".kor"));
    }
    
}

#else
#include "tests.h"
#endif