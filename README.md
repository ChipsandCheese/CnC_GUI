# CnC_GUI

**This is prerelease software, and as such is highly unstable.**

CnC_GUI is a graphical interface for running Chips and Cheese benchmark tools. The intention is to make it easier for our readers to utilise our tools on hardware they own so that we can gather more data as part of our database of results (as found [here](https://chipsandcheese.com/memory-latency-data/)).

## Usage

**Disclaimer: The CnC_GUI application is currently not statically compiled. This means that it requires libraries to be able to run. We currently ship the libraries packaged in the ZIP folder with our Windows builds, however for Linux there are instructions below on installation of dependencies. We intend in future to ship a Flatpak package for Linux platforms to reduce the steps required to use the application.**

### Windows

For users on Windows, please extract the ZIP file as is and navigate to the `CnC GUI` directory, then the build folder located within. Once within this folder, double click `CnC_GUI.exe` to open the application. **The application does not require administrator privileges and we recommend against running as administrator at this time.**

### Linux

As of the time of writing, the application is not statically linked and requires the installation of a number of dependencies. The application has been tested on Ubuntu 22.04 and Arch Linux.

**We are working on developing a Flatpak package to reduce installation complexity. As a result, until this is available, we shall be delaying the release of pre-compiled versions of the application for Linux.**

#### Dependencies

The CnC_GUI is currently built using Qt 6.3.1 and requires the installation of these libraries to function. We recommend that you install the Qt Creator IDE and build the application from source, as installing the libraries separately is a very complex process.

## License

CnC_GUI is available under the terms of the Apache-2.0 license.

## Authors

Copyright © 2022 Chips and Cheese
