#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <iomanip>

#include "layer.h"
#include "weather_cond.h"

#define THICKNESS_LIMIT 0.5
#define NORMAL_MODE

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
					layers[0]->solidify();
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

#ifdef NORMAL_MODE
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
#define CATCH_CONFIG_MAIN
#include "catch.hpp"



#define COMPARE_DOUBLES(A,B) ((A)-(B) < 1e-5f)
TEST_CASE("Outer cycle, Conditions - count 0")
{
    std::ifstream ifs("../tests/Test01.txt");
    if(ifs.fail()) FAIL();

    std::vector<std::unique_ptr<Layer>> data;
    std::vector<WeatherCondition*> conds;
    create(ifs,data,conds);


    for(size_t i = 0; i < conds.size(); ++i)
    {
        transformLayers(data,*conds[i]);
		mergeLayers(data);
    }

	
	CHECK(data[0]->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),5.0));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),0.8));
	CHECK(data[1]->getFromTransformation() == false);

	CHECK(data[2]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[2]->getThickness(),3.0));
	CHECK(data[2]->getFromTransformation() == false);

	CHECK(data[3]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[3]->getThickness(),4));
	CHECK(data[3]->getFromTransformation() == false);
	
}

TEST_CASE("Outer cycle, Conditions - count 1")
{
    std::ifstream ifs("../tests/Test02.txt");
    if(ifs.fail()) FAIL();

    std::vector<std::unique_ptr<Layer>> data;
    std::vector<WeatherCondition*> conds;
    create(ifs,data,conds);


    for(size_t i = 0; i < conds.size(); ++i)
    {
        transformLayers(data,*conds[i]);
		mergeLayers(data);
    }

	CHECK(data[0]->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),4.75));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),0.72));
	CHECK(data[1]->getFromTransformation() == false);

	CHECK(data[2]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[2]->getThickness(),3.4));
	CHECK(data[2]->getFromTransformation() == false);

	CHECK(data[3]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[3]->getThickness(),3.6));
	CHECK(data[3]->getFromTransformation() == false);
	
}

TEST_CASE("Outer cycle, Conditions - count 2")
{
    std::ifstream ifs("../tests/Test03.txt");
    if(ifs.fail()) FAIL();

    std::vector<std::unique_ptr<Layer>> data;
    std::vector<WeatherCondition*> conds;
    create(ifs,data,conds);


    for(size_t i = 0; i < conds.size(); ++i)
    {
        transformLayers(data,*conds[i]);
		mergeLayers(data);
    }

	CHECK(data[0]->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),6.91));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),3.4));
	CHECK(data[1]->getFromTransformation() == false);

	CHECK(data[2]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[2]->getThickness(),1.8));
	CHECK(data[2]->getFromTransformation() == false);
	
}

TEST_CASE("Transmutation - Storm | Ozone")
{
	Ozone o(1.0);
	Layer* newL = o.transmute(*Storm::instance());
	CHECK(newL == nullptr);
	CHECK(o.getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(o.getThickness(),1.0));
	CHECK(o.getFromTransformation() == false);

	Storm::destroy();
}

TEST_CASE("Transmutation - Storm | Oxygen")
{
	Oxygen o(1.0);
	Layer* newL = o.transmute(*Storm::instance());
	CHECK(newL->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(newL->getThickness(),0.5));
	CHECK(newL->getFromTransformation() == true);

	CHECK(o.getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(o.getThickness(),0.5));
	CHECK(o.getFromTransformation() == false);

	delete newL;
	Storm::destroy();
}

TEST_CASE("Transmutation - Storm | CarbonDioxide")
{
	CarbonDioxide o(1.0);
	Layer* newL = o.transmute(*Storm::instance());
	CHECK(newL == nullptr);

	CHECK(o.getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(o.getThickness(),1.0));
	CHECK(o.getFromTransformation() == false);

	Storm::destroy();
}

TEST_CASE("Transmutation - Sunny | Ozone")
{
	Ozone o(1.0);
	Layer* newL = o.transmute(*Sunny::instance());
	CHECK(newL == nullptr);

	CHECK(o.getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(o.getThickness(),1.0));
	CHECK(o.getFromTransformation() == false);

	Sunny::destroy();
}

TEST_CASE("Transmutation - Sunny | Oxygen")
{
	Oxygen o(1.0);
	Layer* newL = o.transmute(*Sunny::instance());
	CHECK(newL->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(newL->getThickness(),0.05));
	CHECK(newL->getFromTransformation() == true);

	CHECK(o.getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(o.getThickness(),0.95));
	CHECK(o.getFromTransformation() == false);

	delete newL;
	Sunny::destroy();
}

TEST_CASE("Transmutation - Sunny | CarbonDioxide")
{
	CarbonDioxide o(1.0);
	Layer* newL = o.transmute(*Sunny::instance());
	CHECK(newL->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(newL->getThickness(),0.05));
	CHECK(newL->getFromTransformation() == true);

	CHECK(o.getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(o.getThickness(),0.95));
	CHECK(o.getFromTransformation() == false);

	delete newL;
	Sunny::destroy();
}

TEST_CASE("Transmutation - Other | Ozone")
{
	Ozone o(1.0);
	Layer* newL = o.transmute(*Other::instance());
	CHECK(newL->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(newL->getThickness(),0.05));
	CHECK(newL->getFromTransformation() == true);

	CHECK(o.getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(o.getThickness(),0.95));
	CHECK(o.getFromTransformation() == false);

	delete newL;
	Other::destroy();
}

TEST_CASE("Transmutation - Other | Oxygen")
{
	Oxygen o(1.0);
	Layer* newL = o.transmute(*Other::instance());
	CHECK(newL->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(newL->getThickness(),0.10));
	CHECK(newL->getFromTransformation() == true);

	CHECK(o.getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(o.getThickness(),0.90));
	CHECK(o.getFromTransformation() == false);

	delete newL;
	Other::destroy();
}

TEST_CASE("Transmutation - Other | CarbonDioxide")
{
	CarbonDioxide o(1.0);
	Layer* newL = o.transmute(*Other::instance());
	CHECK(newL == nullptr);

	CHECK(o.getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(o.getThickness(),1.0));
	CHECK(o.getFromTransformation() == false);

	delete newL;
	Other::destroy();
}

TEST_CASE("Merging - There is same layer w > 0.5 freshTransformed")
{
	std::vector<std::unique_ptr<Layer>> data;
	data.emplace_back(Layer::make('x',3.2));
	data.emplace_back(Layer::make('z',4.6));
	data.emplace_back(Layer::make('x',6.8,true));
	data.emplace_back(Layer::make('s',10.0));

	mergeLayers(data);

	CHECK(data[0]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),10.0));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),4.6));
	CHECK(data[1]->getFromTransformation() == false);

	CHECK(data[2]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[2]->getThickness(),10.0));
	CHECK(data[2]->getFromTransformation() == false);
}

TEST_CASE("Merging - There is same layer w < 0.5")
{
	std::vector<std::unique_ptr<Layer>> data;
	data.emplace_back(Layer::make('x',3.2));
	data.emplace_back(Layer::make('z',4.6));
	data.emplace_back(Layer::make('x',0.3));
	data.emplace_back(Layer::make('s',10.0));

	mergeLayers(data);

	CHECK(data[0]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),3.5));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),4.6));
	CHECK(data[1]->getFromTransformation() == false);

	CHECK(data[2]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[2]->getThickness(),10.0));
	CHECK(data[2]->getFromTransformation() == false);
}

TEST_CASE("Merging - No same layer but all w > 0.5")
{
	std::vector<std::unique_ptr<Layer>> data;
	data.emplace_back(Layer::make('x',3.2));
	data.emplace_back(Layer::make('z',4.6));
	data.emplace_back(Layer::make('s',10.0));

	mergeLayers(data);

	CHECK(data[0]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),3.2));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),4.6));
	CHECK(data[1]->getFromTransformation() == false);

	CHECK(data[2]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[2]->getThickness(),10.0));
	CHECK(data[2]->getFromTransformation() == false);
}

TEST_CASE("Merging - No same layer but exist w < 0.5 - dissipates")
{
	std::vector<std::unique_ptr<Layer>> data;
	data.emplace_back(Layer::make('x',3.2));
	data.emplace_back(Layer::make('z',0.3));
	data.emplace_back(Layer::make('s',10.0));

	mergeLayers(data);

	CHECK(data[0]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),3.2));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),10.0));
	CHECK(data[1]->getFromTransformation() == false);
}

TEST_CASE("Merging - No same layer but exist w > 0.5 fresh - emerges")
{
	std::vector<std::unique_ptr<Layer>> data;
	data.emplace_back(Layer::make('x',3.2));
	data.emplace_back(Layer::make('z',5.5,true));
	data.emplace_back(Layer::make('s',10.0));

	mergeLayers(data);

	CHECK(data[0]->getType() == "Ozon");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),5.5));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),3.2));
	CHECK(data[1]->getFromTransformation() == false);

	CHECK(data[2]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[2]->getThickness(),10.0));
	CHECK(data[2]->getFromTransformation() == false);
}

TEST_CASE("Merging - No same layer but exist w < 0.5 fresh - dissipates")
{
	std::vector<std::unique_ptr<Layer>> data;
	data.emplace_back(Layer::make('x',3.2));
	data.emplace_back(Layer::make('z',0.32,true));
	data.emplace_back(Layer::make('s',10.0));

	mergeLayers(data);

	CHECK(data[0]->getType() == "Oxigen");
	CHECK(COMPARE_DOUBLES(data[0]->getThickness(),3.2));
	CHECK(data[0]->getFromTransformation() == false);

	CHECK(data[1]->getType() == "Szendioxid");
	CHECK(COMPARE_DOUBLES(data[1]->getThickness(),10.0));
	CHECK(data[1]->getFromTransformation() == false);
}

#endif