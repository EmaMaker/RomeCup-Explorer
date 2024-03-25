# RomeCup Explorer

This repository contains the PCBs design files and code for the RomeCup Explorer playfield.

The RomeCup is a yearly event held by [Fondazione Mondo Digitale](https://mondodigitale.org) in
Rome, Italy, revolving around robotics and artificial intelligence. A big part of the event are the
robotics competitions aimed at middle and high school students. Some categories of competition are parallel to
the [RoboCup Junior](https://junior.robocup.org) (e.g. Soccer Open/LightWeight, Rescue Line), while
others are exclusive to RomeCup, like the Explorer category and, starting 2024, the Robotic Arms
category.

In the Explorer category, robots build and programmed by the students have to navigate a maze while
finding and recognizing different "hotspots". The hotspots can be of different kinds (in ascending
	order of points earned):

- Gas sources (1)
- Light source (2)
- Sound sources (3)

In 2024, we decided to rebuild the explorer category playfield from the ground up, favouring ease of
transport and assembly once on site (RomeCup is held every year at a different location, with the
	finals taking place in Rome Capitol every year).

To follow this route, we decided to have each hotspot have its own PCB to generate light and sound,
   instead of having a single source with cables running underneath the playfield. This also makes
   rearranging the maze easier in between rounds. In this repo, you can find the KiCad project files
   for the light and sound generation PCB, along side the BOM and manufacturing files for both
   standalone boards and panels, and the Arduino code powering the AtTiny412 microcontroller used in
   the sound boards. 

If you want to know more about the thought process and design decisions for the playfield, or about
the RoboCup 2024 event in general, I have written a [blog post](https://emamaker.com/events/romecup2024)
about it.
