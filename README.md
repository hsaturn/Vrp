# Opengl application with remote server control
  This piece of software is hard to describe.

  It was initially a rubik's cube solver written for the challenge.
  I had the ability to connect to the software through a socket, allowing to control it remotely.
  Then I decide to use this opengl core to test the vrp algo
  Later I added a generic algorithm to have better results
  I've planned to write a 3D printer slicer with it.

  The software embeds micro applications (under the apps folder).
  Any app can be loaded dynamically and can benefit of the opengl kernel.

  Existing apps
	- Rubik cube solver (with php solver)
	- Redstone (not yet finished, and probably will never be)
	- Vrp (Best path algorithm, with genetic solve)
	- colib (...) : a great engine simulation for a startup
	- slicer, early dev. Not sure I will continue it a day, cura becomes more powerfull every day and have solved some issues I've planned to solve (walls, holes etc). But there are still some important one to be handled that cura does not today (diamond, onion textures ...)
	- world : What is that, I even don't remember. A simutrans very poor clone


## Compilation

makefile based compilation.

You'll need to instal libglew

Kubuntu dependencies
sudo apt-get install libglew-dev
sudo apt-get install freeglut3-dev
sudo apt-get install libfreetype6-dev
sudo apt-get install libsfml-dev
sudo apt-get install libglfw3-dev


SaturnLibs : git clone https://github.com/hsaturn/SaturnLib.git
	cmake .
	make
	sudo make install

libsynth : git clone https://github.com/hsaturn/synth.git
	cmake .
	make
	sudo make install

## Usage

Move the mouse over the console (bottom of the screen).

help : gives you all commands availailble (context dependent).

### Example of usage

We will there delete all running objects (created by startup.cub for example)

> objects   -> list all created objects yet
> delete {name}
> ...
> delete {name}

Then we can create a new galaxy

> new galaxy
# OK new galaxy : galaxy  <- The name of the object created

Now create a vrp and connect it to the previous object

> new vrp galaxy

Then, find the shortest way from start planet to last planet

> vrp.solve

We can start a second vrp on anotyer galaxy :
First, let move and scale the previous galaxy

> galaxy.scale=0.5
> galaxy.dx=20

Then create a new (sphere or flat) galaxy

> new galaxy sphere 100
# OK new galaxy : galaxy_1
> galaxy_1.scale=0.5
> galaxy_1.dx=-20

Create a new vrp and attach it to galaxy_1
this vrp will have another solve initial algorithm (smart)

> new vrp galaxy_1 smart
# OK new vrp : vrp_1
> vrp_1.solve

Just for fun

> new redmap   : this is an unfinished object for a redstone simulator for minecraft.
