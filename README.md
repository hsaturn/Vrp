# Rubiks Cube Solver, Genetic tests.

## Compilation

Lol

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
