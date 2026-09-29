# Dishonored 2 FPS Fix v1.1.0

## What changed

- Converted the fix from a self-contained `dinput8.dll` proxy into the x64
  `Dishonored2HighFPSFix.asi` plugin.
- Removed the `DirectInput8Create` export and system DirectInput forwarding
  from the fix itself.
- Added compatibility with external x64 ASI loaders and validated Ultimate ASI
  Loader v9.7.4 both in an isolated integration host and in Dishonored 2.
- Added direct ASI and external-loader regression tests.
- Preserved the existing hash guards, hook-byte guards, configuration,
  interpolation behavior, and fail-closed handling.

## Release archives

- `Dishonored2-FPSFix-v1.1.0-plugin-only.zip` contains the ASI, configuration,
  project license, documentation, and payload hashes. It is for installations
  that already have a compatible x64 ASI loader.
  SHA-256:
  `5785197CE350A6AB7EC5B583005C3DAAC31547B7F049500A0BA8D4EAF90A6D5D`
- `Dishonored2-FPSFix-v1.1.0-with-Ultimate-ASI-Loader-v9.7.4.zip` additionally
  contains the tested x64 `dinput8.dll` loader and its required MIT notice.
  SHA-256:
  `F89970E9AD4B803D16D5C8B76F80A223DFF1F94F3A34E827CCBA260001BAACE3`

## Upgrading from v1.0.0

Version 1.0.0 used the fix itself as `dinput8.dll`. That old file is not an ASI
loader and will not discover the new plugin.

1. Close the game.
2. Back up or remove the v1.0.0 `dinput8.dll`.
3. Install a compatible x64 ASI loader as directed by that loader, or use the
   explicitly named convenience archive.
4. Copy `Dishonored2HighFPSFix.asi` and `d2-high-fps-fix.ini` beside
   `Dishonored2.exe`.

Do not overwrite an unrelated existing proxy or ASI loader. When a compatible
loader is already installed, use the plugin-only archive.

## Validation

- Supported executable SHA-256:
  `C3150F9F2D9BF967D23CA6A79BB32703B90116854AAD9892C7FC1F46A1060293`
- ASI SHA-256:
  `0AF027D752FCEEB6F590F8F381CAB29031263D97A1CC0C0E0D92D9DDB36D8535`
- Ultimate ASI Loader v9.7.4 x64 SHA-256:
  `FA266E3513D02C08A1B808F28C10538A489EAFFAA4B0707F7CC1066E71B5AFD7`
- Direct ASI loading, unsupported-host refusal, external-loader discovery,
  DirectInput forwarding, joint-pose tests, and live in-game hook installation
  passed.
- The accepted live run reached 45,000 presentation frames with zero tracker
  capacity misses, forced-upload failures, interpolation rejections, skeletal
  layout failures, or same-time mutation failures.

`ShadowTransforms` remains unsupported and disabled. Simulated cloth remains
outside the release scope.
