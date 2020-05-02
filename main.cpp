#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <string>

#include "layer.h"
#include "weather_cond.h"

#define NORMAL_MODE
#ifdef  NORMAL_MODE

int main()
{
    std::string filePath;
    std::cout << "Adja meg az input file nevet: "; std::cin >> filePath;

    std::ifstream ifs(filePath);
    if(ifs.fail())
    {
        std::cerr << "Hiba tortent a \"" << filePath << "\" megnyitasakor." << std::endl;
        return 1;
    }
    
    std::cout << "============== Legkor adatok ============" << std::endl;
    // ============ Populating layers ============ //
    std::vector<Layer*> layers;
    int layerCount; ifs >> layerCount;
    std::cout << "Legretegek szama: " << layerCount << " db" << std::endl;
    std::cout << "Legretegek: " << std::endl;
    for(int i = 0; i < layerCount; ++i)
    {
        char layerType;
        double layerThickness;
        ifs >> layerType >> layerThickness;
        layers.emplace_back(Layer::make(layerType,layerThickness));

        std::cout << layerType << "\t" << layerThickness << std::endl;
    }

    // ============ Populating atmosphere conditions ============ //
    std::vector<WeatherCondition*> conditions;
    std::string conditionRawData; ifs >> conditionRawData;
    std::cout << "Legkori viszonyok:" << std::endl;
    for(char c : conditionRawData)
    {
        conditions.emplace_back(WeatherCondition::make(c));
        std::cout << c;
    }
    std::cout << std::endl << "=========================================" << std::endl;
    ifs.close();


    for(auto& l : layers)
    {
        l->transmute(*conditions[0]);
        std::cout << l->getType() << "\t" << l->getThickness() << std::endl;
    }
}

#else
#include "tests.h"
#endif