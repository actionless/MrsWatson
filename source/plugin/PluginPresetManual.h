
#ifndef MrsWatson_PluginPresetManual_h
#define MrsWatson_PluginPresetManual_h

#include "plugin/PluginPreset.h"


#define MAX_MANUAL_PRESET_PARAMS 256

#define CHAIN_STRING_PROGRAM_DATA_PARAMETER_SEPARATOR '&'
#define CHAIN_STRING_PROGRAM_DATA_VALUE_SEPARATOR '='

#define MANUAL_PRESET_PLACEHOLDER_NAME "Manual MultiParam"

typedef struct {
  unsigned int parameterNumber;
  float parameterValue;
} PluginPresetManualEntry;

typedef struct {
  PluginPresetManualEntry entries[MAX_MANUAL_PRESET_PARAMS];
  size_t count;
} PluginPresetManualDataMembers;

typedef PluginPresetManualDataMembers* PluginPresetManualData;


PluginPreset manualPluginPresetFactory(const CharString presetData);

#endif
