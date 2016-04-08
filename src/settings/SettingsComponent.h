#ifndef SETTINGSCOMPONENT_H
#define SETTINGSCOMPONENT_H

#include <QObject>
#include "utils/Utils.h"
#include "ComponentManager.h"
#include "SettingsValue.h"

#define SETTINGS_SECTION_AUDIO "audio"
#define SETTINGS_SECTION_VIDEO "video"
#define SETTINGS_SECTION_MAIN "main"
#define SETTINGS_SECTION_STATE "state"
#define SETTINGS_SECTION_PATH "path"
#define SETTINGS_SECTION_WEBCLIENT "webclient"
#define SETTINGS_SECTION_SUBTITLES "subtitles"
#define SETTINGS_SECTION_OVERRIDES "overrides"
#define SETTINGS_SECTION_CEC "cec"
#define SETTINGS_SECTION_OPENELEC "openelec"
#define SETTINGS_SECTION_APPLEREMOTE "appleremote"

#define AUDIO_DEVICE_TYPE_BASIC "basic"
#define AUDIO_DEVICE_TYPE_SPDIF "spdif"
#define AUDIO_DEVICE_TYPE_HDMI "hdmi"


class SettingsSection;

///////////////////////////////////////////////////////////////////////////////////////////////////
class SettingsComponent : public ComponentBase
{
  Q_OBJECT
  DEFINE_SINGLETON(SettingsComponent);

public:
  bool componentInitialize() override;

  const char* componentName() override { return "settings"; }
  bool componentExport() override { return true; }

  SettingsSection* getSection(const QString& sectionID)
  {
    return m_sections.value(sectionID, nullptr);
  }

  // JS interface
  Q_INVOKABLE void setValue(const QString& sectionID, const QString& key, const QVariant& value);
  Q_INVOKABLE void setValues(const QVariantMap& options);
  Q_INVOKABLE QVariant value(const QString& sectionID, const QString& key);
  Q_INVOKABLE QVariant allValues(const QString& section = "");
  Q_INVOKABLE void removeValue(const QString& sectionOrKey);
  Q_INVOKABLE void resetToDefault();
  Q_INVOKABLE QVariantList settingDescriptions();

  void updatePossibleValues(const QString& sectionID, const QString& key, const QVariantList& possibleValues);

  void saveSettings();
  void saveStorage();
  void load();

  Q_SIGNAL void groupUpdate(const QString& section, const QVariant& description);

  void setUserRoleList(const QStringList& userRoles);

  // A hack to load a value from the config file at very early init time, before
  // the SettingsComponent is created.
  //
  static QVariant readPreinitValue(const QString& sectionID, const QString& key);

  // Moves the current settings file to plexmediaplayer.conf.old to make way for new
  // configuration.
  //
  static bool resetAndSaveOldConfiguration();

private:
  explicit SettingsComponent(QObject *parent = nullptr);
  bool loadDescription();
  void parseSection(const QJsonObject& sectionObject);
  int platformMaskFromObject(const QJsonObject& object);
  Platform platformFromString(const QString& platformString);
  void saveSection(SettingsSection* section);

  QMap<QString, SettingsSection*> m_sections;

  int m_settingsVersion;
  int m_sectionIndex;

  void loadConf(const QString& path, bool storage);
};

#endif // SETTINGSCOMPONENT_H
