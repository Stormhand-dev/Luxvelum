## What's new since Build-1

- **Multisampling with NVIDIA behind DXVK, no `dxvk.conf` edit.** Since DXVK 2.6.2 the RESZ depth copy is offered
  only to AMD and Intel cards. Where the NVIDIA driver is installed, Luxvelum now asks DXVK to report an AMD card
  (`DxvkAmdVendor=1`, the default), so the game's 2x/4x/8x options appear. Set it to 0 to keep DXVK's own vendor.
  Without DXVK, NVIDIA cards keep multisampling through NVAPI.
- **Darker nights detection.** The package ships `fogdata.bin` and `fogdata-without-darker-nights.bin`; Luxvelum
  loads the one that matches the client's night lighting, with or without Project Reforged's darker nights patch
  (patch-N).
- **Sun and moon halo per zone.** 40 zones ship tuned in `Data\Luxvelum\zones.ini`. Your own adjustments, from
  *This zone* in the settings window, go to `WTF\Luxvelum-zones.ini` and are never overwritten by an update.
- **Burning Crusade starting zones** (Eversong, Ghostlands, Quel'Danas, Azuremyst, Bloodmyst, Exodar) no longer take
  the Outland settings, and Ghostlands keeps the moon at night.
- **Local lights are much cheaper.** For example, at Darkshire with eight lamps the frame rate went from 48 to 69 FPS.
  Lamps also glow inside the clear space a ground fog keeps around the camera.
- **See-through models take the surrounding fog.** Banners, fountain water and glass now fog like the haze around them
  with `TransparentFog=1`, instead of looking dark and unfogged.
- **Targeting a unit no longer breaks the fog** on see-through models with `TransparentFog=0`.
- Settings live in `WTF\Luxvelum.ini`. The settings window names the build, and zone borders no longer flood the log.

All of the above was tested in game on the owner's client (RTX 4060, DXVK 3.1.1, Project Reforged patches).
