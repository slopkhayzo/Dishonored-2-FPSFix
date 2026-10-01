# Dishonored 2 FPS Fix v1.2.0

## What changed

- Replaced the hard executable SHA-256 allowlist with strict runtime-layout
  validation. The reference hash remains logged as build provenance.
- Added PE32+ x64 image, section-permission, renderer-view call-target,
  deep-copy signature, vtable-target, mutable-data, and enabled-hook signature
  checks before the first hook is written.
- Retained the immediate hook-byte guards and added a second renderer-view
  relationship check at the mandatory camera patch boundary.
- Added separate smoke coverage for non-game host rejection and for rejecting
  an incompatible executable deliberately named `Dishonored2.exe`.
- Kept relocated or structurally changed executables fail-closed. Such builds
  still require a researched layout profile and their own live validation.

## Release archives

- `Dishonored2-FPSFix-v1.2.0-plugin-only.zip` contains the ASI, configuration,
  project license, documentation, and payload hashes for installations that
  already have a compatible x64 ASI loader.
  SHA-256:
  `D7CD22947A0A3E62929B2BE8BBAC52E14BF4B0FACFB833504C32C408A9A061CB`
- `Dishonored2-FPSFix-v1.2.0-with-Ultimate-ASI-Loader-v9.7.4.zip` additionally
  contains the pinned x64 loader and its required MIT notice.
  SHA-256:
  `490455B8BE9F84A7F1F5676250AC647307A02F16042878290645E8CABA808D26`

## Validation

- Reference executable SHA-256:
  `C3150F9F2D9BF967D23CA6A79BB32703B90116854AAD9892C7FC1F46A1060293`
- ASI SHA-256:
  `8ABAF45A7A3F1A58D5FFD5CFE1EEC674EC69070D66B043044D18C931AB19AB2E`
- Ultimate ASI Loader v9.7.4 x64 SHA-256:
  `FA266E3513D02C08A1B808F28C10538A489EAFFAA4B0707F7CC1066E71B5AFD7`
- Compilation, joint-pose tests, direct ASI loading, both fail-closed host
  tests, external-loader discovery, and DirectInput forwarding passed.
- Both extracted archives passed their internal payload-manifest checks and
  their appropriate direct or bundled-loader runtime smoke test.
- The development build was installed over v1.1.0 and live-tested without an
  observed issue before these release archives were produced.

The GOG 1.77.9.0 executable remains the fully researched and live-tested
reference. Layout-compatible admission of a different hash is deliberately
not presented as equivalent gameplay validation for another release.

`ShadowTransforms` remains unsupported and disabled. Simulated cloth remains
outside the release scope.
