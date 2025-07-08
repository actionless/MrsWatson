#include "PluginPresetManual.h"

#include "logging/EventLogger.h"

#include <string.h>


static boolByte _manual_openPluginPreset(void *pluginPresetPtr) {
  /*PluginPreset pluginPreset = (PluginPreset)pluginPresetPtr;*/
  /*PluginPresetManualData extraData =*/
  /*    (PluginPresetManualData)pluginPreset->extraData;*/
  /*extraData->programNumberparameterNumber =*/
  /*    (unsigned int)strtoul(pluginPreset->presetName->data, NULL, 10);*/
  return true;
}

static boolByte _manual_loadPluginPreset(void *pluginPresetPtr,
                                                 Plugin plugin) {
  PluginPreset pluginPreset = (PluginPreset)pluginPresetPtr;
  PluginPresetManualData extraData =
	  (PluginPresetManualData)pluginPreset->extraData;
  boolByte result;
  result = plugin->setParameter(plugin, extraData->parameterNumber, extraData->parameterValue);
  return result;
}

static void _manual_freePluginPresetData(void *extraDataPtr) {
  // Nothing needed here
}

////---------------------------------------------------------------------------

void parseManualPresetPart(CharString str, unsigned int* outInt, float* outFloat) {
    if (!str || !str->data) return;

    const char* equalSign = strchr(str->data, '=');
    if (!equalSign) return;

    // Temporarily null-terminate at '=' to isolate left part
    size_t leftLen = equalSign - str->data;
    char* left = (char*)malloc(leftLen + 1);
    strncpy(left, str->data, leftLen);
    left[leftLen] = '\0';

    const char* right = equalSign + 1;

	*outInt = (unsigned int)(strtoul(left, NULL, 10));
    *outFloat = atof(right);

    free(left);
}

////---------------------------------------------------------------------------

PluginPreset newPluginPresetManual(const CharString presetData) {
  logInfo("Got preset data: %s", presetData->data);

	unsigned int parsedInt;
	float parsedFloat;

	parseManualPresetPart(presetData, &parsedInt, &parsedFloat);

	logInfo("Parsed int: %d\n", parsedInt);
	logInfo("Parsed float: %.2f\n", parsedFloat);

  PluginPreset pluginPreset = (PluginPreset)malloc(sizeof(PluginPresetMembers));
  PluginPresetManualData extraData =
      (PluginPresetManualData)malloc(sizeof(PluginPresetManualDataMembers));

  pluginPreset->presetType = PRESET_TYPE_MANUAL;
  pluginPreset->presetName = newCharString();
  charStringCopyCString(pluginPreset->presetName, "Foobar");
  pluginPreset->compatiblePluginTypes = 0;
  pluginPresetSetCompatibleWith(pluginPreset, PLUGIN_TYPE_VST_2X);

  pluginPreset->openPreset = _manual_openPluginPreset;
  pluginPreset->loadPreset = _manual_loadPluginPreset;
  pluginPreset->freePresetData = _manual_freePluginPresetData;

  extraData->parameterNumber = parsedInt;
  extraData->parameterValue = parsedFloat;

  pluginPreset->extraData = extraData;

  return pluginPreset;
}

PluginPreset manualPluginPresetFactory(const CharString presetData) {
  return newPluginPresetManual(presetData);
}
