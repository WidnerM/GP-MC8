# Rackspace and Song Selector Extension for Gig Performer

## Overview

An extension for [Gig Performer](https://gigperformer.com) to utilize the [Morningstar MC](https://www.morningstar.io/) series of foot controllers.  This has been tested and used with the MC8 and MC6 Pro.  It should work with the MC6 as well, but has not been tested.


> _Disclaimer: This is a user-created extension and is not supported by Deskew Technologies. Extensions can cause unexpected behaviour in Gig Performer, and in extreme situations could cause it to crash. Please test it thoroughly before using it in any live performances!_

## Installation

Download the extension (library) files for either MacOS or Windows from the [Releases](https://github.com/WidnerM/GP-MC8/releases) page.  

Mac users will need to move the _libGP-MCx.dylib_ file into this folder and then restart Gig Performer:
```
/Users/Shared/Gig Performer/Extensions
```
Windows users will need to move the _GP-MCx.dll_ file into this folder and then restart Gig Performer:
```
C:\Users\Public\Documents\Gig Performer\Extensions
```
When opening Gig Performer, a prompt will appear asking to confirm whether to install the extension.

## Usage

For the extension to successfully communicate with your MCx unit you must dedicate one bank on the MCx to working with GigPerformer.

Banks have been saved in this repository for the MC8 and the MC6 Pro.  Using the Morningstar editor you must load one of these banks into your MC, then navigate to that bank on your MC before starting GigPerformer.

Documentation is available in this repository describing the functionality of the extension and how to configure your Rackspaces, Songs, and Widgets to interact with the MC controller in the manner you would like.

A demonstration Gigfile is also provided in this repository.  You should be able to load this Gigfile, see how it interacts with the controller, and look at the various widgets to see how their configurations translate into what you see on the MC controller.  This is fully described in the documentation, but may be easier to grasp by playing with the example Gigfile.

## Building the Extension

To customize/build the extension, refer to the [Gig Performer SDK](https://github.com/gigperformer/gp-sdk) GitHub page.