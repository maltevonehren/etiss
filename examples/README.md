# ETISS Application Examples

This folder contains application examples for ETISS in the following directories:

- The `SW` folder contains example target software for simulation
  as well as toolchain files, startup code and a linker script to build them for ETISS.
  It can get build during the regular build process
  (see its [README](SW/README.md))
- The `bare_etiss_processor` used to be in this folder but has been moved to `src/bare_etiss_processor`.
  It gets build as a part of the regular build process
  (see its [README](../src/bare_etiss_processor/README.md)).
