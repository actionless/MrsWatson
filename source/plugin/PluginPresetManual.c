#include "PluginPresetManual.h"

#include "logging/EventLogger.h"

#include <string.h>


static boolByte _manual_openPluginPreset(void *pluginPresetPtr) {
  // Nothing needed here
  return true;
}

static boolByte _manual_loadPluginPreset(void *pluginPresetPtr, Plugin plugin) {
  PluginPreset pluginPreset = (PluginPreset)pluginPresetPtr;
  PluginPresetManualData extraData = (PluginPresetManualData)pluginPreset->extraData;

  boolByte result = true;
  for (size_t i = 0; i < extraData->count; ++i) {
    unsigned int param = extraData->entries[i].parameterNumber;
    float value = extraData->entries[i].parameterValue;
    logInfo("Setting plugin param %u = %.3f", param, value);
    if (!plugin->setParameter(plugin, param, value)) {
      result = false;
      logError("Failed to set parameter %u", param);
    }
  }

  return result;
}

static void _manual_freePluginPresetData(void *extraDataPtr) {
  // Nothing needed here
}

////---------------------------------------------------------------------------

void parseManualPresetPart(CharString str, unsigned int* outInt, float* outFloat) {
    if (!str || !str->data) return;

    const char* separator = strchr(str->data, CHAIN_STRING_PROGRAM_DATA_VALUE_SEPARATOR);
    if (!separator) return;

    // Temporarily null-terminate at separator to isolate left part
    size_t leftLen = separator - str->data;
    char* left = (char*)malloc(leftLen + 1);
    strncpy(left, str->data, leftLen);
    left[leftLen] = '\0';

    const char* right = separator + 1;

	*outInt = (unsigned int)(strtoul(left, NULL, 10));
    *outFloat = atof(right);

    free(left);
}

////---------------------------------------------------------------------------

PluginPreset newPluginPresetManual(const CharString presetData) {
  logInfo("Got preset data: %s", presetData->data);

	PluginPresetManualData extraData = malloc(sizeof(PluginPresetManualDataMembers));
	extraData->count = 0;

	LinkedList pairs = charStringSplit(presetData, CHAIN_STRING_PROGRAM_DATA_PARAMETER_SEPARATOR);
	LinkedListIterator it = pairs;

	while (it && it->item && extraData->count < MAX_MANUAL_PRESET_PARAMS) {
		CharString pair = (CharString)(it->item);
		unsigned int parsedInt = 0;
		float parsedFloat = 0.0f;

		parseManualPresetPart(pair, &parsedInt, &parsedFloat);

		extraData->entries[extraData->count].parameterNumber = parsedInt;
		extraData->entries[extraData->count].parameterValue = parsedFloat;
		extraData->count++;

		it = it->nextItem;
	}

	freeLinkedListAndItems(pairs, (LinkedListFreeItemFunc)freeCharString);

  PluginPreset pluginPreset = (PluginPreset)malloc(sizeof(PluginPresetMembers));
  pluginPreset->presetType = PRESET_TYPE_MANUAL;
  pluginPreset->presetName = newCharString();
  charStringCopyCString(pluginPreset->presetName, MANUAL_PRESET_PLACEHOLDER_NAME);
  pluginPreset->compatiblePluginTypes = 0;
  pluginPresetSetCompatibleWith(pluginPreset, PLUGIN_TYPE_VST_2X);

  pluginPreset->openPreset = _manual_openPluginPreset;
  pluginPreset->loadPreset = _manual_loadPluginPreset;
  pluginPreset->freePresetData = _manual_freePluginPresetData;

  pluginPreset->extraData = extraData;

  return pluginPreset;
}

PluginPreset manualPluginPresetFactory(const CharString presetData) {
  return newPluginPresetManual(presetData);
}
