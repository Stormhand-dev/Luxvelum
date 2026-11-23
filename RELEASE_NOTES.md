## What's new in v1.0.1

- **Default settings match the tuned zone halos.** The 40 shipped zone halos were tuned with the sun and moon scatter
  at full strength, but v1.0.0 shipped `SunScatter=0.44` and `MoonScatter=0.5`, so halos looked about half as bright as
  intended. Both now default to 1.
- **Drifting fog variation on by default**: `NoiseAmount=1`, `NoiseScale=0.316`, `NoiseWindSpeed=1.98`.
- **Water clarity** defaults to 1 (was 1.4).

These are the defaults in `WTF/Luxvelum.ini`. If you keep your own settings file, change these values in the
settings window or in the file to get the new look; the Project Reforged Launcher asks before replacing an edited
file.
