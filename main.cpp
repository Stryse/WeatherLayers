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
    
    std::vector<Layer*> layers;
    // Populating layers
    int layerCount; ifs >> layerCount;
    for(int i = 0; i < layerCount; ++i)
    {
        char layerType;
        double layerThickness;
        ifs >> layerType >> layerThickness;
        layers.emplace_back(Layer::make(layerType,layerThickness));
    }

    std::vector<WeatherCondition*> conditions;
    // Populating atmosphere conditions
    for(ifs.get(); !ifs.fail(); ifs.get())
    {

    }
    ifs.close();

    
}

#else
#include "tests.h"
#endif