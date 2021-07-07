

#include "SPBiome.h"
#include <string.h>

// WARNING these variables are not thread local, but they should be. So any static storage here created on one thread may be overwritten with values from another thread, which can definitely cause problems.
// typeMaps should generally be consistent between the server, the logic thread, and the main thread when the game is running. However they are not consistent between the globe view thread and the rest, so the globe thread must be finished with before the others are loaded.
// possibly could use __declspec(thread) (MSVC) and __thread (for GNU), though not sure on macOS, maybe c11's thread_local

static uint16_t biomeTag_hot;
static uint16_t biomeTag_wet;
static uint16_t biomeTag_dry;
static uint16_t biomeTag_desert;
static uint16_t biomeTag_steppe;
static uint16_t biomeTag_rainforest;
static uint16_t biomeTag_tropical;
static uint16_t biomeTag_monsoon;
static uint16_t biomeTag_savanna;
static uint16_t biomeTag_polar;
static uint16_t biomeTag_icecap;
static uint16_t biomeTag_tundra;
static uint16_t biomeTag_temperate;
static uint16_t biomeTag_veryColdWinter;
static uint16_t biomeTag_heavySnowWinter;
static uint16_t biomeTag_medSnowWinter;
static uint16_t biomeTag_lightSnowWinter;
static uint16_t biomeTag_heavySnowSummer;
static uint16_t biomeTag_medSnowSummer;
static uint16_t biomeTag_lightSnowSummer;
static uint16_t biomeTag_drySummer;
static uint16_t biomeTag_dryWinter;
static uint16_t biomeTag_verySparseForest;
static uint16_t biomeTag_sparseForest;
static uint16_t biomeTag_mediumForest;
static uint16_t biomeTag_denseForest;
static uint16_t biomeTag_coniferous;
static uint16_t biomeTag_birch;
static uint16_t biomeTag_cliff;
static uint16_t biomeTag_river;

static uint32_t terrainBaseType_rock;
static uint32_t terrainBaseType_limestone;
static uint32_t terrainBaseType_beachSand;
static uint32_t terrainBaseType_riverSand;
static uint32_t terrainBaseType_desertSand;
static uint32_t terrainBaseType_ice;
static uint32_t terrainBaseType_desertRedSand;
static uint32_t terrainBaseType_redRock;
static uint32_t terrainBaseType_dirt;
static uint32_t terrainBaseType_richDirt;
static uint32_t terrainBaseType_poorDirt;
static uint32_t terrainBaseType_clay;

static uint32_t terrainVariation_snow;
static uint32_t terrainVariation_temperateGrass;
static uint32_t terrainVariation_temperateGrassPlentiful;
static uint32_t terrainVariation_temperateGrassWinter;
static uint32_t terrainVariation_taigaGrass;
static uint32_t terrainVariation_mediterraneanGrass;
static uint32_t terrainVariation_mediterraneanGrassPlentiful;
static uint32_t terrainVariation_steppeGrass;
static uint32_t terrainVariation_tropicalRainforestGrass;
static uint32_t terrainVariation_monsoonGrass;
static uint32_t terrainVariation_savannaGrass;
static uint32_t terrainVariation_tundraGrass;
static uint32_t terrainVariation_limestone;
static uint32_t terrainVariation_flint;
static uint32_t terrainVariation_clay;

static uint32_t terrainModifcation_snowRemoved;
static uint32_t terrainModifcation_vegetationRemoved;
static uint32_t terrainModifcation_vegetationAdded;
static uint32_t terrainModifcation_preventGrassAndSnow;

static uint32_t gameObjectType_appleTree;
static uint32_t gameObjectType_orangeTree;
static uint32_t gameObjectType_peachTree;
static uint32_t gameObjectType_aspen3;
static uint32_t gameObjectType_sunflower;
static uint32_t gameObjectType_raspberryBush;
static uint32_t gameObjectType_gooseberryBush;
static uint32_t gameObjectType_tallPine;
static uint32_t gameObjectType_beetrootPlant;
static uint32_t gameObjectType_wheatPlant;
static uint32_t gameObjectType_bananaTree;

static uint32_t gameObjectType_rock;
static uint32_t gameObjectType_rockSmall;
static uint32_t gameObjectType_rockLarge;
static uint32_t gameObjectType_limestoneRock;
static uint32_t gameObjectType_limestoneRockSmall;
static uint32_t gameObjectType_limestoneRockLarge;
static uint32_t gameObjectType_flint;
static uint32_t gameObjectType_birchBranch;
static uint32_t gameObjectType_pineBranch;

#define BIRCH_TYPE_COUNT 6
static uint32_t gameObjectType_birchTypes[BIRCH_TYPE_COUNT];

#define MED_PINE_TYPE_COUNT 2
static uint32_t gameObjectType_medPineTypes[MED_PINE_TYPE_COUNT];

#define WILLOW_TYPE_COUNT 2
static uint32_t gameObjectType_willowTypes[WILLOW_TYPE_COUNT];

static uint32_t gameObjectType_smallPine;

void spBiomeInit(SPBiomeThreadState* threadState)
{
	biomeTag_hot = threadState->getBiomeTag(threadState, "hot");
	biomeTag_wet = threadState->getBiomeTag(threadState, "wet");
	biomeTag_dry = threadState->getBiomeTag(threadState, "dry");
	biomeTag_desert = threadState->getBiomeTag(threadState, "desert");
	biomeTag_steppe = threadState->getBiomeTag(threadState, "steppe");
	biomeTag_rainforest = threadState->getBiomeTag(threadState, "rainforest");
	biomeTag_tropical = threadState->getBiomeTag(threadState, "tropical");
	biomeTag_monsoon = threadState->getBiomeTag(threadState, "monsoon");
	biomeTag_savanna = threadState->getBiomeTag(threadState, "savanna");
	biomeTag_polar = threadState->getBiomeTag(threadState, "polar");
	biomeTag_icecap = threadState->getBiomeTag(threadState, "icecap");
	biomeTag_tundra = threadState->getBiomeTag(threadState, "tundra");
	biomeTag_temperate = threadState->getBiomeTag(threadState, "temperate");
	biomeTag_veryColdWinter = threadState->getBiomeTag(threadState, "veryColdWinter");
	biomeTag_heavySnowWinter = threadState->getBiomeTag(threadState, "heavySnowWinter");
	biomeTag_medSnowWinter = threadState->getBiomeTag(threadState, "medSnowWinter");
	biomeTag_lightSnowWinter = threadState->getBiomeTag(threadState, "lightSnowWinter");
	biomeTag_heavySnowSummer = threadState->getBiomeTag(threadState, "heavySnowSummer");
	biomeTag_medSnowSummer = threadState->getBiomeTag(threadState, "medSnowSummer");
	biomeTag_lightSnowSummer = threadState->getBiomeTag(threadState, "lightSnowSummer");
	biomeTag_drySummer = threadState->getBiomeTag(threadState, "drySummer");
	biomeTag_dryWinter = threadState->getBiomeTag(threadState, "dryWinter");
	biomeTag_verySparseForest = threadState->getBiomeTag(threadState, "verySparseForest");
	biomeTag_sparseForest = threadState->getBiomeTag(threadState, "sparseForest");
	biomeTag_mediumForest = threadState->getBiomeTag(threadState, "mediumForest");
	biomeTag_denseForest = threadState->getBiomeTag(threadState, "denseForest");
	biomeTag_coniferous = threadState->getBiomeTag(threadState, "coniferous");
	biomeTag_birch = threadState->getBiomeTag(threadState, "birch");
	biomeTag_cliff = threadState->getBiomeTag(threadState, "cliff");
	biomeTag_river = threadState->getBiomeTag(threadState, "river");

	terrainBaseType_rock							= threadState->getTerrainBaseTypeIndex(threadState, "rock");
	terrainBaseType_limestone						= threadState->getTerrainBaseTypeIndex(threadState, "limestone");
	terrainBaseType_beachSand						= threadState->getTerrainBaseTypeIndex(threadState, "beachSand");
	terrainBaseType_riverSand					    = threadState->getTerrainBaseTypeIndex(threadState, "riverSand");
	terrainBaseType_desertSand						= threadState->getTerrainBaseTypeIndex(threadState, "desertSand");
	terrainBaseType_ice								= threadState->getTerrainBaseTypeIndex(threadState, "ice");
	terrainBaseType_desertRedSand					= threadState->getTerrainBaseTypeIndex(threadState, "desertRedSand");
	terrainBaseType_redRock							= threadState->getTerrainBaseTypeIndex(threadState, "redRock");
	terrainBaseType_dirt							= threadState->getTerrainBaseTypeIndex(threadState, "dirt");
	terrainBaseType_richDirt						= threadState->getTerrainBaseTypeIndex(threadState, "richDirt");
	terrainBaseType_poorDirt						= threadState->getTerrainBaseTypeIndex(threadState, "poorDirt");
	terrainBaseType_clay							= threadState->getTerrainBaseTypeIndex(threadState, "clay");
													
	terrainVariation_snow							= threadState->getTerrainVariation(threadState, "snow");
	terrainVariation_temperateGrass					= threadState->getTerrainVariation(threadState, "temperateGrass");
	terrainVariation_temperateGrassPlentiful		= threadState->getTerrainVariation(threadState, "temperateGrassPlentiful");
	terrainVariation_temperateGrassWinter			= threadState->getTerrainVariation(threadState, "temperateGrassWinter");
	terrainVariation_taigaGrass						= threadState->getTerrainVariation(threadState, "taigaGrass");
	terrainVariation_mediterraneanGrass				= threadState->getTerrainVariation(threadState, "mediterraneanGrass");
	terrainVariation_mediterraneanGrassPlentiful	= threadState->getTerrainVariation(threadState, "mediterraneanGrassPlentiful");	
	terrainVariation_steppeGrass					= threadState->getTerrainVariation(threadState, "steppeGrass");
	terrainVariation_tropicalRainforestGrass		= threadState->getTerrainVariation(threadState, "tropicalRainforestGrass");
	terrainVariation_monsoonGrass					= threadState->getTerrainVariation(threadState, "monsoonGrass");
	terrainVariation_savannaGrass					= threadState->getTerrainVariation(threadState, "savannaGrass");
	terrainVariation_tundraGrass					= threadState->getTerrainVariation(threadState, "tundraGrass");
	terrainVariation_flint							= threadState->getTerrainVariation(threadState, "flint");
	terrainVariation_clay							= threadState->getTerrainVariation(threadState, "clay");
	terrainVariation_limestone						= threadState->getTerrainVariation(threadState, "limestone");
													
	terrainModifcation_snowRemoved					= threadState->getTerrainModification(threadState, "snowRemoved");
	terrainModifcation_vegetationRemoved			= threadState->getTerrainModification(threadState, "vegetationRemoved");
	terrainModifcation_vegetationAdded				= threadState->getTerrainModification(threadState, "vegetationAdded");
	terrainModifcation_preventGrassAndSnow			= threadState->getTerrainModification(threadState, "preventGrassAndSnow");

	if(threadState->getGameObjectTypeIndex) //this function isn't set where game object types aren't required eg. in the initial world creation screen
	{
		gameObjectType_appleTree = threadState->getGameObjectTypeIndex(threadState, "appleTree");
		gameObjectType_orangeTree = threadState->getGameObjectTypeIndex(threadState, "orangeTree");
		gameObjectType_peachTree = threadState->getGameObjectTypeIndex(threadState, "peachTree");
		gameObjectType_aspen3 = threadState->getGameObjectTypeIndex(threadState, "aspen3");
		gameObjectType_bananaTree = threadState->getGameObjectTypeIndex(threadState, "bananaTree");

		gameObjectType_sunflower = threadState->getGameObjectTypeIndex(threadState, "sunflower");
		gameObjectType_raspberryBush = threadState->getGameObjectTypeIndex(threadState, "raspberryBush");
		gameObjectType_gooseberryBush = threadState->getGameObjectTypeIndex(threadState, "gooseberryBush");
		gameObjectType_beetrootPlant = threadState->getGameObjectTypeIndex(threadState, "beetrootPlant");
		gameObjectType_wheatPlant = threadState->getGameObjectTypeIndex(threadState, "wheatPlant");

		gameObjectType_rock = threadState->getGameObjectTypeIndex(threadState, "rock");
		gameObjectType_rockSmall = threadState->getGameObjectTypeIndex(threadState, "rockSmall");
		gameObjectType_limestoneRockSmall = threadState->getGameObjectTypeIndex(threadState, "limestoneRockSmall");
		gameObjectType_rockLarge = threadState->getGameObjectTypeIndex(threadState, "rockLarge");
		gameObjectType_limestoneRock = threadState->getGameObjectTypeIndex(threadState, "limestoneRock");
		gameObjectType_limestoneRockLarge = threadState->getGameObjectTypeIndex(threadState, "limestoneRockLarge");
		gameObjectType_flint = threadState->getGameObjectTypeIndex(threadState, "flint");
		gameObjectType_birchBranch = threadState->getGameObjectTypeIndex(threadState, "birchBranch");
		gameObjectType_pineBranch = threadState->getGameObjectTypeIndex(threadState, "pineBranch");

		gameObjectType_birchTypes[0] = threadState->getGameObjectTypeIndex(threadState, "birch1");
		gameObjectType_birchTypes[1] = threadState->getGameObjectTypeIndex(threadState, "birch2");
		gameObjectType_birchTypes[2] = threadState->getGameObjectTypeIndex(threadState, "birch3");
		gameObjectType_birchTypes[3] = threadState->getGameObjectTypeIndex(threadState, "birch4");
		gameObjectType_birchTypes[4] = threadState->getGameObjectTypeIndex(threadState, "aspen1");
		gameObjectType_birchTypes[5] = threadState->getGameObjectTypeIndex(threadState, "aspen2");

		gameObjectType_willowTypes[0] = threadState->getGameObjectTypeIndex(threadState, "willow1");
		gameObjectType_willowTypes[1] = threadState->getGameObjectTypeIndex(threadState, "willow2");

		gameObjectType_medPineTypes[0] = threadState->getGameObjectTypeIndex(threadState, "pine1");
		gameObjectType_medPineTypes[1] = threadState->getGameObjectTypeIndex(threadState, "pine3");
		gameObjectType_tallPine = threadState->getGameObjectTypeIndex(threadState, "pine2");
		gameObjectType_smallPine = threadState->getGameObjectTypeIndex(threadState, "pine4");
	}
}

float getSoilRichnessNoiseValue(SPBiomeThreadState* threadState, SPVec3 noiseLoc, float steepness, float riverDistance)
{
	SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 20000.0);
	return spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 4) - steepness * 0.5 + (1.0 - pow(riverDistance, 0.3)) * 0.6;
}

void spBiomeGetTagsForPoint(SPBiomeThreadState* threadState,
	uint16_t* tagsOut,
	int* tagCountOut,
	SPVec3 pointNormal,
	SPVec3 noiseLoc,
	double altitude,
	float steepness,
	float riverDistance,
	float temperatureSummer,
	float temperatureWinter,
	float rainfallSummer,
	float rainfallWinter)
{
	int tagCount = *tagCountOut;

	float annualRainfall = rainfallSummer + rainfallWinter;
	float averageTemp = (temperatureSummer + temperatureWinter) * 0.5f;
	float temperatureThreshold = averageTemp * 20.0f;

	SPVec3 scaledNoiseLocC = spVec3Mul(noiseLoc, 802.0);
	double noiseValueC = spNoiseGet(threadState->spNoise1, scaledNoiseLocC, 2);
	//double riverRainfallAddition = (1.0 - pow(riverDistance, 0.05));
	//riverRainfallAddition = riverRainfallAddition * (0.5 + noiseValueC) * 500.0;
	double riverRainfallAddition = (1.0 - pow(spMax(riverDistance - 0.01, 0.0), 0.1));
	riverRainfallAddition = riverRainfallAddition * (1.0 + noiseValueC) * 500.0;

	double annualRainfallWithRiverAddition = annualRainfall + riverRainfallAddition;

	double mixFraction = (rainfallSummer - rainfallWinter * 2.3f) * 0.00001;
	temperatureThreshold = spMix(temperatureThreshold, temperatureThreshold + 200.0f,  mixFraction);

	bool cliff = false;
	if(steepness > 2.0)
	{
		cliff = true;
		tagsOut[tagCount++] = biomeTag_cliff;
	}


	if(riverDistance < 0.01 && altitude < 0.0000002)
	{
		tagsOut[tagCount++] = biomeTag_river;
	}

	if(averageTemp >= 18.0f)
	{
		tagsOut[tagCount++] = biomeTag_hot;
	}
	else
	{
		if(temperatureWinter < -5.0f)
		{
			tagsOut[tagCount++] = biomeTag_veryColdWinter;
		}
	
		if(annualRainfall > temperatureThreshold * 0.5)
		{
			if(temperatureSummer < 8.0f)
			{
				if(annualRainfall > temperatureThreshold * 2.0)
				{
					tagsOut[tagCount++] = biomeTag_heavySnowSummer;
				}
				else if(annualRainfall > temperatureThreshold)
				{
					tagsOut[tagCount++] = biomeTag_medSnowSummer;
				}
				else
				{
					tagsOut[tagCount++] = biomeTag_lightSnowSummer;
				}
			}
			else if(temperatureWinter < 8.0f)
			{
				if(annualRainfall > temperatureThreshold * 2.0)
				{
					tagsOut[tagCount++] = biomeTag_heavySnowWinter;
				}
				else if(annualRainfall > temperatureThreshold)
				{
					tagsOut[tagCount++] = biomeTag_medSnowWinter;
				}
				else
				{
					tagsOut[tagCount++] = biomeTag_lightSnowWinter;
				}
			}
		}
	}



	if (annualRainfallWithRiverAddition < temperatureThreshold) // B
	{
		tagsOut[tagCount++] = biomeTag_dry;

		if (annualRainfallWithRiverAddition < temperatureThreshold * 0.5)
		{
			tagsOut[tagCount++] = biomeTag_desert;
		}
		else
		{
			tagsOut[tagCount++] = biomeTag_steppe;

			if(averageTemp < 18.0f && !cliff)
			{
				SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 8000.0);
				double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
				if(noiseValue > 0.1)
				{
					tagsOut[tagCount++] = biomeTag_verySparseForest;
					tagsOut[tagCount++] = biomeTag_coniferous;
				}
			}
		}

	}
	else
	{
		if (temperatureWinter > 18.0f) // A
		{
			tagsOut[tagCount++] = biomeTag_tropical;

			float driestMonth = (float)spMin(rainfallSummer, rainfallWinter) / 6.0f;
			double treeDensityOffset = 0.0;
			if (driestMonth > 60.0f)
			{
				tagsOut[tagCount++] = biomeTag_rainforest;
			}
			else if (driestMonth > (100.0f - (annualRainfall / 25.0f)))
			{
				tagsOut[tagCount++] = biomeTag_monsoon;
			}
			else
			{
				tagsOut[tagCount++] = biomeTag_savanna;
				treeDensityOffset = - 0.4;
			}

			double noiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance) + treeDensityOffset;
			if(noiseValue > -0.2)
			{
				if(noiseValue > 0.7)
				{
					tagsOut[tagCount++] = biomeTag_denseForest;
				}
				else if(noiseValue > 0.4)
				{
					tagsOut[tagCount++] = biomeTag_mediumForest;
				}
				else
				{
					tagsOut[tagCount++] = biomeTag_sparseForest;
				}
			}
			else
			{
				tagsOut[tagCount++] = biomeTag_verySparseForest;
			}
		}
		else
		{
			if (temperatureSummer < -2.0f)// EF
			{
				tagsOut[tagCount++] = biomeTag_polar;
				tagsOut[tagCount++] = biomeTag_icecap;
			}
			else if (temperatureSummer < 8.0f)// ET
			{
				tagsOut[tagCount++] = biomeTag_polar;
				tagsOut[tagCount++] = biomeTag_tundra;

				if(!cliff)
				{
					double noiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance);
					if(noiseValue > 0.0)
					{
						if(noiseValue > 0.3)
						{
							tagsOut[tagCount++] = biomeTag_sparseForest;
						}
						else
						{
							tagsOut[tagCount++] = biomeTag_verySparseForest;
						}
						tagsOut[tagCount++] = biomeTag_coniferous;
					}
				}
			}
			else // C or D
			{
				tagsOut[tagCount++] = biomeTag_temperate;

				bool drySummer = false;
				if (rainfallSummer < 30.0f && rainfallSummer < rainfallWinter / 3.0f)
				{
					tagsOut[tagCount++] = biomeTag_drySummer;
					drySummer = true;
				}
				else if (rainfallWinter < rainfallSummer / 10.0f)
				{
					tagsOut[tagCount++] = biomeTag_dryWinter;
				}

				if(!cliff)
				{
					double noiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance);
					if(noiseValue > -0.2)
					{
						if(noiseValue > 0.7)
						{
							tagsOut[tagCount++] = biomeTag_denseForest;
						}
						else if(noiseValue > 0.4)
						{
							tagsOut[tagCount++] = biomeTag_mediumForest;
						}
						else
						{
							tagsOut[tagCount++] = biomeTag_sparseForest;
						}
					}
					else
					{
						tagsOut[tagCount++] = biomeTag_verySparseForest;
					}


					SPVec3 scaledNoiseLocB = spVec3Mul(noiseLoc, 60501.0);
					double noiseValueB = spNoiseGet(threadState->spNoise1, scaledNoiseLocB, 2);
					if(temperatureSummer > noiseValueB * 4.0 + 2.0f)
					{
						if(!drySummer)
						{
							tagsOut[tagCount++] = biomeTag_birch;
						}
						if(temperatureWinter < noiseValueB * 10.0 - 5.0f)
						{
							tagsOut[tagCount++] = biomeTag_coniferous;
						}
					}
					else
					{
						tagsOut[tagCount++] = biomeTag_coniferous;
					}
				}

			}
		}
	}

	*tagCountOut = tagCount;
}

#define rockSteepness 2.0
#define claySteepness 1.6


typedef struct SurfaceTypeInfo {
	bool river;
	bool hot;
	int snowDepth;
} SurfaceTypeInfo;

void getSurfaceTypeInfo(uint16_t* biomeTags, int tagCount, int seasonIndex, SurfaceTypeInfo* surfaceTypeInfo, float steepness, double noiseValue)
{
	for(int i = 0; i < tagCount; i++)
	{
		if(biomeTags[i] == biomeTag_river)
		{
			surfaceTypeInfo->river = true;
		}
		else if(biomeTags[i] == biomeTag_hot)
		{
			surfaceTypeInfo->hot = true;
		}
		else if(biomeTags[i] == biomeTag_heavySnowSummer)
		{
			surfaceTypeInfo->snowDepth = 3;
		}
		else if(biomeTags[i] == biomeTag_medSnowSummer)
		{
			if(seasonIndex == 1)
			{
				surfaceTypeInfo->snowDepth = 2;
			}
			else
			{
				surfaceTypeInfo->snowDepth = 3;
			}
		}
		else if(biomeTags[i] == biomeTag_lightSnowSummer)
		{
			if(seasonIndex == 1)
			{
				surfaceTypeInfo->snowDepth = 1;
			}
			else if(seasonIndex == 0 || seasonIndex == 2)
			{
				surfaceTypeInfo->snowDepth = 2;
			}
			else
			{
				surfaceTypeInfo->snowDepth = 3;
			}
		}
		else if(biomeTags[i] == biomeTag_heavySnowWinter)
		{
			if(seasonIndex == 3)
			{
				surfaceTypeInfo->snowDepth = 3;
			}
		}
		else if(biomeTags[i] == biomeTag_medSnowWinter)
		{
			if(seasonIndex == 3)
			{
				surfaceTypeInfo->snowDepth = 2;
			}
		}
		else if(biomeTags[i] == biomeTag_lightSnowWinter)
		{
			if(seasonIndex == 3)
			{
				surfaceTypeInfo->snowDepth = 1;
			}
		}
	}

	if(surfaceTypeInfo->snowDepth > 0)
	{
		if(steepness > 0.4 + noiseValue * 0.2)
		{
			surfaceTypeInfo->snowDepth--;
		}
		if(steepness > 0.7 + noiseValue * 0.2)
		{
			surfaceTypeInfo->snowDepth--;
		}
	}
}

uint32_t getBeachSurfaceType(SurfaceTypeInfo* surfaceTypeInfo, float riverDistance, float noiseValue, int16_t digFillOffset)
{
	if(surfaceTypeInfo->river)
	{
		if(riverDistance < (noiseValue * 0.01 + 0.005) - (0.04 * digFillOffset))
		{
			return terrainBaseType_riverSand;
		}
		else
		{
			return terrainBaseType_beachSand;
		}
	}

	return (noiseValue > 0.1 + (0.4 * digFillOffset) ? terrainBaseType_riverSand : terrainBaseType_beachSand);
}

SPSurfaceTypeResult spBiomeGetSurfaceTypeForPoint(SPBiomeThreadState* threadState,
	SPSurfaceTypeResult incomingType,
	uint16_t* tags,
	int tagCount,
	uint32_t* modifications,
	int modificationCount,
	uint32_t fillGameObjectTypeIndex,
	int16_t digFillOffset,
	uint32_t* variations,
	SPVec3 pointNormal, 
	SPVec3 noiseLoc, 
	double altitude,
	float steepness,
	float riverDistance,
	int seasonIndex)
{
	SurfaceTypeInfo surfaceTypeInfo;
	SPSurfaceTypeResult result = incomingType;
	memset(&surfaceTypeInfo, 0, sizeof(surfaceTypeInfo));
	SPVec3 scaledNoiseLoc = spVec3Mul(noiseLoc, 45999.0);
	double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);

	getSurfaceTypeInfo(tags, tagCount, seasonIndex, &surfaceTypeInfo, steepness, noiseValue);



	SPVec3 scaledNoiseMedScale = spVec3Mul(noiseLoc, 92273.0);
	double noiseValueMed = spNoiseGet(threadState->spNoise1, scaledNoiseMedScale, 2);

	double soilRichnessNoiseValue = getSoilRichnessNoiseValue(threadState, noiseLoc, steepness, riverDistance);

	SPVec3 scaledNoiseLocLargeScale = spVec3Mul(noiseLoc, 8073.0);
	double noiseValueLarge = spNoiseGet(threadState->spNoise1, scaledNoiseLocLargeScale, 2);

	uint32_t fillSurfaceBaseType = 0;
	if(fillGameObjectTypeIndex != 0)
	{
		fillSurfaceBaseType = threadState->getSurfaceBaseTypeForFillObjectType(threadState, fillGameObjectTypeIndex);
	}

	if(altitude < -0.00000001)
	{
		result.surfaceBaseType = (fillSurfaceBaseType != 0 ? fillSurfaceBaseType : getBeachSurfaceType(&surfaceTypeInfo, riverDistance, noiseValue, digFillOffset));

		SPSurfaceTypeDefault defaults = threadState->getSurfaceDefaultsForBaseType(threadState, result.surfaceBaseType);

		result.materialIndex = defaults.materialIndex;
		result.decalTypeIndex = 0;
		result.pathDifficultyIndex = defaults.pathDifficultyIndex;

		return result;
	}

	bool snowRemoved = false;
	bool vegetationRemoved = false;

	for(int i = 0; i < modificationCount; i++)
	{
		if(modifications[i] == terrainModifcation_snowRemoved)
		{
			snowRemoved = true;
		}
		else if(modifications[i] == terrainModifcation_vegetationRemoved)
		{
			vegetationRemoved = true;
		}
	}

	bool hasClay = (noiseValueMed > 0.0 && noiseValue < 0.2);

	bool isBeach = ((altitude + noiseValue * 0.00000005 + noiseValueLarge * 0.0000005) < 0.0000001);
	bool isRock = (steepness > rockSteepness + noiseValue * 0.5);
	bool isClay = hasClay && !isRock && (steepness > claySteepness + noiseValue * 0.5 - (1.0 - riverDistance) * (1.0 - riverDistance) * 0.5);
	bool isDesertSand = (soilRichnessNoiseValue < -0.65);
	bool hasSand = (!hasClay && soilRichnessNoiseValue < -0.45);

	bool isLimestone = (noiseValueMed > 0.2 && noiseValue < 0.2);

	if(digFillOffset != 0 && !isRock)
	{
		if(digFillOffset < (noiseValue * 4) - 2)
		{
			isRock = true;
		}
		else if(hasClay && !isClay)
		{
			if(digFillOffset < (noiseValueMed * 4) - 1)
			{
				isClay = true;
			}
		}
		else if(hasSand && !isDesertSand)
		{
			if(digFillOffset < (noiseValueMed * 4) - 1)
			{
				isDesertSand = true;
			}
		}
	}



	int soilQuality = 1;


	if(fillSurfaceBaseType != 0)
	{
		result.surfaceBaseType = fillSurfaceBaseType;
	}
	else
	{

		for(int i = 0; i < tagCount; i++)
		{
			if(tags[i] == biomeTag_cliff)
			{
				isRock = true;
			}
		}

		if(isRock)
		{
			result.surfaceBaseType = terrainBaseType_rock;
			if(isLimestone)
			{
				result.surfaceBaseType = terrainBaseType_limestone;
			}
		}
		else if(isClay)
		{
			result.surfaceBaseType = terrainBaseType_clay;
		}
		else if(isBeach)
		{
			result.surfaceBaseType = getBeachSurfaceType(&surfaceTypeInfo, riverDistance, noiseValue, digFillOffset);
		}
		else if(isDesertSand)
		{
			result.surfaceBaseType = terrainBaseType_desertSand;
			soilQuality = 0;
		}
		else
		{
			if(soilRichnessNoiseValue < -0.3)
			{
				result.surfaceBaseType = terrainBaseType_poorDirt;
				soilQuality = 0;
			}
			else if(soilRichnessNoiseValue > 0.4)
			{
				result.surfaceBaseType = terrainBaseType_richDirt;
				soilQuality = 2;
			}
			else
			{
				result.surfaceBaseType = terrainBaseType_dirt;
				soilQuality = 1;
			}
		}


		for(int i = 0; i < tagCount; i++)
		{
			if(tags[i] == biomeTag_desert)
			{
				if(isRock)
				{
					result.surfaceBaseType = terrainBaseType_redRock;
				}
				else if(!isClay)
				{
					if(surfaceTypeInfo.hot)
					{
						result.surfaceBaseType =  terrainBaseType_desertRedSand;
					}
					else
					{
						result.surfaceBaseType = terrainBaseType_desertSand;
					}
				}
			}
			else if(tags[i] == biomeTag_steppe)
			{
				if(surfaceTypeInfo.hot)
				{
					if(isRock && !isClay)
					{
						result.surfaceBaseType = terrainBaseType_redRock;
					}
				}
			}
			else if(tags[i] == biomeTag_icecap)
			{
				if(!isRock && !isClay)
				{
					result.surfaceBaseType = terrainBaseType_ice;
				}
			}
		}
	}

	uint32_t grassVariation = 0;

	if (!vegetationRemoved && (!isRock && !isBeach && !isClay && !isDesertSand))
	{
		for (int i = 0; i < tagCount; i++)
		{
			if (tags[i] == biomeTag_steppe)
			{
				if(soilQuality == 0)
				{
					grassVariation = 0;
				}
				else if(soilQuality == 1)
				{
					grassVariation = terrainVariation_mediterraneanGrass;
				}
				else
				{
					grassVariation = terrainVariation_mediterraneanGrassPlentiful;
				}
			}
			else if (tags[i] == biomeTag_rainforest)
			{
				grassVariation = terrainVariation_tropicalRainforestGrass;
			}
			if (tags[i] == biomeTag_monsoon)
			{
				grassVariation = terrainVariation_monsoonGrass;
			}
			if (tags[i] == biomeTag_savanna)
			{
				grassVariation = terrainVariation_savannaGrass;
			}
			if (tags[i] == biomeTag_tundra)
			{
				grassVariation = terrainVariation_tundraGrass;
			}
			if (tags[i] == biomeTag_temperate)
			{
				if (seasonIndex == 0 || seasonIndex == 1)
				{
					if(soilQuality == 0)
					{
						grassVariation = terrainVariation_tundraGrass;
					}
					else if(soilQuality == 1)
					{
						grassVariation = terrainVariation_temperateGrass;
					}
					else
					{
						grassVariation = terrainVariation_temperateGrassPlentiful;
					}
				}
				else if (seasonIndex == 2)
				{
					if(soilQuality == 0)
					{
						grassVariation = 0;
					}
					else if(soilQuality == 1)
					{
						grassVariation = terrainVariation_mediterraneanGrass;
					}
					else
					{
						grassVariation = terrainVariation_mediterraneanGrassPlentiful;
					}
				}
				else
				{
					if(soilQuality == 0)
					{
						grassVariation = terrainVariation_tundraGrass;
					}
					else if(soilQuality == 1)
					{
						grassVariation = terrainVariation_temperateGrassWinter;
					}
					else
					{
						grassVariation = terrainVariation_temperateGrass;
					}
				}
			}
		}
	}

	bool hasSnow = false;
	if(!snowRemoved && surfaceTypeInfo.snowDepth > 0)
	{
		if(surfaceTypeInfo.snowDepth == 3)
		{
			hasSnow = true;
		}
		else if((noiseValue > -0.1))
		{
			if(surfaceTypeInfo.snowDepth == 2 || (noiseValue > 0.3))
			{
				hasSnow = true;
			}
		}
	}


	if(isLimestone)
	{
		variations[result.variationCount++] = terrainVariation_limestone;
		if(noiseValueMed > 0.3 && soilRichnessNoiseValue < 0.1 && noiseValue < 0.0)
		{
			variations[result.variationCount++] = terrainVariation_flint;
		}
	}

	if(hasClay)
	{
		variations[result.variationCount++] = terrainVariation_clay;
	}


	if(grassVariation != 0)
	{
		variations[result.variationCount++] = grassVariation;
	}
	if(hasSnow)
	{
		variations[result.variationCount++] = terrainVariation_snow;
	}

	result.materialIndex = 0;

	if(grassVariation != 0 || hasSnow)
	{
		SPSurfaceTypeDefault variationDefaults = threadState->getSurfaceDefaultsForVariationType(threadState, (hasSnow ? terrainVariation_snow : grassVariation));
		if(variationDefaults.materialIndex != 0)
		{
			result.materialIndex = variationDefaults.materialIndex;
			result.pathDifficultyIndex = variationDefaults.pathDifficultyIndex;
			if(hasSnow)
			{
				if(grassVariation != 0)
				{
					result.decalTypeIndex = variationDefaults.decalGroupIndex;
				}
				else
				{
					result.decalTypeIndex = 0;
				}
			}
			else
			{
				result.decalTypeIndex = variationDefaults.decalGroupIndex;
			}
		}
	}
	
	if(result.materialIndex == 0)
	{
		SPSurfaceTypeDefault defaults = threadState->getSurfaceDefaultsForBaseType(threadState, result.surfaceBaseType);
		result.materialIndex = defaults.materialIndex;
		result.decalTypeIndex = defaults.decalGroupIndex;
		result.pathDifficultyIndex = defaults.pathDifficultyIndex;
	}

	return result;
}

bool getHasSingleTag(uint16_t* biomeTags, int tagCount, uint16_t tag)
{
	for(int i = 0; i < tagCount; i++)
	{
		if(biomeTags[i] == tag)
		{
			return true;
		}
	}
	return false;
}

typedef struct ForestInfo {
	int forestDensity;
	bool coniferous;
	bool birch;
	bool cold;
	bool river;
	bool tropical;

} ForestInfo;


void getForestInfo(uint16_t* biomeTags, 
	int tagCount, 
	float steepness,
	ForestInfo* forestInfo)
{
	for(int i = 0; i < tagCount; i++)
	{
		if(biomeTags[i] == biomeTag_cliff || steepness > 1.0)
		{
			forestInfo->forestDensity = 0;
			forestInfo->coniferous = false;
			forestInfo->birch = false;
			return;
		}
		else if(biomeTags[i] == biomeTag_river)
		{
			forestInfo->river = true;
		}
		else if(biomeTags[i] == biomeTag_denseForest)
		{
			forestInfo->forestDensity = 4;
		}
		else if(biomeTags[i] == biomeTag_mediumForest)
		{
			forestInfo->forestDensity = 3;
		}
		else if(biomeTags[i] == biomeTag_sparseForest)
		{
			forestInfo->forestDensity = 2;
		}
		else if(biomeTags[i] == biomeTag_verySparseForest)
		{
			forestInfo->forestDensity = 1;
		}
		else if(biomeTags[i] == biomeTag_coniferous)
		{
			forestInfo->coniferous = true;
		}
		else if(biomeTags[i] == biomeTag_birch)
		{
			forestInfo->birch = true;
		}
		else if(biomeTags[i] == biomeTag_tropical)
		{
			forestInfo->tropical = true;
		}
		else if(biomeTags[i] == biomeTag_polar ||
			biomeTags[i] == biomeTag_veryColdWinter)
		{
			forestInfo->cold = true;
		}
	}
}

#define ADD_OBJECT(__addType__)\
types[addedCount++] = __addType__;\
if(addedCount >= BIOME_MAX_GAME_OBJECT_COUNT_PER_SUBDIVISION)\
{\
	return addedCount;\
}

uint32_t getPineType(uint64_t faceUniqueID, int i)
{
	if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 22245 + i, 12) == 1)
	{
		return gameObjectType_tallPine;
	}
	return gameObjectType_medPineTypes[spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3434 + i, MED_PINE_TYPE_COUNT)];
}

uint32_t getBirchType(uint64_t faceUniqueID, int i)
{
	return gameObjectType_birchTypes[spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3434 + i, BIRCH_TYPE_COUNT)];
}

uint32_t getWillowType(uint64_t faceUniqueID, int i)
{
	return gameObjectType_willowTypes[spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3434 + i, WILLOW_TYPE_COUNT)];
}

uint32_t getTreeType(uint64_t faceUniqueID, int i, ForestInfo* forestInfo)
{

	if(forestInfo->river)
	{
		if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 342573 + i, 2) == 1)
		{
			return getWillowType(faceUniqueID, i);
		}
	}

	if(forestInfo->birch && forestInfo->coniferous)
	{
		if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 54634 + i, 2) == 1)
		{
			return getPineType(faceUniqueID, i);
		}
		return getBirchType(faceUniqueID, i);
	}

	if(forestInfo->birch)
	{
		return getBirchType(faceUniqueID, i);
	}

	return getPineType(faceUniqueID, i);
}


int spBiomeGetTransientGameObjectTypesForFaceSubdivision(SPBiomeThreadState* threadState,
	int incomingTypeCount,
	uint32_t* types,
	uint16_t* biomeTags,
	int tagCount,
	SPVec3 pointNormal, 
	SPVec3 noiseLoc, 
	uint64_t faceUniqueID, 
	int level, 
	double altitude, 
	float steepness,
	uint32_t terrainBaseType, 
	uint32_t* variations,
	int variationCount,
	float riverDistance)
{
	int addedCount = incomingTypeCount;

	if(addedCount < BIOME_MAX_GAME_OBJECT_COUNT_PER_SUBDIVISION)
	{
		if(altitude > -0.0000001)
		{
			if(level >= SP_SUBDIVISIONS - 5)
			{
				SPVec3 noiseLookup = spVec3Mul(noiseLoc, 999.0);

				if(level == SP_SUBDIVISIONS - 5)
				{
					ForestInfo forestInfo;
					memset(&forestInfo, 0, sizeof(forestInfo));
					getForestInfo(biomeTags,
						tagCount,
						steepness,
						&forestInfo);


					int treeCount = 0;
					if(forestInfo.coniferous || forestInfo.birch)
					{
						switch(forestInfo.forestDensity)
						{
						case 0:
							break;
						case 1:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 16) - 14;
							break;
						case 2:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) - 3;
							break;
						case 3:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) + 1;
							break;
						case 4:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) + 16;
							break;

						}

						if(treeCount > 0)
						{
							if(forestInfo.birch)
							{
								if(spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 100 / treeCount) == 1)
								{
									ADD_OBJECT(gameObjectType_aspen3);
								}
							}

							for(int i = 0; i < treeCount; i++)
							{
								ADD_OBJECT(getTreeType(faceUniqueID, i, &forestInfo));
							}
						}
					}
				}
				else if(level == SP_SUBDIVISIONS - 4)
				{
					ForestInfo forestInfo;
					memset(&forestInfo, 0, sizeof(forestInfo));
					getForestInfo(biomeTags,
						tagCount,
						steepness,
						&forestInfo);


					if(forestInfo.forestDensity > 0)
					{
						SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 610.0);
						double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);

						if(noiseValue > 0.3)
						{
							uint32_t treeObjectType = gameObjectType_appleTree;

							if(forestInfo.tropical)
							{
								treeObjectType = gameObjectType_bananaTree;
							}
							int objectCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 9235, 16) - 12;
							for(int i = 0; i < objectCount; i++)
							{

								ADD_OBJECT(treeObjectType);
							}
						}
						else if(noiseValue < -0.3)
						{
							if(!forestInfo.cold)
							{
								int objectCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 9235, 16) - 12;
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_orangeTree);
								}
							}
							else
							{
								int objectCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 9235, 16) - 12;
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_peachTree);
								}
							}
						}
					}

					int treeCount = 0;
					if(forestInfo.coniferous)
					{
						switch(forestInfo.forestDensity)
						{
						case 0:
							break;
						case 1:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 16) - 14;
							break;
						case 2:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) - 3;
							break;
						case 3:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) + 1;
							break;
						case 4:
							treeCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 3254, 8) + 16;
							break;

						}

						treeCount /= 4;

						for(int i = 0; i < treeCount; i++)
						{
							ADD_OBJECT(gameObjectType_smallPine);
						}
					}

					SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 400.0);
					double rawValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
					double rangedFractionValue = rawValue * rawValue * 8.0;
					if(terrainBaseType == terrainBaseType_riverSand)
					{
						rangedFractionValue += 1.0;
					}
					else if(forestInfo.river)
					{
						rangedFractionValue += 0.5;
					}

					int addBoulderCount = (spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 83637, 60) - 58 + 5 * rangedFractionValue);
					if(addBoulderCount > 0)
					{
						uint32_t rockType = gameObjectType_rockLarge;
						for(int i = 0; i < variationCount; i++)
						{
							if(variations[i] == terrainVariation_limestone)
							{
								rockType = gameObjectType_limestoneRockLarge;
							}
						}

						for(int i = 0; i < addBoulderCount; i++)
						{
							ADD_OBJECT(rockType);
						}
					}

				}
				else if(level == SP_SUBDIVISIONS - 3)
				{
					bool temperate = false;
					bool steppe = false;

					for(int i = 0; i < tagCount; i++)
					{
						if(biomeTags[i] == biomeTag_temperate)
						{
							temperate = true;
						}
						else if(biomeTags[i] == biomeTag_steppe)
						{
							steppe = true;
						}
					}

					if(temperate || steppe)
					{

						ForestInfo forestInfo;
						memset(&forestInfo, 0, sizeof(forestInfo));
						getForestInfo(biomeTags,
							tagCount,
							steepness,
							&forestInfo);

						int objectCount = 0;

						if(forestInfo.forestDensity > 0)
						{
							if(forestInfo.forestDensity > 2)
							{
								objectCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 235432, 8) - 4;
							}
							else
							{
								objectCount = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 235432, 32) - 28;
							}
						}

						if(objectCount > 0)
						{
							SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 129.0);
							double noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);

							if(noiseValue > 0.4)
							{
								if(temperate)
								{
									for(int i = 0; i < objectCount; i++)
									{
										ADD_OBJECT(gameObjectType_sunflower);
									}
								}
							}
							else if(noiseValue < -0.4)
							{
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_raspberryBush);
								}
							}
							SPVec3 offset = {0.2,0.1,0.3};
							scaledNoiseLoc = spVec3Mul(spVec3Add(noiseLookup, offset), 124.2);
							noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2); 
							if(noiseValue > 0.4)
							{
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_gooseberryBush);
								}
							}
							else if(noiseValue < -0.4)
							{
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_beetrootPlant);
								}
							}
							SPVec3 offsetB = {0.6,0.22,0.5};
							scaledNoiseLoc = spVec3Mul(spVec3Add(noiseLookup, offsetB), 134.7);
							noiseValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2); 
							if(noiseValue > 0.4)
							{
								for(int i = 0; i < objectCount; i++)
								{
									ADD_OBJECT(gameObjectType_wheatPlant);
								}
							}
						}
					}
				}
				else if(level == SP_SUBDIVISIONS - 2)
				{
					ForestInfo forestInfo;
					memset(&forestInfo, 0, sizeof(forestInfo));
					getForestInfo(biomeTags,
						tagCount,
						steepness,
						&forestInfo);

					SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 400.0);
					double rawValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
					double rangedFractionValue = rawValue * rawValue * 8.0;
					if(terrainBaseType == terrainBaseType_riverSand)
					{
						rangedFractionValue += 1.0;
					}
					else if(forestInfo.river)
					{
						rangedFractionValue += 0.5;
					}
					int objectCount = ((int)spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 1324, 20)) - 20 + 4 * rangedFractionValue;
					if(objectCount > 0)
					{
						uint32_t rockType = gameObjectType_rock;
						for(int i = 0; i < variationCount; i++)
						{
							if(variations[i] == terrainVariation_limestone)
							{
								rockType = gameObjectType_limestoneRock;
							}
						}

						for(int i = 0; i < objectCount; i++)
						{
							ADD_OBJECT(rockType);
						}
					}

					if(forestInfo.forestDensity > 0)
					{
						int denominator = 100 / (forestInfo.forestDensity * forestInfo.forestDensity);
						denominator = spMax(denominator, 2);
						int roll = spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 235432, denominator);
						if(forestInfo.coniferous && roll == 0)
						{
							ADD_OBJECT(gameObjectType_pineBranch);
						}
						if(forestInfo.birch && roll == 1)
						{
							ADD_OBJECT(gameObjectType_birchBranch);
						}
					}

				}
				else if(level == SP_SUBDIVISIONS - 1)
				{

					SPVec3 scaledNoiseLoc = spVec3Mul(noiseLookup, 4000.0);
					double rawValue = spNoiseGet(threadState->spNoise1, scaledNoiseLoc, 2);
					double rangedFractionValue = rawValue * rawValue * 8.0;
					if(terrainBaseType == terrainBaseType_riverSand)
					{
						rangedFractionValue += 1.0;
					}
					else if(getHasSingleTag(biomeTags, tagCount, biomeTag_river))
					{
						rangedFractionValue += 0.5;
					}
					int objectCount = ((int)spRandomIntegerValueForUniqueIDAndSeed(faceUniqueID, 5243, 40)) - 38 + 2 * rangedFractionValue;

					if(objectCount > 0)
					{
						if(objectCount > 0)
						{
							uint32_t rockType = gameObjectType_rockSmall;
							for(int i = 0; i < variationCount; i++)
							{
								if(variations[i] == terrainVariation_flint)
								{
									rockType = gameObjectType_flint;
									break;
								}
								else if(variations[i] == terrainVariation_limestone)
								{
									rockType = gameObjectType_limestoneRockSmall;
								}
							}

							for(int i = 0; i < objectCount; i++)
							{
								ADD_OBJECT(rockType);
							}
						}

					}
				}
			}
		}
	}
	return addedCount;
}