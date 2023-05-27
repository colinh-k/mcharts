# `mcharts`
a command-line tool to view real-time music chart data

## requirements
- GNU `make` -- makefile contains GNU-specific functions
- `gcc` -- the only tested compiler for this project
- `libcurl` -- download chart data via the network
- `libxml-2.0` -- parse chart data from an HTML document
- NOTE: building the project was only tested with `gcc` on MacOS

## usage
1. build the project
```bash
$ make
```
2. install the program by adding the build directory to the `PATH` variable, or by creating an alias for the executable. perform either of these by providing the path to your shell's profile/environment setup file (e.g., `~/.bash_profile`, `~/.zshenv`, etc). remember to run `source ~/bash_profile` (or whatever profile file you are using) so the new command becomes available. please do not run both of the commands below
```bash
# add build directory to path
$ make add-path profile=~/.bash_profile
# add executable as alias
$ make add-alias profile=~/.bash_profile
```
3. invoke the executable using the updated path or new alias
```
$ mcharts
 rank    track                             artist
---------------------------------------------------------------------
[  1]    Last Night                        Morgan Wallen
[  2]    All My Life                       Lil Durk Featuring J. Cole
[  3]    Flowers                           Miley Cyrus
[  4]    Kill Bill                         SZA
[  5]    Ella Baila Sola                   Eslabon Armado X Peso Pluma
[  6]    Calm Down                         Rema & Selena Gomez
[  7]    Creepin'                          Metro Boomin, The Weeknd & 21 Savage
[  8]    Favorite Song                     Toosii
[  9]    Fast Car                          Luke Combs
[ 10]    Anti-Hero                         Taylor Swift
...
```
currently, the output displays each track in order of rank and includes the title and artist. there are plans to add more options and include more data in the output (see [TODO](#todo) below)

## notes
- this project is essentially a web scraper because there is no api that distributes music chart data
- billboard chart link: https://www.billboard.com/charts/hot-100/

## TODO:
- [ ] add options for:
  - [ ] number of results
  - [ ] chart by region
  - [ ] chart by platform (i.e., apple music, spotify)
  - [ ] filtering by artist
- [ ] get raw accumulation data for each track (e.g., sales, # streams, etc)
- [ ] display number of weeks at position and weekly differences
