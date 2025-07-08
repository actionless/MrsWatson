
#ifndef MrsWatson_PluginPresetManual_h
#define MrsWatson_PluginPresetManual_h

#include "plugin/PluginPreset.h"

PluginPreset manualPluginPresetFactory(const CharString presetData);

typedef struct {
  unsigned int parameterNumber;
  float parameterValue;
} PluginPresetManualDataMembers;
typedef PluginPresetManualDataMembers *PluginPresetManualData;

#endif
