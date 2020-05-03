#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <iomanip>

#include "layer.h"
#include "weather_cond.h"

#define NORMAL_MODE
#ifdef  NORMAL_MODE

#define THICKNESS_LIMIT 0.5


void printState(const std::vector<std::unique_ptr<Layer>> & v,const std::string & iteration = "kezdoallapot",const std::string& condition = "",std::ostream & os = std::cout)
{
	os << "==================== Legkor adatok " << iteration << " ==================" << std::endl << std::endl;
	os << "Legretegek szama: " << v.size() << " db" << std::endl;
    os << ((condition != "") ? (std::string("Idojaras viszony: ") + condition) : "") << std::endl << std::endl;  
	for (const auto& layer : v)
	{
		os << std::setw(17) << layer->getType() << std::setw(17) << layer->getThickness() << std::setw(10) << (layer->getFromTransformation() ? "Friss" : "") << std::endl;
	}
	os << "==================================================================================" << std::endl;
}

bool getFile(/*out*/ std::ifstream& ifs, std::istream& is = std::cin)
{
	std::cout << "Adja meg az input file nevet: ";
	std::string filePath; is >> filePath;
	ifs.open(filePath);
	if (ifs.fail())
	{
		std::cerr << "Hiba tortent a \"" << filePath << "\" megnyitasakor." << std::endl;
		return false;
	}
	return true;
}

void create(std::istream& ifs, std::vector<std::unique_ptr<Layer>>& layers, std::vector<WeatherCondition*>& conds)
{
	// ============ Populating layers ============ //
	int layerCount; ifs >> layerCount;
	for (int i = 0; i < layerCount; ++i)
	{
		char layerType;
		double layerThickness;
		ifs >> layerType >> layerThickness;
		layers.emplace_back(Layer::make(layerType, layerThickness));
	}

	// ============ Populating atmosphere conditions ============ //
	std::string conditionRawData; ifs >> conditionRawData;
	for (char c : conditionRawData)
		conds.emplace_back(WeatherCondition::make(c));
}

void transformLayers(std::vector<std::unique_ptr<Layer>>& layers, const WeatherCondition& cond)
{
	for (size_t i = 0; i < layers.size(); ++i)
	{
		std::unique_ptr<Layer> newL(layers[i]->transmute(cond));
		if (newL)
		{
			layers.insert(layers.begin() + i, std::move(newL));
			++i;
		}
	}
}

std::unique_ptr<Layer>* findSameTypeAbove(std::vector<std::unique_ptr<Layer>>& layers, int start)
{
	for (int i = (start - 1); i >= 0; --i)
	{
		if (layers[i]->getType() == layers[start]->getType())
			return &(layers[i]);
	}
	return nullptr;
}

void mergeLayers(std::vector<std::unique_ptr<Layer>>& layers)
{
	int e = 0;	// Emerged layers
	int v = (int)layers.size() - 1;
	while (e <= v)
	{
		if (layers[v]->getFromTransformation() || layers[v]->getThickness() < THICKNESS_LIMIT)
		{
			if (std::unique_ptr<Layer> * aboveSameLayer = findSameTypeAbove(layers, v)) 
			{
				// Merge if there is layer above of same type
				(*aboveSameLayer)->merge(layers[v].release());
				layers.erase(layers.begin() + v);
			}
			else
			{
				if (layers[v]->getThickness() < THICKNESS_LIMIT)
				{
					// Dissipate if too thin
					layers.erase(layers.begin() + v);
				}
				else
				{
					// Emerge to the top if thick enough
					std::unique_ptr<Layer> toReplace(std::move(*(layers.begin() + v)));
					layers.erase(layers.begin() + v);
					layers.insert(layers.begin(), std::move(toReplace));
					++e; ++v;
				}
			}
		}
		--v;
	}
}

bool simulationEnd(int layersCount, int initLayersCount)
{
    return (layersCount >= initLayersCount * 3) || (layersCount < 3);
}

int main()
{
	// ============ GetFileName ============ //
	std::ifstream ifs;
	if (!getFile(ifs)) return 1;

	// ============ Main variables ============ //
	std::vector<std::unique_ptr<Layer>> layers;
	std::vector<WeatherCondition*> conditions;

	// ============ Reading data and populating main variables ============ //
	create(ifs, layers, conditions);
    int initLayerCount = layers.size();

	ifs.close();

	// ============ Print Start state ============ //
	printState(layers);

	// ============ Simulation ============ //

    int loopRound = 0;
    while(!simulationEnd(layers.size(),initLayerCount))
    {
	    for (size_t i = 0; i < conditions.size() && !simulationEnd(layers.size(),initLayerCount); ++i)
	    {
            // TRANSFORMING
		    transformLayers(layers, *conditions[i]);

            // BEFORE MERGE
		    printState(layers,
                        std::to_string((conditions.size() * loopRound) + (i + 1))
                        + std::string(".kor - Osszeolvadas elott"),conditions[i]->getType());
		    
            // MERGING
            mergeLayers(layers);

            // AFTER MERGE
		    printState(layers,
                        std::to_string((conditions.size() * loopRound) + (i + 1)) 
                        + std::string(".kor - Osszeolvadas utan"),conditions[i]->getType());
        }
        ++loopRound;
	}


    // Destroying singletons
    Storm::destroy();
    Sunny::destroy();
    Other::destroy();
}

#else
#include "tests.h"
#endif